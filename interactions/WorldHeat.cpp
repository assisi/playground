#include <limits>

#include "WorldHeat.h"

using namespace Enki;
using namespace std;

const double WorldHeat::THERMAL_DIFFUSIVITY_AIR = 1.9e-5;
const double WorldHeat::THERMAL_DIFFUSIVITY_COPPER = 1.11e-4;

WorldHeat::
WorldHeat (double normalHeat, double gridScale, double borderSize):
	AbstractGridSimulation (gridScale, borderSize),
	normalHeat (normalHeat),
	logStream (NULL),
	relativeTime (0),
	// gridScale is in centimetres
	maxDeltaTime ((gridScale / 100) * (gridScale / 100) / WorldHeat::THERMAL_DIFFUSIVITY_COPPER)
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

double WorldHeat::getHeatAt (const Vector &pos) const
{
	int x, y;
	toIndex (pos, x, y);
	return this->grid [this->adtIndex][x][y];
}

void WorldHeat::
setHeatAt (const Vector &pos, double value)
{
	int x, y;
	toIndex (pos, x, y);
	this->grid [this->adtIndex][x][y] = value;
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
	this->relativeTime += deltaTime;
	if (this->logStream != NULL) {
		dumpState (*this->logStream);
	}
	int nextAdtIndex = 1 - this->adtIndex;
	double factor = 
		WorldHeat::THERMAL_DIFFUSIVITY_COPPER
		* deltaTime
		/ (this->gridScale / 100) // gridScale is in centimetres
		/ (this->gridScale / 100) // gridScale is in centimetres
		;
	static bool first = true;
	if (factor > 0.25 && first) {
		first = false;
		cout << "Factor is " << factor << '\n';
	}
	// to avoid overshooting
	factor = std::min (factor, 0.25);
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
			os << '\t' << this->grid [this->adtIndex][x][y];
		}
	}
	os << '\n';
// #endif
}
