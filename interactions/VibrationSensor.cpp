#include <algorithm>
#include <iostream>

#include "VibrationSensor.h"
#include "VibrationActuator.h"

using namespace Enki;

VibrationSensor::
VibrationSensor (double range, Enki::Robot* owner, Enki::Vector relativePosition, double maxMeasurableAmplitude, double maxMeasurableFrequency):
	LocalInteraction (range, owner),
	Component (owner, relativePosition), 
	maxMeasurableAmplitude (maxMeasurableAmplitude),
	maxMeasurableFrequency (maxMeasurableFrequency),
	amplitude (0),
	frequency (0)
{
}

void VibrationSensor::
init (double dt, Enki::World* w)
{
	this->amplitude = 0;
	this->frequency = 0;
	Component::init ();
	// std::cout << "initialisation step for vibration sensor " << this->value << '\n';
}

void VibrationSensor::
objectStep (double dt, Enki::World* w, Enki::PhysicalObject *po)
{
	VibrationActuator *vibrationActuator = dynamic_cast<VibrationActuator *>(po);
	if (vibrationActuator == NULL) {
		return ;
	}
	if (vibrationActuator->Component::owner == this->Component::owner) {
#ifdef __DEBUG__
		std::cout << "self-inhibit "  << vibrationActuator->value << '\n';
#endif
		return ;
	}

	Enki::Vector my = this->Component::owner->pos + this->relativePosition;
	Enki::Vector ot = po->pos;


#ifdef __DEBUG__
	std::cout
		<< "interaction between "
		<< this->value << '@' << my
		<< " and "
		<< vibrationActuator->value << '@' << ot
		<< '\n';
#endif

	double squareDistance;
	squareDistance = (my - ot).norm2 ();
	this->amplitude += vibrationActuator->amplitude / (1 + squareDistance);
	this->frequency += vibrationActuator->frequency / (1 + squareDistance);
}

void VibrationSensor::
finalize (double dt, Enki::World* w)
{
	this->amplitude = std::min (this->amplitude, this->maxMeasurableAmplitude);
	this->frequency = std::min (this->frequency, this->maxMeasurableFrequency);
}


std::ostream & operator << (std::ostream & outs, const VibrationSensor &vs)
{
	outs << "vs(#" << vs.getAmplitude () << ',' << vs.getFrequency () << ')';
	return outs;
}
