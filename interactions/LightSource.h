/* 
 * File:   LightSource.h
 * Author: Pedro Mariano
 *
 * Created on 7 de Janeiro de 2014, 12:14
 */

#ifndef __LIGHTSOURCE_HPP
#define	__LIGHTSOURCE_HPP

#include "../Interaction.h"
#include "../Component.h"

namespace Enki {

	/**
	 * Abstract base class of all light sources.  Light sources must
	 * implement a method that return the wavelength intensity at some
	 * global position.  Light sensors use this method to perceive the
	 * wavelength intensity measured at their positions.
	 */
	class LightSource:
		public LocalInteraction,
		public Component
	{
	protected:
		LightSource (double range, Robot* owner, Vector relativePosition);
	public:
		/**
		 * Get the wavelength intensity of this light source at the given
		 * position.
		 */
		virtual double getIntensityAt (const Point& position, double wavelength) const = 0;
	};
}

#endif	/* LIGHTSOURCE_HPP */

