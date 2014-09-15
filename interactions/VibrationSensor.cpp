#include <algorithm>
#include <iostream>

#include "VibrationSensor.h"
#include "VibrationSource.h"
#include "WaveVibrationSource.h"

using namespace Enki;

VibrationSensor::
VibrationSensor
	(double range, Robot* owner,
	 Vector relativePosition, double orientation,
	 double maxMeasurableFrequency, double amplitudeStandardDeviationGaussianNoise, double frequencyStandardDeviationGaussianNoise)
	:
	LocalInteraction (range, owner),
	Component (owner, relativePosition, orientation), 
	maxMeasurableFrequency (maxMeasurableFrequency),
	amplitudeStandardDeviationGaussianNoise (amplitudeStandardDeviationGaussianNoise),
	frequencyStandardDeviationGaussianNoise (frequencyStandardDeviationGaussianNoise),
	amplitudeValues (),
	frequencyValues (),
	totalElapsedTime (0)
{
}

VibrationSensor::VibrationSensor (const VibrationSensor& orig):
	LocalInteraction (orig.LocalInteraction::r, orig.LocalInteraction::owner),
	Component (orig),
	maxMeasurableFrequency (orig.maxMeasurableFrequency),
	amplitudeStandardDeviationGaussianNoise (orig.amplitudeStandardDeviationGaussianNoise),
	frequencyStandardDeviationGaussianNoise (orig.frequencyStandardDeviationGaussianNoise),
	amplitudeValues (0),
	frequencyValues (0),
	totalElapsedTime (0)
{
}

VibrationSensor::~VibrationSensor ()
{
}

void VibrationSensor::
init (double dt, Enki::World* w)
{
	this->amplitudeValues.clear ();
	this->frequencyValues.clear ();
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

#ifdef __DEBUG__
	Enki::Vector my = this->Component::owner->pos + this->relativePosition;
	Enki::Vector ot = po->pos;
	std::cout
		<< "interaction between "
		<< this->value << '@' << my
		<< " and "
		<< vibrationSource->value << '@' << ot
		<< '\n';
#endif

	double value;
	WaveVibrationSource *waveVibrationSource = dynamic_cast<WaveVibrationSource *>(po);
	if (waveVibrationSource != NULL) {
		value = waveVibrationSource->getFrequency ();
		value = std::min (value, this->maxMeasurableFrequency);
		value = gaussianRand (value, value * this->frequencyStandardDeviationGaussianNoise);
		this->frequencyValues.push_back (value);
		value = waveVibrationSource->getMaximumAmplitude ();
		value = gaussianRand (value, fabs (value * this->amplitudeStandardDeviationGaussianNoise));
		this->amplitudeValues.push_back (value);
	}
}

void VibrationSensor::
finalize (double dt, Enki::World* w)
{
}


