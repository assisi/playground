
#include "WaveVibrationSource.h"

using namespace Enki;

WaveVibrationSource::WaveVibrationSource
	(double range, Robot* owner,
	 Vector relativePosition,
	 double amplitude,
	 double frequency)
	:
	VibrationSource (range, owner, relativePosition, OMNIDIRECTIONAL),
	amplitude (amplitude),
	frequency (frequency),
	totalElapsedTime (0)
{
}

WaveVibrationSource::WaveVibrationSource (const WaveVibrationSource& orig):
	VibrationSource (orig),
	amplitude (orig.amplitude),
	frequency (orig.frequency),
	totalElapsedTime (0)
{
}

WaveVibrationSource::~WaveVibrationSource()
{
}

double WaveVibrationSource::getWaveAt (const Point &position, double time) const
{
	double distance = (this->absolutePosition - position).norm ();
	this->totalElapsedTime += time;
	return this->amplitude * sin (distance * this->totalElapsedTime * this->frequency);
}
