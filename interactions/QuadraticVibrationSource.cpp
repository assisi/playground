/* 
 * File:   QuadraticVibrationSource.cpp
 * Author: pedro
 * 
 * Created on 19 de Janeiro de 2014, 16:50
 */

#include "QuadraticVibrationSource.h"

#include "NotSimulated.h"

using namespace Enki;

QuadraticVibrationSource::QuadraticVibrationSource
	(double range, Robot* owner,
	 Vector relativePosition,
	 double a)
	:
	VibrationSource (range, owner, relativePosition, OMNIDIRECTIONAL),
	a (a),
	amplitude (0),
	frequency (0)
{
}

QuadraticVibrationSource::QuadraticVibrationSource (const QuadraticVibrationSource& orig):
	VibrationSource (orig),
	a (orig.a),
	amplitude (orig.amplitude),
	frequency (orig.frequency)
{
}

QuadraticVibrationSource::~QuadraticVibrationSource()
{
}

double QuadraticVibrationSource::getAmplitudeAt (const Point &position, double time) const
{
	throw new NotSimulated ();
}

double QuadraticVibrationSource::getIntensityAt (const Point &position) const
{
	double squareDistance = (this->absolutePosition - position).norm2 ();
	return this->frequency / (this->a * squareDistance + 1);
}
