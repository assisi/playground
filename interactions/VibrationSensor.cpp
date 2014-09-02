#include <algorithm>
#include <iostream>

#include "VibrationSensor.h"
#include "VibrationSource.h"

using namespace Enki;

VibrationSensor::
VibrationSensor
	(double range, Robot* owner,
	 Vector relativePosition, double orientation,
	 double maxMeasurableAmplitude, double maxMeasurableIntensity, double amplitudeStandardDeviationGaussianNoise, double intensityStandardDeviationGaussianNoise)
	:
	LocalInteraction (range, owner),
	Component (owner, relativePosition, orientation), 
	maxMeasurableAmplitude (maxMeasurableAmplitude),
	maxMeasurableIntensity (maxMeasurableIntensity),
	amplitudeStandardDeviationGaussianNoise (amplitudeStandardDeviationGaussianNoise),
	intensityStandardDeviationGaussianNoise (intensityStandardDeviationGaussianNoise),
	amplitude (0),
	intensity (0),
	totalElapsedTime (0)
{
}

VibrationSensor::VibrationSensor (const VibrationSensor& orig):
	LocalInteraction (orig.LocalInteraction::r, orig.LocalInteraction::owner),
	Component (orig),
	maxMeasurableAmplitude (orig.maxMeasurableAmplitude),
	maxMeasurableIntensity (orig.maxMeasurableIntensity),
	amplitudeStandardDeviationGaussianNoise (orig.amplitudeStandardDeviationGaussianNoise),
	intensityStandardDeviationGaussianNoise (orig.intensityStandardDeviationGaussianNoise),
	amplitude (0),
	intensity (0),
	totalElapsedTime (0)
{
}

VibrationSensor::~VibrationSensor ()
{
}

void VibrationSensor::
init (double dt, Enki::World* w)
{
	this->amplitude = 0;
	this->intensity = 0;
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
	value = vibrationSource->getIntensityAt (this->absolutePosition);
	value = gaussianRand (value, value * this->intensityStandardDeviationGaussianNoise);
	this->intensity += value;
}

void VibrationSensor::
finalize (double dt, Enki::World* w)
{
	// this->amplitude = gaussianRand (this->amplitude, this->amplitudeStandardDeviationGaussianNoise);
	this->amplitude = std::max (-this->maxMeasurableAmplitude, std::min (this->amplitude, this->maxMeasurableAmplitude));
	// this->frequency = gaussianRand (this->frequency, this->frequencyStandardDeviationGaussianNoise);
	this->intensity = std::max (0.0, std::min (this->intensity, this->maxMeasurableIntensity));
}


