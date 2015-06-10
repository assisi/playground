#include "AirFlowSensor.h"
#include "AirPump.h"

using namespace Enki;

AirFlowSensor::AirFlowSensor (double range, Enki::Robot* owner, Enki::Vector relativePosition):
	LocalInteraction (range, owner),
	Component (owner, relativePosition, OMNIDIRECTIONAL)
{
}

AirFlowSensor::~AirFlowSensor ()
{
}

void AirFlowSensor::
init (double dt, Enki::World* w)
{
	this->intensity = Vector (0, 0);
	Component::init ();
}

void AirFlowSensor::
objectStep (double dt, Enki::World* w, Enki::PhysicalObject *po)
{
	AirPump *airPump = dynamic_cast<AirPump *>(po);
	// Not an air pump
	if (airPump == NULL) {
		return ;
	}
	// Sensor is suppressed by self actuator
	if (airPump->Component::owner == this->Component::owner) {
		return ;
	}

	this->intensity += airPump->getAirFlowAt (this->absolutePosition);
}
