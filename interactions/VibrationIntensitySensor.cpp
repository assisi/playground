#include <algorithm>
#include <iostream>

#include "VibrationIntensitySensor.h"
#include "VibrationSource.h"

using namespace Enki;

VibrationIntensitySensor::
VibrationIntensitySensor
	(double range, Robot* owner,
	 Vector relativePosition,
	 double minMeasurableIntensity, double maxMeasurableIntensity, double standardDeviationGaussianNoise)
	:
	LocalInteraction (range, owner),
	Component (owner, relativePosition, Component::OMNIDIRECTIONAL),
	minMeasurableIntensity (minMeasurableIntensity),
	maxMeasurableIntensity (maxMeasurableIntensity),
	standardDeviationGaussianNoise (standardDeviationGaussianNoise),
	intensity (0)
{
}

VibrationIntensitySensor::VibrationIntensitySensor (const VibrationIntensitySensor& orig):
	LocalInteraction (orig.LocalInteraction::r, orig.LocalInteraction::owner),
	Component (orig),
	minMeasurableIntensity (orig.minMeasurableIntensity),
	maxMeasurableIntensity (orig.maxMeasurableIntensity),
	standardDeviationGaussianNoise (orig.standardDeviationGaussianNoise),
	intensity (0)
{
}

VibrationIntensitySensor::~VibrationIntensitySensor ()
{
}

void VibrationIntensitySensor::
init (double dt, Enki::World* w)
{
	this->intensity = fabs (gaussianRand (0, this->standardDeviationGaussianNoise * (this->maxMeasurableIntensity - this->minMeasurableIntensity)));
	Component::init ();
	// std::cout << "initialisation step for vibration sensor " << this->value << '\n';
}

void VibrationIntensitySensor::
objectStep (double dt, Enki::World* w, Enki::PhysicalObject *po)
{
	VibrationSource *vibrationSource = dynamic_cast<VibrationSource *>(po);
	if (vibrationSource == NULL) {
		return ;
	}
	if (vibrationSource->Component::owner == this->Component::owner) {
		return ;
	}

	Enki::Vector my = this->Component::owner->pos + this->relativePosition;
	Enki::Vector ot = po->pos;


	double value;
	value = vibrationSource->getIntensityAt (this->absolutePosition);
	if ((value >= this->minMeasurableIntensity)
		 && (value <= this->maxMeasurableIntensity)) {
		this->intensity += value;
	}

	// double value;
	// value = vibrationSource->getIntensityAt (this->absolutePosition);
	// if ((value >= this->minMeasurableIntensity)
	// 	 && (value <= this->maxMeasurableIntensity)) {
	// 	value = gaussianRand (value, fabs (value * this->standardDeviationGaussianNoise));
	// 	this->intensity += value;
	// }
}
