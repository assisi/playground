#include <limits>
#include <fstream>

#include "WorldHeatCache.h"
#include "HeatActuator.h"

using namespace Enki;
using namespace std;

const double WorldHeat::THERMAL_DIFFUSIVITY_AIR = 1.9e-5;

const double WorldHeat::HEAT_DIFF_EPSILON = 0.1; // 1.5e-5; //1.4e-5;

ofstream ofs ("delta-heat.txt");

WorldHeat::
WorldHeat (double normalHeat, double gridScale, double borderSize):
	AbstractGridSimulation (gridScale, borderSize),
	normalHeat (normalHeat),
	timeState (CHECKING_STEADY_STATE)
{
}

double WorldHeat::getHeatAt (Vector &pos) const
{
	int x, y;
	toIndex (pos, x, y);
	return this->grid [this->adtIndex][x][y];
}

void WorldHeat::
setHeatAt (Vector &pos, double value)
{
	int x, y;
	toIndex (pos, x, y);
	switch (this->timeState) {
	case USING_CACHED_VALUES:
		if (fabs (value - this->grid [this->adtIndex][x][y]) > WorldHeat::HEAT_DIFF_EPSILON) {
			this->timeState = CHECKING_STEADY_STATE;
		}
		break;
	case CHECKING_STEADY_STATE:
		break;
	}
	this->grid [this->adtIndex][x][y] = value;
#ifdef DEBUG
	std::cout << pos << " <=> " << x << "," << y << " => " << value << '\n';
#endif
}

void WorldHeat::
computeHeatDistribution ()
{
	if (this->timeState == USING_CACHED_VALUES) {
		this->timeState == COMPUTE;
		this->clock = (int) (2000 * std::max (this->size.x, this->size.y)) + 1;
		std::cout << "Clock set at " << this->clock << "\n";
		ofs << "computeHeatDistribution\n";
		dumpState (ofs);
		ofs.flush ();
	}
}

void WorldHeat::
initParameters (const ExtendedWorld *world)
{
	AbstractGridSimulation::initParameters (world);
	for (int i = 0; i < 2; i++) {
		for (int x = 0; x < this->size.x; x++) {
			for (int y = 0; y < this->size.y; y++) {
				this->grid [i][x][y] = this->normalHeat;
			}
		}
	}
}

void WorldHeat::
initStateComputing (double deltaTime)
{
}

void WorldHeat::
computeNextState (double deltaTime)
{
	switch (this->timeState) {
	case USING_CACHED_VALUES:
		break;
	case CHECKING_STEADY_STATE: {
		double maxDeltaHeat = updateGrid (deltaTime);
		bool reachedSteadyState = maxDeltaHeat < WorldHeat::HEAT_DIFF_EPSILON;
		if (reachedSteadyState) {
			this->timeState = USING_CACHED_VALUES;
		dumpState (ofs);
		ofs.flush ();
#ifdef DEBUG
			cout << "Reached heat steady state!!!\n";
#endif
		}
	}
		break;
	case COMPUTE:
		updateGrid (deltaTime);
		this->clock--;
		if (this->clock <= 0) {
			this->timeState = CHECKING_STEADY_STATE;
		}
		break;
	}
}

double WorldHeat::
updateGrid (double deltaTime)
{
	double maxHeatDiff = std::numeric_limits<double>::min ();
	int nextAdtIndex = 1 - this->adtIndex;
	// gridScale is in centimetres
	double factor = 
		WorldHeat::THERMAL_DIFFUSIVITY_AIR
		* deltaTime
		/ (this->gridScale / 100)
		/ 4.0;
	// to avoid overshooting
	factor = std::min (factor, 1.0);
	for (int x = 1; x < this->size.x - 1; x++) {
		for (int y = 1; y < this->size.y - 1; y++) {
			double deltaHeat = 
				(
				 + this->grid [this->adtIndex][x][y + 1]
				 + this->grid [this->adtIndex][x][y - 1]
				 + this->grid [this->adtIndex][x + 1][y]
				 + this->grid [this->adtIndex][x - 1][y]
				 - 4 * this->grid [this->adtIndex][x][y]
				 )
				* factor
				;
			this->grid [nextAdtIndex][x][y] =
				this->grid [this->adtIndex][x][y] + deltaHeat;
			maxHeatDiff = std::max (maxHeatDiff, fabs (this->grid [this->adtIndex][x][y] - this->grid [this->adtIndex][x][y - 1]));
			maxHeatDiff = std::max (maxHeatDiff, fabs (this->grid [this->adtIndex][x][y] - this->grid [this->adtIndex][x - 1][y]));
		}
	}
	{
		int y = this.size.y - 1;
		for (int x = 1; x < this->size.x - 1; x++) {
			maxHeatDiff = std::max (maxHeatDiff, fabs (this->grid [this->adtIndex][x][y] - this->grid [this->adtIndex][x][y - 1]));
		}
	}
	{
		int x = this.size.x - 1;
		for (int y = 1; y < this->size.y - 1; y++) {
			maxHeatDiff = std::max (maxHeatDiff, fabs (this->grid [this->adtIndex][x][y] - this->grid [this->adtIndex][x - 1][y]));
		}
	}
	this->adtIndex = nextAdtIndex;
	ofs << maxHeatDiff << "\n";
#ifdef DEBUG
		std::cout << "Max delta: " << maxHeatDiff << std::endl;
#endif
	return maxHeatDiff;
}

void WorldHeat::
dumpState (ostream &os)
{
	for (int x = 1; x < this->size.x - 1; x++) {
		for (int y = 1; y < this->size.y - 1; y++) {
			os << '\t' << this->grid [this->adtIndex][x][y];
		}
	}
	os << '\n';
}
