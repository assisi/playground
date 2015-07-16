/* 
 * File:   AirPump.h
 * Author: Pedro Mariano
 *
 * Created on April 21, 2015
 */

#include <limits>

#include <stdio.h>

#include "AirPump.h"

using namespace Enki;

AirPump::AirPump (double range, Robot* owner, Vector relativePosition, double orientation, double aperture, double intensity):
	LocalInteraction (range, owner),
	Component (owner, relativePosition, orientation),
	aperture (aperture),
	intensity (intensity)
{
}

AirPump::~AirPump ()
{
}

void AirPump::
init (double dt, Enki::World* w)
{
	Component::init ();
	printf ("AirPump::  %10p  position %3f %3f\n", this->Component::owner, this->absolutePosition.x, this->absolutePosition.y);
}

Vector AirPump::getAirFlowAt (const Point &position) const
{
	Vector delta = position - this->absolutePosition;
	if (this->intensity < std::numeric_limits<double>::epsilon ()) {
		return Vector (0, 0);
	}
	else if (delta.norm2 () > this->LocalInteraction::r * this->LocalInteraction::r) {
		return Vector (0, 0);
	}
	else if (fabs (delta.angle () - this->absoluteOrientation) > this->aperture) {
		return Vector (0, 0);
	}
	else {
		double x = intensity * cos (this->absoluteOrientation);
		double y = intensity * sin (this->absoluteOrientation);
		return Vector (x, y);
	}
}
