#include "HeatActuator.h"
#include "WorldHeat.h"

using namespace Enki;

HeatActuator::HeatActuator (Robot* owner, Vector relativePosition, double heat):
	PhysicInteraction (owner),
	Component (owner, relativePosition, Component::OMNIDIRECTIONAL), 
	heat (heat),
	switchedOn (true)
{
	Component::init ();
}


void HeatActuator::
init (double dt, PhysicSimulation *ps)
{
	Component::init ();
}

void HeatActuator::
step (double dt, PhysicSimulation *ps)
{
	WorldHeat *worldHeat = dynamic_cast<WorldHeat *> (ps);
	if (worldHeat != NULL) {
		if (this->switchedOn) {
			worldHeat->setHeatAt (this->absolutePosition, this->heat);
		}
	}
}
