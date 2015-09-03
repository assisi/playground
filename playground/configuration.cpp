#include <QApplication>
#include <QImage>

#include <boost/program_options.hpp>

#include <fstream>

#include "extensions/ExtendedWorld.h"
#include "interactions/WorldHeat.h"

#include "robots/Bee.h"
#include "robots/Casu.h"
#include "handlers/PhysicalObjectHandler.h"
#include "handlers/CasuHandler.h"
#include "handlers/BeeHandler.h"

#include "configuration.h"

using namespace std;
using namespace Enki;

namespace po = boost::program_options;

namespace Enki {

	int radius;

	double heat_scale;

	int heat_border_size;

	double maxVibration;

	Mode mode;

	double env_temp;

	double timerPeriodSec = 1;
}

void Enki::processConfiguration (int argc, char *argv[])
{
	po::options_description desc("Recognized options");
    
	// Variables to store the options
	string config_file_name ("Playground.cfg");

	desc.add_options
		()
		("help,h", "produce help message")
		("nogui", "run without viewer")
		("config_file,c", 
		 po::value<string> (&config_file_name)->default_value ("Playground.cfg"),
		 "configuration file name")
		(
		 "Arena.radius,r",
		 po::value<int> (&radius), 
		 "playground radius, in cm"
		 )
		(
		 "Heat.env_temp,t",
		 po::value<double> (&env_temp), 
		 "environment temperature, in C"
		 )
		(
		 "Heat.scale",
		 po::value<double> (&heat_scale), 
		 "heat model scale"
		 )
		(
		 "Heat.border_size",
		 po::value<int> (&heat_border_size),
		 "playground radius, in cm"
		 )
		(
		 "Vibration.range",
		 po::value<double> (&Casu::VIBRATION_SOURCE_RANGE),
		 "vibration range, in cm"
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
		 "Bee.scale_factor",
		 po::value<double> (&Bee::SCALE_FACTOR),
		 "bee scale factor"
		 )
		;

	po::variables_map vm;
	po::store (po::parse_command_line (argc, argv, desc), vm);
	ifstream config_file (config_file_name.c_str (), std::ifstream::in);
	po::store(po::parse_config_file(config_file, desc), vm);
	config_file.close ();
	po::notify (vm);

	if (vm.count ("help")) {
		cout << desc << endl;
		exit (1);
	}
	if (vm.count ("nogui") == 0) {
		mode = MODE_NO_VIEWER;
	}
	else {
		mode = MODE_GUI;
	}
}

WorldExt *Enki::createWorld ()
{
	WorldExt *result;
	string pub_address ("tcp://*:5555"); 
	string sub_address ("tcp://*:5556");

	switch (mode) {
	case MODE_NO_VIEWER: {
		result = new WorldExt (radius, pub_address, sub_address);
		break;
	}
	case MODE_GUI: {
		// QImage texture (QString (":/textures/ground_grayscale.png"));
		// texture = QGLWidget::convertToGLFormat (texture);
		// result = new WorldExt (
		// 	radius, pub_address, sub_address,
		// 	Color::gray, 
		// 	World::GroundTexture (
		// 		texture.width (),
		// 		texture.height (),
		// 		(const uint32_t*) texture.constBits ()));
		break;
	}
	default:
		return NULL;
	}
	WorldHeat *heatModel = new WorldHeat (env_temp, heat_scale, heat_border_size);
	result->addPhysicSimulation (heatModel);

	// Add handlers
	CasuHandler *ch = new CasuHandler ();
	result->addHandler ("Casu", ch);

	BeeHandler *bh = new BeeHandler ();
	result->addHandler ("Bee", bh);

	PhysicalObjectHandler *ph = new PhysicalObjectHandler ();
	result->addHandler ("Physical", ph);    
	return result;
}
