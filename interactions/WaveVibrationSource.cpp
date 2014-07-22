#include <math.h>

#include "WaveVibrationSource.h"

using namespace Enki;

WaveVibrationSource::WaveVibrationSource
	(double range, Robot* owner,
	 Vector relativePosition,
	 double amplitude,
	 double waveVelocity)
	:
	VibrationSource (range, owner, relativePosition, OMNIDIRECTIONAL),
	amplitude (amplitude),
	waveVelocity (waveVelocity),
	totalElapsedTime (0)
{
}

WaveVibrationSource::WaveVibrationSource (const WaveVibrationSource& orig):
	VibrationSource (orig),
	amplitude (orig.amplitude),
	waveVelocity (orig.waveVelocity),
	totalElapsedTime (0)
{
}

WaveVibrationSource::~WaveVibrationSource()
{
}

double WaveVibrationSource::getWaveAt (const Point &position, double time) const
{
	double distance = (this->absolutePosition - position).norm ();
	double pii = M_PIl;
	return
		this->amplitude
		* std::sin (2 * pii *
			(time - distance / this->waveVelocity));
}
