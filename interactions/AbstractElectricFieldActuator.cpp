/* 
 * File:   AbstractElectricFieldActuator.cpp
 * Author: pedro
 * 
 * Created on 3 de Março de 2014, 19:09
 */

#include "AbstractElectricFieldActuator.h"

using namespace Enki;

double AbstractElectricFieldActuator::PERMITTIVITY = 8.8541878176E-12;

AbstractElectricFieldActuator::
AbstractElectricFieldActuator (
	Robot *owner,
	Vector relativePosition,
	double relativeOrientation
):
	Component (owner, relativePosition, relativeOrientation),
	GlobalInteraction (GlobalInteraction::owner),
	switchedOn (true)
{
}

AbstractElectricFieldActuator::
AbstractElectricFieldActuator (
	const AbstractElectricFieldActuator& orig
):
	Component (orig),
	GlobalInteraction (orig.GlobalInteraction::owner),
	switchedOn (orig.switchedOn)
{
}

AbstractElectricFieldActuator::
~AbstractElectricFieldActuator ()
{
}

void AbstractElectricFieldActuator::
toogle ()
{
	this->switchedOn = !this->switchedOn;
}