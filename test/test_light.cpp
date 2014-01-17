#include <iostream>

#include <enki/PhysicalEngine.h>

#include "../interactions/LightSensor.h"
#include "../interactions/LightSourceFromAbove.h"

using namespace Enki;

class Test:
	public Robot
{
public:
	LightSensor *lightSensor;
	LightSourceFromAbove *lightSource;
	Test (Enki::Vector location, Enki::World *world, double wavelength)
	{
		this->pos = location;
		this->lightSensor = new LightSensor (10, this, Enki::Vector (0, 1), 0, wavelength);
		this->lightSource = new LightSourceFromAbove
			(10.0, this, Enki::Vector (0, -1), 0.0,
			 500.0,   // k
			 9.0,     // radius
			 wavelength,
			 100.0,   // max intensity
			 1.0);    // sigma

		// turn the actuators to point objects so that they don't move
		this->lightSource->setCylindric (0, 0, -1);

		world->addObject (this->lightSource);
		this->addLocalInteraction (this->lightSensor);

		// this->setCylindric (CASU::RADIUS, CASU::HEIGHT, CASU::MASS);
		this->setCylindric (0, 0, -1);
	}
};

Test *test1;
Test *test2;

void debug (int time);

int main (int argc, char *argv[])
{
	// Create the world
	Enki::World world (200, 200);

	test1 = new Test (Vector (-4, 0), &world, 450);
	test2 = new Test (Vector (4, 0), &world, 550);

	world.addObject (test1);
	world.addObject (test2);
	debug (-1);
	for (int it = 0; it < 3; it++) {
		world.step (0.05);
		debug (it);
	}
	return 0;
}

void debug (int time)
{
	std::cout << time;
	std::cout << '\t' << test1->lightSensor->getIntensity ();
	std::cout << '\t' << test1->pos;
	std::cout << '\t' << test2->lightSensor->getIntensity ();
	std::cout << '\n';
}
