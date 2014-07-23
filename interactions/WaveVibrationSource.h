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
	public:
		/**
		 * Current vibration amplitude of this source.
		 */
		double maximumAmplitude;
		/**
		 * Wave period.
		 */
		double period;
		/**
		 * Wave speed propagation.
		 */
		double waveVelocity;
		/**
		 * Quadratic constant used to compute amplitude decay.  Zero means no
		 * decay.
		 */
		double amplitudeQuadraticDecay;
	public:

		WaveVibrationSource (double range, Robot* owner, Vector relativePosition, double maximumAmplitude, double period, double waveVelocity, double amplitudeQuadraticDecay);

		WaveVibrationSource (const WaveVibrationSource& orig);

		virtual ~WaveVibrationSource ();

		virtual double getWaveAt (const Point &position, double time) const;
	private:

	};
}

#endif	/* WAVEVIBRATIONSOURCE_H */


// Local Variables: 
// mode: c++
// mode: flyspell-prog
// ispell-local-dictionary: "british"
// End: 
