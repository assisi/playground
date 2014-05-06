
#include "HeatSensor.h"
#include "WorldHeat.h"

using namespace Enki;

HeatSensor::
HeatSensor (Robot* owner, Vector relativePosition, double minMeasurableHeat, double maxMeasurableHeat):
	Component (owner, relativePosition, Component::OMNIDIRECTIONAL), 
	minMeasurableHeat (minMeasurableHeat),
	maxMeasurableHeat (maxMeasurableHeat),
	measuredHeat (0)
{
}

void HeatSensor::
init (double dt, PhysicSimulation* ps)
{
	Component::init ();
}

void HeatSensor::
step (double dt, PhysicSimulation* ps)
{
	WorldHeat *worldHeat = dynamic_cast<WorldHeat *> (ps);
	if (worldHeat != NULL) {
		this->measuredHeat = worldHeat->getHeatAt (this->absolutePosition);
	}
}
