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
	 * amplitude and frequency decays quadratic with distance.
	 *
	 * <p> The user can control the amplitude and frequency.
	 */
	class QuadraticVibrationSource:
		public VibrationSource
	{
		/**
		 * Quadratic coefficient of the vibration model.
		 */
		const double a;
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

		QuadraticVibrationSource (double range, Robot* owner, Vector relativePosition, double a);

		QuadraticVibrationSource (const QuadraticVibrationSource& orig);

		virtual ~QuadraticVibrationSource ();

		virtual double getAmplitudeAt (const Point &position, double time) const;

		virtual double getIntensityAt (const Point &position) const;
	private:

	};
}

#endif	/* QUADRATICVIBRATIONSOURCE_H */


// Local Variables: 
// mode: c++
// mode: flyspell-prog
// ispell-local-dictionary: "british"
// End: 
