#include <limits>

#include "WorldHeat.h"

using namespace Enki;
using namespace std;

const double WorldHeat::THERMAL_DIFFUSIVITY_AIR = 1.9e-5;
const double WorldHeat::THERMAL_DIFFUSIVITY_COPPER = 1.11e-4;

const double WorldHeat::DEFAULT_TEMPERATURE = 25;
const double WorldHeat::DEFAULT_HEAT_DIFFUSIVITY = WorldHeat::THERMAL_DIFFUSIVITY_AIR;

/*
struct Cell {
	double temperature;
	double heatDiffusivity;
	Cell ():
		temperature (WorldHeat::DEFAULT_TEMPERATURE),
		heatDiffusivity (WorldHeat::DEFAULT_HEAT_DIFFUSIVITY)
	{
	}
	Cell (const Cell &other):
		temperature (other.temperature),
		heatDiffusivity (other.heatDiffusivity)
	{
	}
};
*/

Cell::Cell ():
	temperature (WorldHeat::DEFAULT_TEMPERATURE),
	heatDiffusivity (WorldHeat::DEFAULT_HEAT_DIFFUSIVITY)
{
}

Cell::Cell (const Cell &other):
	temperature (other.temperature),
	heatDiffusivity (other.heatDiffusivity)
{
}


WorldHeat::
WorldHeat (double normalHeat, double gridScale, double borderSize, int logRate):
	AbstractGridSimulation (gridScale, borderSize),
	normalHeat (normalHeat),
	logStream (NULL),
	logRate (logRate - 1),
	iterationsToNextLog (logRate),
	relativeTime (0),
	partialAlpha (
		100 * 100 // gridScale is in centimetres
		/ (gridScale * gridScale))
{
}

WorldHeat::
~WorldHeat ()
{
	if (this->logStream != NULL) {
		cout << "Closing heat log\n";
		this->logStream->flush ();
		delete this->logStream;
	}
}

bool WorldHeat::validParameters (double deltaTime) const
{
	double alpha = 
		this->partialAlpha
		* WorldHeat::THERMAL_DIFFUSIVITY_COPPER
		* deltaTime
		;
	return alpha <= 0.25;
}

double WorldHeat::getHeatAt (const Vector &pos) const
{
	int x, y;
	toIndex (pos, x, y);
	return this->grid [this->adtIndex][x][y].temperature;
}

void WorldHeat::
setHeatAt (const Vector &pos, double value)
{
	int x, y;
	toIndex (pos, x, y);
	this->grid [this->adtIndex][x][y].temperature = value;
}

double WorldHeat::
getHeatDiffusivityAt (const Point &pos) const
{
	int x, y;
	toIndex (pos, x, y);
	return this->grid [this->adtIndex][x][y].heatDiffusivity;
}

void WorldHeat::
setHeatDiffusivityAt (const Point &pos, double value)
{
	int x, y;
	toIndex (pos, x, y);
	this->grid [0][x][y].heatDiffusivity = value;
	this->grid [1][x][y].heatDiffusivity = value;
}


void WorldHeat::
initParameters (const ExtendedWorld *world)
{
	AbstractGridSimulation::initParameters (world);
	for (int i = 0; i < 2; i++) {
		for (int x = 0; x < this->size.x; x++) {
			for (int y = 0; y < this->size.y; y++) {
				this->grid [i][x][y].temperature = this->normalHeat;
				this->grid [i][x][y].heatDiffusivity = WorldHeat::THERMAL_DIFFUSIVITY_AIR;
			}
		}
	}
	cout
		<< "Initialised heat diffusivity to " << this->grid [0][0][0].heatDiffusivity
		<< "\n";
}

void WorldHeat::
initStateComputing (double deltaTime)
{
}

void WorldHeat::
computeNextState (double deltaTime)
{
	this->relativeTime += deltaTime;
	if (this->logStream != NULL) {
		if (this->iterationsToNextLog == 0) {
			dumpState (*this->logStream);
		}
		else {
			this->iterationsToNextLog--;
		}
	}
	int nextAdtIndex = 1 - this->adtIndex;
	for (int x = 1; x < this->size.x - 1; x++) {
		for (int y = 1; y < this->size.y - 1; y++) {
			const double alpha = 
				this->partialAlpha
				* this->grid [this->adtIndex][x][y].heatDiffusivity
				* deltaTime
				;
			const double deltaHeat = 
				(
				 + this->grid [this->adtIndex][x][y + 1].temperature
				 + this->grid [this->adtIndex][x][y - 1].temperature
				 + this->grid [this->adtIndex][x + 1][y].temperature
				 + this->grid [this->adtIndex][x - 1][y].temperature
				 - 4 * this->grid [this->adtIndex][x][y].temperature
				)
				* alpha
				;
			this->grid [nextAdtIndex][x][y].temperature =
				this->grid [this->adtIndex][x][y].temperature + deltaHeat;
		}
	}
	this->adtIndex = nextAdtIndex;
}

void WorldHeat::
dumpState (ostream &os)
{
#ifdef DUMP_MATRIX
	for (int y = 1; y < this->size.y - 1; y++) {
		os << this->grid [this->adtIndex][1][y];
		for (int x = 2; x < this->size.x - 1; x++) {
			os << '\t' << this->grid [this->adtIndex][x][y];
		}
		os << '\n';
	}
	os << '\n';
#endif
// #ifdef DUMP_LINE
	os << this->relativeTime;
	for (int y = 1; y < this->size.y - 1; y++) {
		for (int x = 1; x < this->size.x - 1; x++) {
			os << '\t' << this->grid [this->adtIndex][x][y].temperature;
		}
	}
	os << '\n';
// #endif
	this->iterationsToNextLog = this->logRate;
}

void WorldHeat::
resetTemperature (double value)
{
	for (int x = this->size.x - 1; x >= 0; x--) {
		for (int y = this->size.y - 1; y >= 0; y--) {
			this->grid [this->adtIndex][x][y].temperature = value;
		}
	}
}
