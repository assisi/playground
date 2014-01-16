/* 
 * File:   LightSourceFromAbove.h
 * Author: pedro
 *
 * Created on 8 de Janeiro de 2014, 15:54
 */

#ifndef LIGHTSOURCEFROMABOVE_H
#define	LIGHTSOURCEFROMABOVE_H

#include "LightSource.h"

namespace Enki {

	/**
	 * A light source that emits from above and is not obscured by objects
	 * in the world.  The light model is defined in http://larics.rasip.fer.hr/assisiwiki/doku.php?id=software:simulators
	 */
	class LightSourceFromAbove:
		public LightSource
	{
		/**
		 * Shaping coefficient of the sigmoid function.
		 */
		const double k;
		/**
		 * Radius of the illuminated area.
		 */
		const double radius;
		/**
		 * Wave length with maximum intensity.
		 */
		const double peakWavelength;
		/**
		 * Maximum intensity of this light source.
		 */
		const double maxIntensity;
		/**
		 * Shaping coefficient of the Gaussian model.
		 */
		const double sigma;
	public:
		LightSourceFromAbove (double range, Robot* owner, Vector relativePosition,
			double k, double radius, double peakWavelength, double maxIntensity,
			double sigma);
		LightSourceFromAbove (const LightSourceFromAbove& orig);
		virtual ~LightSourceFromAbove ();

		virtual double getIntensityAt (const Point& position, double wavelength) const;
	private:

	};
	
}

#endif	/* LIGHTSOURCEFROMABOVE_H */

