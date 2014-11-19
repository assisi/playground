
#include "HeatSensor.h"
#include "WorldHeat.h"

using namespace Enki;

HeatSensor::
HeatSensor (Robot* owner, Vector relativePosition,
				double minMeasurableHeat, double maxMeasurableHeat,
				double thermalResponseTime,
				double ambientTemperature
				):
	Component (owner, relativePosition, Component::OMNIDIRECTIONAL), 
	minMeasurableHeat (minMeasurableHeat),
	maxMeasurableHeat (maxMeasurableHeat),
	thermalResponseTime (thermalResponseTime),
	measuredHeat (ambientTemperature)
{
}

HeatSensor::
HeatSensor (Robot* owner, Vector relativePosition,
				double minMeasurableHeat, double maxMeasurableHeat
				):
	Component (owner, relativePosition, Component::OMNIDIRECTIONAL), 
	minMeasurableHeat (minMeasurableHeat),
	maxMeasurableHeat (maxMeasurableHeat),
	thermalResponseTime (-1)
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
		// double factor = std::min (1.0, this->thermalResponseTime * dt);
		// this->measuredHeat =
		// 	factor * worldHeat->getHeatAt (this->absolutePosition)
		// 	+ (1 - factor) * this->measuredHeat;
	}
}
