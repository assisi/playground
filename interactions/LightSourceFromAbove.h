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
	 * in the world.  The light model is defined in
	 * http://larics.rasip.fer.hr/assisiwiki/doku.php?id=software:simulators
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
		 * Current maximum intensity of this light source.
		 */
		double maxIntensity;
		/**
		 * Shaping coefficient of the Gaussian model.
		 */
		const double sigma;
	public:
		/**
		 * Construct a light source from above with the given parameters.
		 */
		LightSourceFromAbove (double range, Robot* owner, Vector relativePosition, double orientation,
			double k, double radius, double peakWavelength, double maxIntensity,
			double sigma);
		/**
		 * Copy constructor.
		 */
		LightSourceFromAbove (const LightSourceFromAbove& orig);

		virtual ~LightSourceFromAbove ();

		/**
		 * Update absolute position of this component.
		 */
		virtual void init (double dt, Enki::World* w);

		virtual double getIntensityAt (const Point& position, double wavelength) const;

        //! Turn the light source on.
        /*!

         */
        virtual void on( double intensity )
        {
            maxIntensity = intensity;
        }

        //! Turn the light source off.
        /*!

         */
        virtual void off( void )
        {
            maxIntensity = 0;
        }
        
	private:

	};
	
}

#endif	/* LIGHTSOURCEFROMABOVE_H */

