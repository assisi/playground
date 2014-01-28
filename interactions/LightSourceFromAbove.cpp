/* 
 * File:   LightSourceFromAbove.cpp
 * Author: pedro
 * 
 * Created on 8 de Janeiro de 2014, 15:54
 */

#include "LightSourceFromAbove.h"

using namespace Enki;

LightSourceFromAbove::LightSourceFromAbove
	(double range, Robot* owner,
	 Vector relativePosition, double orientation,
	 double k, double radius, double peakWavelength, double maxIntensity, double sigma)
	:
	LightSource (range, owner, relativePosition, orientation),
	k (k),
	radius (radius),
	peakWavelength (peakWavelength),
	maxIntensity (maxIntensity),
	sigma (sigma)
{
}

LightSourceFromAbove::LightSourceFromAbove (const LightSourceFromAbove& orig):
	LightSource (orig),
	k (orig.k),
	radius (orig.radius),
	peakWavelength (orig.peakWavelength),
	maxIntensity (orig.maxIntensity),
	sigma (orig.sigma)
	// C11 feature
	// LightSourceFromAbove (orig.r, orig.LocalInteraction::owner,
	// 	orig.relativePosition,
	// 	orig.k, orig.radius, orig.peakWavelength, orig.maxIntensity, orig.sigma)
{
}

LightSourceFromAbove::~LightSourceFromAbove ()
{
}

void LightSourceFromAbove::
init (double dt, Enki::World* w)
{
	Component::init ();
}

double LightSourceFromAbove::
getIntensityAt (const Point& position, double wavelength) const
{
    double distance = (absolutePosition - position).norm ();
    double wavelengthDiff = peakWavelength - wavelength;
    return maxIntensity
        * (-tanh (k * distance - radius) + 1) / 2
        * exp (-wavelengthDiff * wavelengthDiff / 2 / sigma / sigma);
}
