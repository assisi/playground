#include <iostream>

#include "HeatActuatorPointSource.h"

using namespace Enki;

HeatActuatorPointSource::HeatActuatorPointSource
	(Robot* owner, Vector relativePosition,
	 double thermalResponseTime,
	 double ambientTemperature
	 ):
	PhysicInteraction (owner),
	Component (owner, relativePosition, Component::OMNIDIRECTIONAL), 
	heat (ambientTemperature),
	thermalResponseTime (thermalResponseTime),
	switchedOn (true),
	recomputeHeatDistribution (false)
{
	Component::init ();
}


void HeatActuatorPointSource::
init (double dt, PhysicSimulation *ps)
{
	Component::init ();
}

void HeatActuatorPointSource::
setHeat (double value)
{
	this->heat = value;
	this->recomputeHeatDistribution = true;
}

void HeatActuatorPointSource::
setSwitchedOn (bool value)
{
	this->switchedOn = value;
	this->recomputeHeatDistribution = true;
}
void  HeatActuatorPointSource::
toogleSwitchedOn ()
{
	this->switchedOn = !this->switchedOn;
	this->recomputeHeatDistribution = true;
}

void HeatActuatorPointSource::
step (double dt, PhysicSimulation *ps)
{
	WorldHeat *worldHeat = dynamic_cast<WorldHeat *> (ps);
	if (worldHeat != NULL) {
		if (this->switchedOn) {
			worldHeat->setHeatAt (this->absolutePosition, this->getRealHeat (dt, worldHeat));
		}
		if (this->recomputeHeatDistribution) {
			this->recomputeHeatDistribution = false;
			worldHeat->computeHeatDistribution ();
			std::cout << "Forcing computation of steady state\n";
		}
	}
}
