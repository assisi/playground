#include <iostream>

#include "HeatActuatorPointSource.h"
#include "WorldHeat.h"

using namespace Enki;

HeatActuatorPointSource::HeatActuatorPointSource (Robot* owner, Vector relativePosition, double heat):
	PhysicInteraction (owner),
	Component (owner, relativePosition, Component::OMNIDIRECTIONAL), 
	heat (heat),
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
			// std::cout << "Heating " << this->heat << std::endl;
			worldHeat->setHeatAt (this->absolutePosition, this->heat);
		}
		if (this->recomputeHeatDistribution) {
			this->recomputeHeatDistribution = false;
			worldHeat->computeHeatDistribution ();
			std::cout << "Forcing computation of steady state\n";
		}
	}
}
