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

using namespace std;
using namespace Enki;

namespace po = boost::program_options;
namespace fs = boost::filesystem;

namespace Enki {
	double env_temp;
}
static WorldExt *world;

/**
 * Timer period used in the headless simulation mode.
 */
static double timerPeriodSec = 1;

/**
 * Speed up factor used in the headless simulation mode.
 */
static double speedupFactor = 1.0;

/**
 * Function assigned to SIGALRM signal.
 */
void progress (int dummy);

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

    fs::path default_config = fs::read_symlink(fs::path("/proc/self/exe"));
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
		  "Simulation.timer_period",
		  po::value<double> (&timerPeriodSec),
		  "simulation timer period (in seconds)"
		  )
		 (
		  "Simulation.speedup_factor",
		  po::value<double> (&speedupFactor),
		  "simulation speedup factor (in seconds)"
		  )
		 (
		  "Bee.scale_factor",
		  po::value<double> (&Bee::SCALE_FACTOR),
		  "bee scale factor"
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
			(const uint32_t*) texture.constBits ()));

	WorldHeat *heatModel = new WorldHeat(env_temp, heat_scale, heat_border_size);
	if (heat_log_file_name != "") {
		heatModel->logToStream (heat_log_file_name);
	}
	world->addPhysicSimulation(heatModel);

	CasuHandler *ch = new CasuHandler();
	world->addHandler("Casu", ch);

	PhysicalObjectHandler *ph = new PhysicalObjectHandler();
	world->addHandler("Physical", ph);

	BeeHandler *bh = new BeeHandler();
	world->addHandler("Bee", bh);

	if (vm.count ("nogui") == 0) {
		QApplication app(argc, argv);

		AssisiPlayground viewer (world, heatModel, maxVibration);	
		viewer.show ();
	
		return app.exec();
	}
	else {
		/* set up the action for alarm */
		struct sigaction saProgresso;
		saProgresso.sa_handler = progress;
		saProgresso.sa_flags = 0;
		sigaction (SIGALRM, &saProgresso, 0);
		/* set up timer */
		struct itimerval value;
		timerPeriodSec = fabs (timerPeriodSec);
		value.it_interval.tv_sec = timerPeriodSec;
		long usec = timerPeriodSec * 1000000;
		while (usec > 999999) {
			usec -= 1000000;
		}
		value.it_interval.tv_usec = usec;
		value.it_value.tv_sec = 1;
		value.it_value.tv_usec = 0;
		setitimer (ITIMER_REAL, &value, NULL);
		/* block on a semaphore */
		sem_t block;
		int ret;
		do {
			ret = sem_init (&block, 0, 0);
			if (ret != 0) {
				printf ("errno=%d\n", errno);
				perror ("initialisation of playground semaphore");
				return 1;
			}
		} while (ret == -1 && errno == EAGAIN);
		do {
			do {
				ret = sem_wait (&block);
			} while (ret == -1 && errno == EINTR);
			if (ret != 0) {
				printf ("errno=%d\n", errno);
				perror ("playground blocking semaphore");
			}
		} while (ret == -1 && errno == EAGAIN);
		cout << "Simulator finished\n";
		return 0;
	}
}

void progress (int dummy)
{
	world->step (speedupFactor * timerPeriodSec);
}
