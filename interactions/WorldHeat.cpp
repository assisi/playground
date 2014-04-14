#include <limits>

#include "WorldHeat.h"
#include "HeatActuator.h"

using namespace Enki;
using namespace std;

const double WorldHeat::THERMAL_DIFFUSIVITY_AIR = 1.9e-5;

WorldHeat::
WorldHeat (double normalHeat, double gridScale, double borderSize):
	adtIndex (0),
	normalHeat (normalHeat),
	gridScale (gridScale),
	borderSize (borderSize)
{
}

double WorldHeat::getHeatAt (Vector &pos) const
{
	int x, y;
	toIndex (pos, x, y);
	return this->heat [this->adtIndex][x][y];
}

void WorldHeat::setHeatAt (Vector &pos, double value)
{
	int x, y;
	toIndex (pos, x, y);
	this->heat [this->adtIndex][x][y] = value;
	// std::cout << pos << " => " << value << '\n';
}


void WorldHeat::
initParameters (const ExtendedWorld *world)
{
	// Enki::Vector min (std::numeric_limits<double>::max (), std::numeric_limits<double>::max ());
	// Enki::Vector max (std::numeric_limits<double>::min (), std::numeric_limits<double>::min ());
	
	Enki::Vector min (0, 0);
	Enki::Vector max (world->w, world->h);

	Enki::World::ObjectsIterator iterator = world->objects.begin ();
	while (iterator != world->objects.end ()) {
		Enki::PhysicalObject* po = *iterator;
		min.x = std::min (min.x, po->pos.x);
		min.y = std::min (min.y, po->pos.y);
		max.x = std::max (max.x, po->pos.x);
		max.y = std::max (max.y, po->pos.y);
		iterator++;
	}
	min.x -= borderSize + gridScale;
	min.y -= borderSize + gridScale;
	max.x += borderSize + gridScale;
	max.y += borderSize + gridScale;
	this->size = (max - min) / gridScale;
	for (int i = 0; i < 2; i++) {
		this->heat [i].resize (this->size.x);
		for (int x = 0; x < this->size.x; x++) {
			this->heat [i][x].resize (this->size.y);
			for (int y = 0; y < this->size.y; y++) {
				this->heat [i][x][y] = this->normalHeat;
			}
		}
	}
	this->origin = min;
	std::cout << "min: " << min << "\nmax: " << max << "\nsize: " << size << std::endl;
}


void WorldHeat::
initStateComputing (double deltaTime)
{
}

// void WorldHeat::
// handleObjectAction (const PhysicalObject *po)
// {
// 	HeatActuator *heatActuator = dynamic_cast<HeatActuator *>(po);
// 	if ((heatActuator != NULL) &&
// 		 heatActuator->isSwitchedOn ()) {
// 		int x, y;
// 		toIndex (heatActuator->pos, x, y);
// 		this->heat [this->adtIndex][x][y] = heatActuator->getHeat ();
// 		// std::cout << "(" << x << "," << y << ") ";
// 		// double heat = heatActuator->getHeat ();
// 		// if (this->heat [this->adtIndex][x][y] < heat) {
// 		// 	this->heat [this->adtIndex][x][y] = heat;
// 		// }
// 	}
// }

void WorldHeat::
computeNextState (double deltaTime)
{
	int nextAdtIndex = 1 - this->adtIndex;
	for (int x = 1; x < this->size.x - 1; x++) {
		for (int y = 1; y < this->size.y - 1; y++) {
			this->heat [nextAdtIndex][x][y] =
				this->heat [this->adtIndex][x][y]
				+ (
					+ this->heat [this->adtIndex][x][y + 1]
					+ this->heat [this->adtIndex][x][y - 1]
					+ this->heat [this->adtIndex][x + 1][y]
					+ this->heat [this->adtIndex][x - 1][y]
					- 4 * this->heat [this->adtIndex][x][y]
					)
				* WorldHeat::THERMAL_DIFFUSIVITY_AIR
				* deltaTime
				/ this->gridScale
				/ 4.0
				;
		}
	}
	this->adtIndex = nextAdtIndex;
}

// void WorldHeat::
// handleObjectSense (PhysicalObject *po)
// {
// 	HeatSensor *heatSensor = dynamic_cast<HeatSensor *>(po);
// 	if (heatSensor != NULL) {
		
// 	}
// }

void WorldHeat::
toIndex (const Enki::Vector& position, int &x, int &y) const
{
	// x = (position.x - this->origin.x) / this->gridScale;
	// y = (position.y - this->origin.y) / this->gridScale;
	x = round ((position.x - this->origin.x) / this->gridScale);
	y = round ((position.y - this->origin.y) / this->gridScale);
}

void WorldHeat::
dumpState (ostream &os)
{
	for (int x = 1; x < this->size.x - 1; x++) {
		for (int y = 1; y < this->size.y - 1; y++) {
			os << '\t' << this->heat [this->adtIndex][x][y];
		}
	}
	os << '\n';
}