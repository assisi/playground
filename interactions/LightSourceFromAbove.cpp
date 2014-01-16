/* 
 * File:   LightSourceFromAbove.cpp
 * Author: pedro
 * 
 * Created on 8 de Janeiro de 2014, 15:54
 */

#include "LightSourceFromAbove.h"

using namespace Enki;

LightSourceFromAbove::LightSourceFromAbove (double range, Robot* owner, Vector relativePosition,
	double k, double radius, double peakWavelength, double maxIntensity, double sigma)
	:
	LightSource (range, owner, relativePosition),
	k (k),
	radius (radius),
	peakWavelength (peakWavelength),
	maxIntensity (maxIntensity),
	sigma (sigma)
{
}

LightSourceFromAbove::LightSourceFromAbove (const LightSourceFromAbove& orig):
	LightSourceFromAbove (orig.r, orig.LocalInteraction::owner, orig.relativePosition, orig.k, orig.radius, orig.peakWavelength, orig.maxIntensity, orig.sigma)
{
}

LightSourceFromAbove::~LightSourceFromAbove ()
{
}

double LightSourceFromAbove::
getIntensityAt (const Point& position, double wavelength) const
{
	double distance = (absolutePosition - position).norm ();
	double wavelengthDiff = peakWavelength - wavelength;
	return maxIntensity * tanh (k * distance - radius) * exp (-wavelengthDiff * wavelengthDiff / 2 / sigma / sigma);
}
