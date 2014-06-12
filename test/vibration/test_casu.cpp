#include <iostream>
#include <fstream>

#include <boost/foreach.hpp>
#include <boost/math/constants/constants.hpp>

#include <enki/PhysicalEngine.h>

#include "../robots/Casu.h"
#include "../robots/Bee.h"

const double pi = boost::math::constants::pi<double>();

using namespace Enki;

Point BEE_SHAPE[] = {
	Point ( Bee::LENGTH/2,                           Bee::WIDTH/4),
	Point ( Bee::LENGTH/2 - Bee::WIDTH/(4*sqrt(2)),  Bee::WIDTH/2),
	Point (-Bee::LENGTH/2 + Bee::WIDTH/(4*sqrt(2)),  Bee::WIDTH/2),
	Point (-Bee::LENGTH/2,                           Bee::WIDTH/4),
	Point (-Bee::LENGTH/2,                          -Bee::WIDTH/4),
	Point (-Bee::LENGTH/2 + Bee::WIDTH/(2*sqrt(2)), -Bee::WIDTH/2),
	Point ( Bee::LENGTH/2 - Bee::WIDTH/(2*sqrt(2)), -Bee::WIDTH/2),
	Point ( Bee::LENGTH/2,                          -Bee::WIDTH/4)
};

Point CASU_SHAPE[] = {
	Point (Casu::RADIUS * cos (pi / 6 + 0 * pi / 3), Casu::RADIUS * sin (pi / 6 + 0 * pi / 3)),
	Point (Casu::RADIUS * cos (pi / 6 + 1 * pi / 3), Casu::RADIUS * sin (pi / 6 + 1 * pi / 3)),
	Point (Casu::RADIUS * cos (pi / 6 + 2 * pi / 3), Casu::RADIUS * sin (pi / 6 + 2 * pi / 3)),
	Point (Casu::RADIUS * cos (pi / 6 + 3 * pi / 3), Casu::RADIUS * sin (pi / 6 + 3 * pi / 3)),
	Point (Casu::RADIUS * cos (pi / 6 + 4 * pi / 3), Casu::RADIUS * sin (pi / 6 + 4 * pi / 3)),
	Point (Casu::RADIUS * cos (pi / 6 + 5 * pi / 3), Casu::RADIUS * sin (pi / 6 + 5 * pi / 3)),
};

const double CASU_OUTER_RING = 0.03;

class Experiment
{
	Enki::World *world;
	std::vector<Casu *> casus;
	std::vector<Bee *> bees;
	double end;
	double stepTime;
	unsigned int numberSamples;
	double wavelength;
public:
	Experiment (char *configFilename, std::istream &is, char*, std::ostream &osGnuplot);
	void run (std::ostream &os);
	void writeGnuplot (char *configFilename, char *dataFilename, std::ostream &osGnuplot, char* pngFilename, char* lightIntensityMap, char *vibrationAmplitudeMap);
	void sampleLightIntensity (std::ostream &os);
	void sampleVibrationIntensity (std::ostream &os);
};

int main (int argc, char* argv[])
{
	if (argc == 7) {
		char *configFilename = argv [1];
		char *dataFilename = argv [2];
		char *vibrationAmplitudeMap = argv [6];
		std::ifstream ifs (configFilename);
		std::ofstream ofsData (dataFilename);
		std::ofstream ofsGnuplot (argv [3]);
		std::ofstream ofsLightMap (argv [4]);
		std::ofstream ofsVibrationMap (vibrationAmplitudeMap);
		Experiment experiment (configFilename, ifs, argv [2], ofsGnuplot);
		experiment.writeGnuplot (configFilename, dataFilename, ofsGnuplot, argv [5], argv [4], vibrationAmplitudeMap);
		experiment.run (ofsData);
		experiment.sampleLightIntensity (ofsLightMap);
		experiment.sampleVibrationIntensity (ofsVibrationMap);
		ofsLightMap.close ();
		ofsGnuplot.close ();
		ofsData.close ();
	}
	else {
		std::cerr << "Usage: " << argv [0] << " CONFIGURATION_FILENAME DATA_FILENAME GNUPLOT_FILENAME PNG_FILENAME\n";
	}
	return 0;
}


void gnuplotDrawShape (std::ostream &os, Point base, int numberPoints, Point *delta)
{
	for (int j = 0; j < numberPoints - 1; j++) {
		os
			<< "set arrow from "
			<< (base.x + delta [  j  ].x) << ", " << (base.y + delta [j].y)
			<< " to "
			<< (base.x + delta [j + 1].x) << ", " << (base.y + delta [j + 1].y)
			<< " nohead\n";
	}
	os
		<< "set arrow from "
		<< (base.x + delta [numberPoints - 1].x) << ", " << (base.y + delta [numberPoints - 1].y)
		<< " to "
		<< (base.x + delta [0].x)                << ", " << (base.y + delta [0].y)
		<< " nohead\n";
}

Experiment::Experiment (char *configFilename, std::istream &isConfig, char* dataFilename, std::ostream &osGnuplot)
{
	Point point;
	double width, height;
	isConfig >> this->stepTime >> this->end;
	isConfig >> width >> height;

	this->world = new Enki::World (width, height);
	int numberCASUs;
	isConfig >> numberCASUs;
	this->casus.reserve (numberCASUs);
	for (int i = 0; i < numberCASUs; i++) {
		isConfig
			>> point.x >> point.y;

		Casu *casu = new Casu (world, &point);
		this->casus.insert (this->casus.end (), casu);
		isConfig
			>> casu->lightSource->intensity
			>> casu->vibrationSource->amplitude
			>> casu->vibrationSource->frequency;
		this->world->addObject (casu);
		std::cerr << "Read casu #" << i << std::endl;


		// gnuplotDrawShape (osGnuplot, point, 6, CASU_SHAPE);

	}
	int numberBees;
	isConfig >> numberBees;
	this->bees.reserve (numberBees);
	for (int i = 0; i < numberBees; i++) {
		isConfig >> point.x >> point.y; 
		Bee *bee = new Bee (&point);
		this->bees.insert (this->bees.end (), bee);
		this->world->addObject (bee);
		std::cerr << "Read bee #" << i << std::endl;

	}
	isConfig >> this->numberSamples >> this->wavelength;
}

void Experiment::run (std::ostream &os)
{
	double time = 0;
	while (time < this->end) {
		os << time;
		BOOST_FOREACH (Casu *casu, this->casus) {
			os << '\t';
			os << casu->pos.x;
			os << '\t';
			os << casu->pos.y;
		}
		BOOST_FOREACH (Bee *bee, this->bees) {
			os << '\t';
			os << bee->pos.x;
			os << '\t';
			os << bee->pos.y;
			os << '\t';
			os << bee->leftSpeed;
			os << '\t';
			os << bee->rightSpeed;
			os << '\t';
			// os << bee->frequencyGradientAlpha;
			os << bee->vibrationGradientAlpha;
			BOOST_FOREACH (LightSensor *lightSensor, bee->lightSensors) {
				os << '\t';
				os << lightSensor->getIntensity ();
			}
			BOOST_FOREACH (VibrationIntensitySensor *vibrationSensor, bee->vibrationSensors) {
				os << '\t';
				os << vibrationSensor->getIntensity ();
			}
		}
		os << std::endl;


		world->step (this->stepTime);
		BOOST_FOREACH (Bee *bee, this->bees) {
			bee->controlStep (time, this->world);
		}
		time += this->stepTime;
	}
}

void Experiment::writeGnuplot (char *configFilename, char *dataFilename, std::ostream &osGnuplot, char* pngFilename, char* lightIntensityMap, char *vibrationAmplitudeMap)
{
	osGnuplot
		<< "set terminal png enhanced large size 1200, 800\n"
		<< "set output '" << pngFilename << "'\n"
		<< "set xrange [" << this->world->w * -0.1 << " : " << this->world->w * 1.1 << "]\n"
		<< "set yrange [" << this->world->h * -0.1 << " : " << this->world->h * 1.1 << "]\n"
		<< "set xtics mirror 0, " << this->world->w << "\n"
		<< "set ytics mirror 0, " << this->world->h << "\n"
		<< "set size ratio -1\n"
		<< "set multiplot layout 2,2 title '" << configFilename << "'\n"
		;
	BOOST_FOREACH (Casu *casu, this->casus) {
		gnuplotDrawShape (osGnuplot, casu->pos, 6, CASU_SHAPE);
	}
	BOOST_FOREACH (Bee *bee, this->bees) {
		gnuplotDrawShape (osGnuplot, bee->pos, 8, BEE_SHAPE);
	}
	osGnuplot
		<< "set parametric\n"
		<< "set title 'CASU arena'\n"
		<< "plot 1/0,1/0 notitle";


	BOOST_FOREACH (Casu *casu, this->casus) {
		osGnuplot
			<< ", \\\n  "
			<< casu->pos.x << " + " << CASU_OUTER_RING << " * cos(t), "
			<< casu->pos.y << " + " << CASU_OUTER_RING << " * sin(t) notitle with lines lc 1";
	}
	const int BEE_RECORD_SIZE = 5 + Bee::NUMBER_LIGHT_SENSORS + Bee::NUMBER_VIBRATION_SENSORS;
	const int CASU_RECORD_SIZE = 2;
	const int START_BEE_RECORDS = 1 + this->casus.size () * CASU_RECORD_SIZE;

	int beeIndex = 0;
	BOOST_FOREACH (Bee *bee, this->bees) {
		osGnuplot
			<< ", \\\n  '" << dataFilename
			<< "' using "
			<< (START_BEE_RECORDS + 1 + BEE_RECORD_SIZE * beeIndex) << ":"
			<< (START_BEE_RECORDS + 2 + BEE_RECORD_SIZE * beeIndex)
			<< " notitle with lines"
			<< ", \\\n  '" << dataFilename
			<< "' using "
			<< (START_BEE_RECORDS + 1 + BEE_RECORD_SIZE * beeIndex) << ":"
			<< (START_BEE_RECORDS + 2 + BEE_RECORD_SIZE * beeIndex)
			<< " notitle with points pt 0"
			;
		beeIndex++;
	}
	osGnuplot << "\n";

	osGnuplot
		<< "set xrange [0 : " << this->world->w << "]\n"
		<< "set yrange [0 : " << this->world->h << "]\n"
		<< "set xtics mirror autofreq\n"
		<< "set ytics mirror autofreq\n"
		;
	osGnuplot
		<< "set title 'light intensity map " << this->wavelength << "'\n"
		<< "plot '" << lightIntensityMap << "' using 1:2:3 with image notitle\n"
		;
	osGnuplot
		<< "set title 'vibration intensity map'\n"
		<< "plot '" << vibrationAmplitudeMap << "' using 1:2:3 with image notitle\n"
		;
	osGnuplot
		<< "set title 'sensed alpha vibration intensity gradient'\n"
		<< "set size ratio 1 0.5, 0.5\n"
		<< "set xrange [0:*]\n"
		<< "set xtics auto\n"
		<< "set xlabel 'time'\n"
		<< "set yrange [-180:180]\n"
		<< "set ylabel 'angle'\n"
		<< "set ytics auto\n"
		<< "plot 1/0,1/0 notitle"
		;
	beeIndex = 0;
	BOOST_FOREACH (Bee *bee, this->bees) {
		osGnuplot
			<< ", \\\n  '" << dataFilename
			<< "' using 1:($"
			<< (START_BEE_RECORDS + 5 + BEE_RECORD_SIZE * beeIndex)
			<< "*45/atan(1)) title 'bee " << beeIndex << "' with lines"
			;
		beeIndex++;
	}
	osGnuplot << "\n";

	osGnuplot << "unset multiplot\n";
}

void Experiment::sampleLightIntensity (std::ostream &os)
{
	double dx = this->world->w / this->numberSamples;
	double dy = this->world->h / this->numberSamples;
	double deltaSpace = std::min (dx, dy);
	Point position;
	for (position.x = 0; position.x < world->w; position.x += deltaSpace) {
		for (position.y = 0; position.y < world->h; position.y += deltaSpace) {
			double intensity = 0;
			for (World::ObjectsIterator object = world->objects.begin (); object != world->objects.end (); ++object) {
				LightSource *lightSource = dynamic_cast<LightSource *>(*object);
				if (lightSource != NULL) {
					intensity += lightSource->getIntensityAt (position, wavelength);
				}
			}
			os << position.x << '\t' << position.y << '\t' << intensity << '\n';
		}
	}
}

void Experiment::sampleVibrationIntensity (std::ostream &os)
{
	double dx = world->w / numberSamples;
	double dy = world->h / numberSamples;
	double deltaSpace = std::min (dx, dy);
	Point position;
	for (position.x = 0; position.x < world->w; position.x += deltaSpace) {
		for (position.y = 0; position.y < world->h; position.y += deltaSpace) {
			double amplitude = 0;
			for (World::ObjectsIterator object = world->objects.begin (); object != world->objects.end (); ++object) {
				VibrationSource *vibrationSource = dynamic_cast<VibrationSource *>(*object);
				if (vibrationSource != NULL) {
					amplitude += vibrationSource->getIntensityAt (position);
				}
			}
			os << position.x << '\t' << position.y << '\t' << amplitude << '\n';
		}
	}
}
