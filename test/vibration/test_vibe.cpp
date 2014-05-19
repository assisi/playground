#include <iostream>
#include <algorithm>

#include <enki/PhysicalEngine.h>

#include "interactions/VibrationSensor.h"
#include "interactions/QuadraticVibrationSource.h"

#include "robots/FCULBee.h"

using namespace Enki;

class Test:
	public Robot
{
public:
	VibrationSensor *vibrationSensor;

	QuadraticVibrationSource *vibrationSource;

	Test (Enki::Vector location, Enki::World *world, double amplitude, double frequency)
	{
		this->pos = location;
		this->vibrationSensor = new VibrationSensor
			(20, this,
			 Enki::Vector (0, 1), Component::OMNIDIRECTIONAL,
			 amplitude, frequency, 1.0, 1.0);
		this->vibrationSource = new QuadraticVibrationSource
			(20.0, this, Enki::Vector (0, -1),
			 1.0);   // a
		this->vibrationSource->amplitude = amplitude;

		// turn the actuators to point objects so that they don't move
		this->vibrationSource->setCylindric (0, 0, -1);

		world->addObject (this->vibrationSource);
		this->addLocalInteraction (this->vibrationSensor);
		PhysicalObject::dryFrictionCoefficient = 1000; // Casus are immovable

		// this->setCylindric (CASU::RADIUS, CASU::HEIGHT, CASU::MASS);
		this->setCylindric (0, 0, -1);
	}
};

Test *test[2];


void debug (int time);
void sampleVibrationIntensity (Enki::World *world, unsigned int numberSamples, double time);

int main (int argc, char *argv[])
{
	// Create the world
	Enki::World world (50, 50);

	test[0] = new Test (Vector (10, 10), &world, 0.1, 2);
	test[1] = new Test (Vector (15, 10), &world, 0.05, 3);

	world.addObject (test[0]);
	world.addObject (test[1]);
	debug (-1);
	for (int it = 0; it < 30; it++) {
		world.step (0.05);
		debug (it);
	}
	// sampleVibrationIntensity (&world, 50, 450);
	return 0;
}

void sampleVibrationIntensity (Enki::World *world, unsigned int numberSamples, double time)
{
	const double deltaTime = 1;
	double dx = world->w / numberSamples;
	double dy = world->h / numberSamples;
	double deltaSpace = std::min (dx, dy);
	Point position;
	for (position.x = 0; position.x < world->w; position.x += deltaSpace) {
		for (position.y = 0; position.y < world->h; position.y += deltaSpace) {
			double intensity = 0;
			for (World::ObjectsIterator object = world->objects.begin (); object != world->objects.end (); ++object) {
				VibrationSource *vibrationSource = dynamic_cast<VibrationSource *>(*object);
				if (vibrationSource != NULL) {
					intensity += vibrationSource->getAmplitudeAt (position, time);
				}
			}
			std::cout << '\t' << intensity;
		}
		std::cout << '\n';
	}
}

void debug (int time)
{
	std::cout << time;
	for (int i = 0; i < 2; i++) {
		std::cout << '\t' << test [i]->pos;
		std::cout << '\t' << test [i]->vibrationSensor->getAmplitude ();
		std::cout << '\t' << test [i]->vibrationSource->pos;
		std::cout << '\t' << test [i]->vibrationSource->amplitude;
	}

	std::cout << '\n';
}
