/* 
 * File:   AirPump.h
 * Author: Pedro Mariano
 *
 * Created on April 21, 2015
 */

#include <limits>

#include <boost/math/constants/constants.hpp>

#include "AirPump.h"

const double pi = boost::math::constants::pi<double>();

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
	else {
		double deltangle = delta.angle ();
		double diff = deltangle - this->absoluteOrientation;
		while (diff > pi) {
			diff -= 2 * pi;
		}
		while (diff < -pi) {
			diff += 2 * pi;
		}
		if (fabs (diff) > this->aperture) {
			return Vector (0, 0);
		}
		else {
			// double x = intensity * cos (this->absoluteOrientation);
			// double y = intensity * sin (this->absoluteOrientation);
			double x = intensity * cos (deltangle);
			double y = intensity * sin (deltangle);
			return Vector (x, y);
		}
	}
}
