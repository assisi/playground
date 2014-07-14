/* 
 * File:   QuadraticVibrationSource.h
 * Author: pedro
 *
 * Created on 19 de Janeiro de 2014, 16:50
 */

#ifndef QUADRATICVIBRATIONSOURCE_H
#define	QUADRATICVIBRATIONSOURCE_H

#include <enki/Geometry.h>

#include "VibrationSource.h"

namespace Enki
{
	/**
	 * A vibration source that approximates the wave equation.  Vibration
	 * amplitude decays quadratic with distance.  It is given by equation:
	 *
	 * <code>A / (c d + 1)</code>
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
	class QuadraticVibrationSource:
		public VibrationSource
	{
		/**
		 * Quadratic coefficient of the vibration model.
		 */
		const double decayConstant;
	public:
		/**
		 * Current vibration amplitude of this source.
		 */
		double amplitude;
		/**
		 * Current vibration frequency of this source.
		 */
		double frequency;
	public:

		QuadraticVibrationSource (double range, Robot* owner, Vector relativePosition, double decayConstant, double amplitude);

		QuadraticVibrationSource (const QuadraticVibrationSource& orig);

		virtual ~QuadraticVibrationSource ();

		virtual double getWaveAt (const Point &position, double time) const;
	private:

	};
}

#endif	/* QUADRATICVIBRATIONSOURCE_H */


// Local Variables: 
// mode: c++
// mode: flyspell-prog
// ispell-local-dictionary: "british"
// End: 
