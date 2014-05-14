#include <iostream>
#include <algorithm>

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
		this->lightSensor = new LightSensor (20, this, Enki::Vector (0, 1), 0, wavelength);
		this->lightSource = new LightSourceFromAbove
			(20.0, this, Enki::Vector (0, -1), 0.0,
			 1.0,   // k
			 9.0,     // radius
			 wavelength,
			 100.0,   // max intensity
			 1.0);    // sigma

		// turn the actuators to point objects so that they don't move
		this->lightSource->setCylindric (0, 0, -1);

		world->addObject (this->lightSource);
		this->addLocalInteraction (this->lightSensor);
		PhysicalObject::dryFrictionCoefficient = 1000; // Casus are immovable

		// this->setCylindric (CASU::RADIUS, CASU::HEIGHT, CASU::MASS);
		this->setCylindric (0, 0, -1);
	}
};

Test *test[2];


void debug (int time);
void sampleLightIntensity (Enki::World *world, unsigned int numberSamples, double wavelength);

int main (int argc, char *argv[])
{
	// Create the world
	Enki::World world (50, 50);

	test[0] = new Test (Vector (10, 10), &world, 450);
	test[1] = new Test (Vector (15, 10), &world, 550);

	world.addObject (test[0]);
	world.addObject (test[1]);
	debug (-1);
	for (int it = 0; it < 3; it++) {
		world.step (0.05);
		debug (it);
	}
	sampleLightIntensity (&world, 50, 450);
	return 0;
}

void sampleLightIntensity (Enki::World *world, unsigned int numberSamples, double wavelength)
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
				LightSource *lightSource = dynamic_cast<LightSource *>(*object);
				if (lightSource != NULL) {
					intensity += lightSource->getIntensityAt (position, wavelength);
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
		std::cout << '\t' << test [i]->lightSensor->getIntensity ();
		std::cout << '\t' << test [i]->lightSource->getAbsolutePosition ();
	}

	std::cout << '\n';
}
