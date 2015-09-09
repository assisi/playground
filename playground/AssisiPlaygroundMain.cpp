#include <signal.h>
#include <sys/time.h>
#include <semaphore.h>

#include <QApplication>
#include <QImage>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include "WorldExt.h"
#include "AssisiPlayground.h"

#include "extensions/ExtendedWorld.h"
#include "interactions/WorldHeat.h"

#include "handlers/PhysicalObjectHandler.h"
#include "handlers/EPuckHandler.h"
#include "handlers/CasuHandler.h"
#include "handlers/BeeHandler.h"

#include "robots/Bee.h"
#include "robots/Casu.h"

#include <iostream>
#include <fstream>

// MAC workaround for Thomas
#if defined __APPLE__
#include <mach-o/dyld.h>
#include <sys/param.h>
#endif

using namespace std;
using namespace Enki;

namespace po = boost::program_options;
namespace fs = boost::filesystem;

namespace Enki {
	double env_temp;
}

static WorldExt *world;

/**
 * Heat model used by ASSISIbf playground
 */
static WorldHeat *heatModel;

/**
 * Timer period used in the headless simulation mode.  If the timer period is
 * greater than zero we use an alarm to update the world at every {@code
 * timerPeriod} seconds.  Otherwise a loop is used to update the world.
 */
static double timerPeriod = 0.01;

/**
 * Camera view parameters.
 */
static double cameraPosX = 0;
static double cameraPosY = 0;
static double cameraAltitude = 1;
static char layer = 'N';

/**
 * Delta time used to update world state.  It is equal to the value used by
 * enki.  Unit is seconds.
 */
static const double DELTA_TIME = .03;

static unsigned int skewMonitorRate = 60;

static double skewReportThreshold = 0.05;

/**
 * Semaphore used in the headless simulation mode with an alarm to block the
 * main thread.
 */
static sem_t block;

#define PHYSICS_OVERSAMPLING 3

static bool go = true;

int runTimer ();

void runLoop ();

int main(int argc, char *argv[])
{
	//	QApplication app(argc, argv);
	
    /** Parse command line options **/
    
    po::options_description desc("Recognized options");
    
    // Variables to store the options
    int r;
    string config_file_name("Playground.cfg");
	// Create the world and the viewer
    string pub_address("tcp://*:5555"); 
    string sub_address("tcp://*:5556");
    string heat_log_file_name;
    double heat_scale;
    int heat_border_size;

    double maxVibration;
    double parallelismLevel = 1.0;

    // Bee physical parameters
    double bee_body_length, bee_body_width, bee_body_height,
        bee_body_mass, bee_max_speed;

    fs::path default_config = fs::path("");
    // MAC workaround for Thomas
#if defined __APPLE__
    char * pathbuf = new char[MAXPATHLEN];
    uint32_t* bufsize = new uint32_t(MAXPATHLEN);
    _NSGetExecutablePath(pathbuf, bufsize);
    default_config = fs::path(pathbuf);
    delete [] pathbuf;
    delete bufsize;
#elif defined __linux__
    default_config = fs::read_symlink(fs::path("/proc/self/exe"));
#endif

    default_config.remove_filename() /= "Playground.cfg";

    desc.add_options
        ()
        ("help,h", "produce help message")
        ("nogui", "run without viewer")
        ("config_file,c", 
         po::value<string>(&config_file_name)->default_value(default_config.native()),
         "configuration file name")
        ("pub_addr",
         po::value<string>(&pub_address)->default_value("tcp://*:5555"),
         "Address for publishing data, in the form tcp://hostname:port")
        ("sub_addr",
         po::value<string>(&sub_address)->default_value("tcp://*:5556"),
         "Address for subscribing to commands, in the form tcp://hostname:port")
        ("Arena.radius,r", po::value<int>(&r), 
         "playground radius, in cm")
        ("Heat.env_temp,t", po::value<double>(&env_temp), 
         "environment temperature, in C")
        ("Heat.scale", po::value<double>(&heat_scale), 
         "heat model scale")
        ("Heat.border_size", po::value<int>(&heat_border_size), "playground radius, in cm")
        (
            "Vibration.range",
            po::value<double> (&Casu::VIBRATION_SOURCE_RANGE),
            "vibration range, in cm"
            )
        (
            "Heat.log_file",
            po::value<string> (&heat_log_file_name)->default_value (""),
            "heat log file name"
            )
        (
            "Heat.cell_dissipation",
            po::value<double> (&WorldHeat::CELL_DISSIPATION),
            "heat lost by cells directly to outside world"
            )
        (
            "AirFlow.pump_range",
            po::value<double> (&Casu::AIR_PUMP_RANGE),
            "maximum range of CASU air pump"
            )
        (
            "AirFlow.sensor_range",
            po::value<double> (&Bee::AIR_FLOW_SENSOR_RANGE),
            "maximum range of Bee air flow sensor"
            )
        (
            "Vibration.maximum_amplitude", 
            po::value<double> (&Casu::VIBRATION_SOURCE_MAXIMUM_AMPLITUDE),
            "maximum amplitude of vibration"
            )
        (
            "Vibration.amplitude_quadratic_decay", 
            po::value<double> (&Casu::VIBRATION_SOURCE_AMPLITUDE_QUADRATIC_DECAY),
            "quadratic decay of vibration amplitude"
            )
        (
            "Vibration.noise", 
            po::value<double> (&Casu::VIBRATION_SOURCE_NOISE),
            "vibration frequency noise"
            )
        (
            "Peltier.thermal_response", 
            po::value<double> (&Casu::PELTIER_THERMAL_RESPONSE),
            "peltier thermal response"
            )
        (
            "Viewer.max_vibration",
            po::value<double> (&maxVibration),
            "maximum displayed vibration intensity"
            )
        (
            "Viewer.no_help",
            "do not show help text"
            )
        (
            "Viewer.layer",
            po::value<char> (&layer),
            "which data layer should be displayed by default: N none, H heat, V vibration, A airflow"
            )
        (
            "Simulation.timer_period",
            po::value<double> (&timerPeriod),
            "simulation timer period (in seconds)"
            )
        (
            "Simulation.parallelism_level",
            po::value<double> (&parallelismLevel),
            "Percentage of CPU threads to use"
            )
        (
            "Bee.body_length",
            po::value<double> (&bee_body_length),
            "Bee body length"
            )
        (
            "Bee.body_width",
            po::value<double> (&bee_body_width),
            "Bee body width"
            )
        (
            "Bee.body_height",
            po::value<double> (&bee_body_height),
            "Bee body height"
            )
        (
            "Bee.body_mass",
            po::value<double> (&bee_body_mass),
            "Bee body mass"
            )
        (
            "Bee.max_speed",
            po::value<double> (&bee_max_speed),
            "Maximum bee motion velocity"
            )
        (
            "Camera.pos_x",
            po::value<double> (&cameraPosX),
            "camera x position"
            )
       (
           "Camera.pos_y",
           po::value<double> (&cameraPosY),
           "camera y position"
           )
        (
           "Camera.altitude",
           po::value<double> (&cameraAltitude),
           "camera altitude"
            )
        (
             "Skew.threshold",
             po::value<double> (&skewReportThreshold),
             "Threshold to print a message because of skewness between real time and simulated time"
            )
        ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    // here we read the config file, so it seems crucial that notify is executed first
    ifstream config_file(config_file_name.c_str(), std::ifstream::in);
    po::store(po::parse_config_file(config_file, desc), vm);
    config_file.close();
	// not clear what the consequences of >1 notify call are, but no issues noticed
    po::notify(vm);

    if (vm.count("help"))
    {
        cout << desc << endl;
        return 1;
    }

    //QImage texture("playground/world.png");
    QImage texture(QString(":/textures/ground_grayscale.png"));
    texture = QGLWidget::convertToGLFormat(texture);    
    //texture.invertPixels(QImage::InvertRgba);
    
    world = new WorldExt (r, pub_address, sub_address,
        Color::gray, 
        World::GroundTexture (
            texture.width(),
            texture.height(),
            (const uint32_t*) texture.constBits ()),
        skewMonitorRate,
        skewReportThreshold);

    heatModel = new WorldHeat (world, env_temp, heat_scale, heat_border_size, parallelismLevel);
	if (heat_log_file_name != "") {
		heatModel->logToStream (heat_log_file_name);
	}
	world->addPhysicSimulation(heatModel);

	CasuHandler *ch = new CasuHandler();
	world->addHandler("Casu", ch);

	PhysicalObjectHandler *ph = new PhysicalObjectHandler();
	world->addHandler("Physical", ph);

	BeeHandler *bh = new BeeHandler(bee_body_length,bee_body_width, bee_body_height,
                                    bee_body_mass, bee_max_speed);
	world->addHandler("Bee", bh);

	if (vm.count ("nogui") == 0) {
		QApplication app(argc, argv);

		AssisiPlayground viewer (world, heatModel, maxVibration);
		if (!heatModel->validParameters (viewer.timerPeriodMs / 1000.)) {
			cerr << "Parameters of heat model are not valid!\nExiting.\n";
			return 1;
		}
      if (vm.count ("Camera.pos_x") > 0
          || vm.count ("Camera.pos_y") > 0
          || vm.count ("Camera.altitude") > 0) {
         viewer.setCameraPosition (-cameraPosX, -cameraPosY, cameraAltitude);
      }
      if (vm.count ("Viewer.no_help") > 0) {
         viewer.showHelp = false;
      }
      if (vm.count ("Viewer.layer") > 0) {
         switch (layer) {
         case 'H':
            viewer.layerToDraw = AssisiPlayground::HEAT;
            break;
         case 'V':
            viewer.layerToDraw = AssisiPlayground::VIBRATION;
            break;
         case 'A':
            viewer.layerToDraw = AssisiPlayground::AIR_FLOW;
            break;
         case 'N':
         default:
            viewer.layerToDraw = AssisiPlayground::NONE;
            break;
         }
      }
		viewer.show ();
	
		return app.exec();
	}
	else {
		if (!heatModel->validParameters (DELTA_TIME)) {
			cerr << "Parameters of heat model are not valid!\nExiting.\n";
			return 1;
		}
		int ret;
		if (timerPeriod == 0) {
			runLoop ();
			ret = 0;
		}
		else {
			ret = runTimer ();
		}
		/* clean up */
		delete world;
		delete heatModel;
		cout << "Simulator finished CORRECTLY!!!\n";
		return ret;
	}
}

/**
 * Function assigned to SIGQUIT, SIGINT and SIGTERM signals.
 */
void finishLoop (int dummy)
{
	cout << "Received signal " << dummy << "\n";
	go = false;
}

void runLoop ()
{
	/* set up the action for control-C */
	struct sigaction saFinish;
	saFinish.sa_handler = finishLoop;
	saFinish.sa_flags = 0;
	sigaction (SIGQUIT, &saFinish, 0);
	sigaction (SIGINT, &saFinish, 0);
	sigaction (SIGTERM, &saFinish, 0);
	/* main loop */
	while (go) {
		world->step (DELTA_TIME, PHYSICS_OVERSAMPLING);
	}
	return ;
}

/**
 * Function assigned to SIGALRM signal.
 */
void progress (int dummy)
{
	world->step (DELTA_TIME, PHYSICS_OVERSAMPLING);
}

/**
 * Function assigned to SIGQUIT, SIGINT and SIGTERM signals.
 */
void finishTimer (int dummy)
{
	if (sem_post (&block) == -1) {
	 	perror ("Error unlocking blocking semaphore");
	}
}

int runTimer ()
{
	/* set up the action for control-C */
	struct sigaction saFinish;
	saFinish.sa_handler = finishTimer;
	saFinish.sa_flags = 0;
	sigaction (SIGHUP, &saFinish, 0);
	sigaction (SIGQUIT, &saFinish, 0);
	sigaction (SIGINT, &saFinish, 0);
	sigaction (SIGTERM, &saFinish, 0);

	/* set up the action for alarm */
	struct sigaction saProgresso;
	saProgresso.sa_handler = progress;
	saProgresso.sa_flags = 0;
	sigaction (SIGALRM, &saProgresso, 0);
	/* set up timer */
	struct itimerval value;
	timerPeriod = fabs (timerPeriod);
	value.it_interval.tv_sec = (time_t) timerPeriod;
	long usec = timerPeriod * 1000000;
	while (usec > 999999) {
		usec -= 1000000;
	}
	value.it_interval.tv_usec = usec;
	value.it_value.tv_sec = 1;
	value.it_value.tv_usec = 0;
	setitimer (ITIMER_REAL, &value, NULL);
	/* initialise blocking a semaphore */
	int ret;
	do {
		ret = sem_init (&block, 0, 0);
		if (ret != 0) {
			printf ("errno=%d\n", errno);
			perror ("initialisation of playground semaphore");
			return 1;
		}
	} while (ret == -1 && errno == EAGAIN);
	/* block on a semaphore */
	do {
		do {
			ret = sem_wait (&block);
		} while (ret == -1 && errno == EINTR);
		if (ret != 0) {
			printf ("errno=%d\n", errno);
			perror ("playground blocking semaphore");
		}
	} while (ret == -1 && errno == EAGAIN);
	/* disable timer */
	value.it_interval.tv_sec = 0;
	value.it_interval.tv_usec = 0;
	value.it_value.tv_sec = 0;
	value.it_value.tv_usec = 0;
	setitimer (ITIMER_REAL, &value, NULL);
	return 0;
}
