/* 
 * File:   DecayVibration.cpp
 * Author: pedro
 * 
 * Created on 20 de Maio de 2014, 13:07
 */

#include "DecayVibration.h"
#include "NotSimulated.h"

using namespace Enki;

const double DecayVibration::DECAY_CONSTANT = 4;

DecayVibration::DecayVibration (double range, Robot* owner, Vector relativePosition, double a):
	VibrationSource (range, owner, relativePosition, OMNIDIRECTIONAL)
{
}

DecayVibration::DecayVibration (const DecayVibration& orig):
	VibrationSource (orig)
{
}

DecayVibration::~DecayVibration ()
{
}

double DecayVibration::getAmplitudeAt (const Point &position, double time) const
{
	double squareDistance = (this->absolutePosition - position).norm2 ();
	double result = this->amplitude - squareDistance * DecayVibration::DECAY_CONSTANT;
	if (result < 0)
		return 0;
	else
		return result;
}

double DecayVibration::getIntensityAt (const Point &position) const
{
	throw new NotSimulated ();
}
