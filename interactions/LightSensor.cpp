/* 
 * File:   LightSensor.cpp
 * Author: Pedro Mariano
 * 
 * Created on 7 de Janeiro de 2014, 16:41
 */

#include "LightSensor.h"
#include "LightSource.h"

using namespace Enki;

LightSensor::LightSensor (double range, Enki::Robot* owner, Enki::Vector relativePosition, double wavelength):
	LocalInteraction (range, owner),
	Component (owner, relativePosition),
	wavelength (wavelength)
{
}

LightSensor::LightSensor (const LightSensor& orig):
	LocalInteraction (orig.r, orig.LocalInteraction::owner),
	Component (orig),
	wavelength (orig.wavelength)
{
}

LightSensor::~LightSensor ()
{
}

void LightSensor::
init (double dt, Enki::World* w)
{
	this->intensity = 0;
	Component::init ();
}

void LightSensor::
objectStep (double dt, Enki::World* w, Enki::PhysicalObject *po)
{
	LightSource *lightSource = dynamic_cast<LightSource *>(po);
	if (lightSource == NULL) {
		return ;
	}
	if (lightSource->Component::owner == this->Component::owner) {
		return ;
	}
	this->intensity += lightSource->getIntensityAt (this->absolutePosition, this->wavelength);
}
