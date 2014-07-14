/* 
 * File:   DecayVibration.h
 * Author: pedro
 *
 * Created on 20 de Maio de 2014, 13:07
 */

#ifndef DECAYVIBRATION_H
#define	DECAYVIBRATION_H

#include "VibrationSource.h"

namespace Enki
{
	/**
	 * A vibration source whose amplitude decays with the square of the
	 * distance.  If the actuator is at the origin, the amplitude equation
	 * is:

	 * <code>A - d^2 * c</code>
	 *
	 * where
	 *
	 * <ul><li><i>A</i> is the maximum amplitude.</li>
	 *
	 * <li><i>d</i> is the distance to the orign.</li>
	 *
	 * <li><i>c</i> is the decaying constant.</li></ul>
	 */
	class DecayVibration:
		public VibrationSource
	{
	private:
		/**
		 * How vibration amplitude decays with the square of the distance.
		 */
		static const double DECAY_CONSTANT;
	public:
		/**
		 * Current vibration amplitude of this source.
		 */
		double amplitude;
	public:
		DecayVibration (double range, Robot* owner, Vector relativePosition, double amplitude);
		DecayVibration (const DecayVibration& orig);
		virtual ~DecayVibration ();

		virtual double getWaveAt (const Point &position, double time) const;

	private:

	};
}
#endif	/* DECAYVIBRATION_H */


// Local Variables: 
// mode: c++
// mode: flyspell-prog
// ispell-local-dictionary: "british"
// End: 
