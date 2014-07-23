#include <boost/math/constants/constants.hpp>
#include <math.h>

#include "WaveVibrationSource.h"

using namespace Enki;

WaveVibrationSource::WaveVibrationSource
	(double range, Robot* owner,
	 Vector relativePosition,
	 double maximumAmplitude,
	 double period,
	 double waveVelocity,
	 double amplitudeQuadraticDecay)
	:
	VibrationSource (range, owner, relativePosition, OMNIDIRECTIONAL),
	maximumAmplitude (maximumAmplitude),
	period (period),
	waveVelocity (waveVelocity),
	amplitudeQuadraticDecay (amplitudeQuadraticDecay)
{
}

WaveVibrationSource::WaveVibrationSource (const WaveVibrationSource& orig):
	VibrationSource (orig),
	maximumAmplitude (orig.maximumAmplitude),
	period (orig.period),
	waveVelocity (orig.waveVelocity),
	amplitudeQuadraticDecay (orig.amplitudeQuadraticDecay)
{
}

WaveVibrationSource::~WaveVibrationSource()
{
}

double WaveVibrationSource::getWaveAt (const Point &position, double time) const
{
	double distance2 = (this->absolutePosition - position).norm2 ();
	double distance = sqrt (distance2);
	return
		this->maximumAmplitude
		* std::sin (
			2 * boost::math::constants::pi<double> ()
			* (time - distance / this->waveVelocity)
			/ this->period)
		/ (1 + distance2 * this->amplitudeQuadraticDecay);
}
