#include <algorithm>
#include <iostream>

#include "VibrationSensor.h"
#include "VibrationSource.h"

using namespace Enki;

VibrationSensor::
VibrationSensor
	(double range, Robot* owner,
	 Vector relativePosition, double orientation,
	 double maxMeasurableAmplitude, double maxMeasurableFrequency, double amplitudeStandardDeviationGaussianNoise, double frequencyStandardDeviationGaussianNoise)
	:
	LocalInteraction (range, owner),
	Component (owner, relativePosition, orientation), 
	maxMeasurableAmplitude (maxMeasurableAmplitude),
	maxMeasurableFrequency (maxMeasurableFrequency),
	amplitudeStandardDeviationGaussianNoise (amplitudeStandardDeviationGaussianNoise),
	frequencyStandardDeviationGaussianNoise (frequencyStandardDeviationGaussianNoise),
	amplitude (0),
	frequency (0)
{
}

VibrationSensor::VibrationSensor (const VibrationSensor& orig):
	LocalInteraction (orig.LocalInteraction::r, orig.LocalInteraction::owner),
	Component (orig),
	maxMeasurableAmplitude (orig.maxMeasurableAmplitude),
	maxMeasurableFrequency (orig.maxMeasurableFrequency),
	amplitudeStandardDeviationGaussianNoise (orig.amplitudeStandardDeviationGaussianNoise),
	frequencyStandardDeviationGaussianNoise (orig.frequencyStandardDeviationGaussianNoise),
	amplitude (0),
	frequency (0)
{
}

VibrationSensor::~VibrationSensor ()
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
	VibrationSource *vibrationSource = dynamic_cast<VibrationSource *>(po);
	if (vibrationSource == NULL) {
		return ;
	}
	if (vibrationSource->Component::owner == this->Component::owner) {
#ifdef __DEBUG__
		std::cout << "self-inhibit "  << vibrationSource->value << '\n';
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
		<< vibrationSource->value << '@' << ot
		<< '\n';
#endif

	double value;
	value = vibrationSource->getAmplitudeAt (this->absolutePosition, dt);
	value = gaussianRand (value, fabs (value * this->amplitudeStandardDeviationGaussianNoise));
	this->amplitude += value;
	value = vibrationSource->getFrequencyAt (this->absolutePosition, dt);
	value = gaussianRand (value, value * this->frequencyStandardDeviationGaussianNoise);
	this->frequency += value;
}

void VibrationSensor::
finalize (double dt, Enki::World* w)
{
	// this->amplitude = gaussianRand (this->amplitude, this->amplitudeStandardDeviationGaussianNoise);
	this->amplitude = std::max (-this->maxMeasurableAmplitude, std::min (this->amplitude, this->maxMeasurableAmplitude));
	// this->frequency = gaussianRand (this->frequency, this->frequencyStandardDeviationGaussianNoise);
	this->frequency = std::max (0.0, std::min (this->frequency, this->maxMeasurableFrequency));
}


