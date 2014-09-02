/* 
 * File:   QuadraticVibrationSource.cpp
 * Author: pedro
 * 
 * Created on 19 de Janeiro de 2014, 16:50
 */

#include "QuadraticVibrationSource.h"

using namespace Enki;

QuadraticVibrationSource::QuadraticVibrationSource
	(double range, Robot* owner,
	 Vector relativePosition,
	 double decayConstant,
	 double amplitude)
	:
	VibrationSource (range, owner, relativePosition, OMNIDIRECTIONAL),
	decayConstant (decayConstant),
	amplitude (amplitude)
{
}

QuadraticVibrationSource::QuadraticVibrationSource (const QuadraticVibrationSource& orig):
	VibrationSource (orig),
	decayConstant (orig.decayConstant),
	amplitude (orig.amplitude)
{
}

QuadraticVibrationSource::~QuadraticVibrationSource()
{
}

double QuadraticVibrationSource::getWaveAt (const Point &position, double time) const
{
	double squareDistance = (this->absolutePosition - position).norm2 ();
	return this->amplitude / (this->decayConstant * squareDistance + 1);
}
