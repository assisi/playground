/* 
 * File:   WaveVibrationSource.h
 * Author: pedro
 *
 * Created on 14 de Julho de 2014, 18:30
 */

#ifndef WAVEVIBRATIONSOURCE_H
#define	WAVEVIBRATIONSOURCE_H

#include <enki/Geometry.h>

#include "VibrationSource.h"

namespace Enki
{
	/**
	 * A vibration source that approximates the wave equation.  Vibration
	 * amplitude decays quadratic with distance.  It is given by equation:
	 *
	 * <code>A / (1 + c d^2) sin ( 2 pi / T (t - d / v) )</code>
	 *
	 * where
	 *
	 * <ul>
	 *
	 * <li> <i>A</i> is the maximum amplitude; </li>
	 *
	 * <li><i>c</i> is the decaying constant;</li>
	 *
	 * <li><i>d</i> is the distance to the orign.</li>
	 *
	 * </ul>
	 *
	 * <p> The user can control the amplitude.
	 */
	class WaveVibrationSource:
		public VibrationSource
	{
		/**
		 * Wave frequency.
		 */
		double frequency;
		/**
		 * Noise used when setting the frequency.  The real frequency is
		 * added an uniform number from the range [-n,+n].
		 */
		const double noise;
	public:
		/**
		 * Current vibration amplitude of this source.
		 */
		double maximumAmplitude;
		/**
		 * Wave speed propagation.
		 */
		double velocity;
		/**
		 * Wave phase
		 */
		double phase;
		/**
		 * Quadratic constant used to compute amplitude decay.  Zero means no
		 * decay.
		 */
		double amplitudeQuadraticDecay;
	public:

		WaveVibrationSource (double range, Robot* owner, Vector relativePosition,
			double maximumAmplitude, double phase, double velocity,
			double amplitudeQuadraticDecay, double noise);

		WaveVibrationSource (const WaveVibrationSource& orig);

		virtual ~WaveVibrationSource ();

		virtual double getWaveAt (const Point &position, double time) const;

		/**
		 * Sets the frequency of this wave source.  The real frequency
		 * depends on the random noise parameter.
		 */
		void setFrequency (double value);

		double getMaximumAmplitude () const
		{
			return this->maximumAmplitude;
		}

		double getFrequency () const
		{
			return this->frequency;
		}
	private:

	};
}

#endif	/* WAVEVIBRATIONSOURCE_H */


// Local Variables: 
// mode: c++
// mode: flyspell-prog
// ispell-local-dictionary: "british"
// End: 
