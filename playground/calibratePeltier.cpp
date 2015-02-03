#include <fstream>
#include <string>
#include <iostream>
#include "configuration.h"
#include "robots/Casu.h"

using namespace Enki;
using namespace std;

int main (int argc, char *argv[])
{
	radius = 20;
	processConfiguration (argc, argv);
	// timerPeriodSec = 30.0 / 1000.0;
	cout << "Radius=" << radius << '\n';
	mode = MODE_NO_VIEWER;
	WorldExt *world = createWorld ();
	Casu *casu = new Casu (world, env_temp);
	casu->pos = Vector (heat_scale / 2, heat_scale / 2);
	world->addObject (casu);
	int i;
	ostringstream filename;
	filename
		<< "log"
		<< "_timer-period="
		<< timerPeriodSec
		<< "_thermal-response="
		<< Casu::PELTIER_THERMAL_RESPONSE
		<< "_world-temperature="
		<< env_temp
		<< ".txt";
	std::ofstream log (filename.str ().c_str ());
	const int NUM = 5;
	// double setPoints[] = {42.1493, 32.273,  37.7843, 31.4044, 33.8716};
	double setPoints[] = {42, 26,  38, 30, 34};
	int interval[] =     {1217.39, 2409.32, 3606.27, 4823.35, 6005.22};
	int index = 0;

	casu->peltier->setHeat (setPoints [index]);
	while (index < NUM) {
		world->step (timerPeriodSec);
		log << world->getAbsoluteTime ();
		for (int j = 0; j < Casu::TEMP_SENS_COUNT; j++) {
			log << '\t'
				 << casu->temp_sensors [j]->getMeasuredHeat ();
		}
		log << '\n';

		if (world->getAbsoluteTime () > interval [index]) {
			index++;
			if (index < NUM) {
				casu->peltier->setHeat (setPoints [index]);
			}
		}
	}
	return 0;
}
