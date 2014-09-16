#include <boost/math/constants/constants.hpp>
#include <math.h>

#include <enki/Random.h>

#include "WaveVibrationSource.h"

using namespace Enki;

WaveVibrationSource::WaveVibrationSource
	(double range, Robot* owner,
	 Vector relativePosition,
	 double maximumAmplitude,
	 double phase,
	 double velocity,
	 double amplitudeQuadraticDecay,
	 double noise)
	:
	VibrationSource (range, owner, relativePosition, OMNIDIRECTIONAL),
	noise (noise),
	frequency (0),
	maximumAmplitude (maximumAmplitude),
	velocity (velocity),
	frequency (0),
	phase (phase),
	amplitudeQuadraticDecay (amplitudeQuadraticDecay)
{
}

WaveVibrationSource::WaveVibrationSource
	(double range, Robot* owner,
	 Vector relativePosition,
	 double maximumAmplitude,
	 double phase,
	 double velocity,
	 double amplitudeQuadraticDecay,
	 double noise)
	:
	VibrationSource (range, owner, relativePosition, OMNIDIRECTIONAL),
	noise (noise),
	frequency (0),
	maximumAmplitude (maximumAmplitude),
	velocity (velocity),
	phase (phase),
	amplitudeQuadraticDecay (amplitudeQuadraticDecay)
{
}

WaveVibrationSource::WaveVibrationSource (const WaveVibrationSource& orig):
	VibrationSource (orig),
	frequency (orig.frequency),
	noise (orig.noise),
	maximumAmplitude (orig.maximumAmplitude),
	velocity (orig.velocity),
	phase (orig.phase),
	amplitudeQuadraticDecay (orig.amplitudeQuadraticDecay)
{
}

WaveVibrationSource::~WaveVibrationSource()
{
}

void WaveVibrationSource::
setFrequency (double value)
{
	// std::cout << "setFrequency (" << value << ")" << std::endl;
	this->frequency = value + (2 * uniformRand () - 1) / 2 * this->noise;
}

double WaveVibrationSource::getWaveAt (const Point &position, double time) const
{
	double distance2 = (this->absolutePosition - position).norm2 ();
	double distance = sqrt (distance2);
	return
		this->maximumAmplitude
		* std::sin (
			2 * boost::math::constants::pi<double> ()
			* this->frequency
			* (time + distance / this->velocity + this->phase)
		)
		/ (1 + distance2 * this->amplitudeQuadraticDecay);
}
