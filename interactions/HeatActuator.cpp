#include <iostream>

#include "HeatActuator.h"
#include "WorldHeat.h"

using namespace Enki;

HeatActuator::HeatActuator (Robot* owner, Vector relativePosition, double heat):
	PhysicInteraction (owner),
	Component (owner, relativePosition, Component::OMNIDIRECTIONAL), 
	heat (heat),
	switchedOn (true),
	recomputeHeatDistribution (false)
{
	Component::init ();
}


void HeatActuator::
init (double dt, PhysicSimulation *ps)
{
	Component::init ();
    // // Never called???
    // WorldHeat* wh = dynamic_cast<WorldHeat*>(ps);
    // if (wh)
    // {
    //     std::cout << "We got a heat model!" << std::endl;
    // }
    // else
    // {
    //     std::cout << "Corak!" << std::endl;
    // }
}

void HeatActuator::
setHeat (double value)
{
	this->heat = value;
	this->recomputeHeatDistribution = true;
}

void HeatActuator::
setSwitchedOn (bool value)
{
	this->switchedOn = value;
	this->recomputeHeatDistribution = true;
}
void  HeatActuator::
toogleSwitchedOn ()
{
	this->switchedOn = !this->switchedOn;
	this->recomputeHeatDistribution = true;
}

void HeatActuator::
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
