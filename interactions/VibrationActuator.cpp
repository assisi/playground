#include "VibrationActuator.h"

using namespace Enki;

VibrationActuator::VibrationActuator (double range, Enki::Robot* owner, Enki::Vector relativePosition, double amplitude, double frequency):
	LocalInteraction (range, owner),
	Component (owner, relativePosition),
	amplitude (amplitude),
	frequency (frequency)
{
}

void VibrationActuator::
writePropertiesCSV (std::ostream &os) const
{
	os
		<< this->relativePosition.x << ',' << this->relativePosition.y;
}

void VibrationActuator::
writeStateCSV (std::ostream &os) const
{
	os
		<< this->absolutePosition.x << ',' << this->absolutePosition.y << ','
		<< this->amplitude << ','
		<< this->frequency;
}

std::ostream & operator << (std::ostream & outs, const VibrationActuator &va)
{
	outs << "vs(#" << va.getAmplitude () << ',' << va.getFrequency () << ')';
	return outs;
}
