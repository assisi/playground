/* 
 * File:   ElectricFieldActuator.cpp
 * Author: pedro
 * 
 * Created on 3 de Março de 2014, 19:05
 */

#include <iostream>

#include "PointElectricFieldActuator.h"

using namespace Enki;

PointElectricFieldActuator::
PointElectricFieldActuator (
	Robot *owner, Vector relativePosition,
	double electricCharge
):
	AbstractElectricFieldActuator (owner, relativePosition, Component::OMNIDIRECTIONAL),
	electricCharge (electricCharge)
{
}

PointElectricFieldActuator::
PointElectricFieldActuator (const PointElectricFieldActuator& orig
):
	AbstractElectricFieldActuator (orig),
	electricCharge (orig.electricCharge)
{
}

PointElectricFieldActuator::~PointElectricFieldActuator ()
{
}

void PointElectricFieldActuator::
init (double dt, World *w)
{
	std::cout << "PointElectricFieldActuator::init()\n";
	Component::init ();
}
		
void PointElectricFieldActuator::
measureAt (const Point &position, double *value, Point *direction)
{
	//	std::cout << "Measure at " << this->absolutePosition << "\n";
	if (this->switchedOn) {
		Point delta = position - this->Component::owner->pos;
		double distance2 = delta.norm2 ();
		double strength = 1.0 / (4.0 * M_PI * AbstractElectricFieldActuator::PERMITTIVITY) * this->electricCharge / distance2;
		if (value != NULL) {
			*value = strength;
		}
		if (direction != NULL) {
			*direction = delta.unitary () * strength;
		}
	}
	else {
		if (value != NULL) {
			*value = 0;
		}
		if (direction != NULL) {
			direction->x = 0;
			direction->y = 0;
		}
	}
}
