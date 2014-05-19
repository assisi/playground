/* 
 * File:   VibrationActuator.h
 * Author: Pedro Mariano
 *
 * Created on 7 de Janeiro de 2014, 12:14
 */

#ifndef VIBRATION_SOURCE
#define VIBRATION_SOURCE

#include <iostream>

#include <enki/Interaction.h>
#include <enki/PhysicalEngine.h>
#include <enki/Geometry.h>

#include "extensions/Component.h"

namespace Enki
{
	/**
	 * Abstract base class of all vibration sources.  Vibration sources must
	 * implement methods that return vibration wave properties such as
	 * amplitude and frequency.  This can be a class that implements the
	 * wave equation or some approximation.
	 *
	 * @author Pedro Mariano
	 */
	class VibrationSource :
		public LocalInteraction,
		public Component,
		public PhysicalObject
	{
	protected:
		/**
		 */
		VibrationSource (double range, Robot* owner, Vector relativePosition, double orientation);
		/**
		 * Copy constructor.
		 */
		VibrationSource (const VibrationSource &orig);

		virtual ~VibrationSource ();
	public:
		/**
		 * Update absolute position of this component.
		 */
		virtual void init (double dt, Enki::World* w);
		/**
		 * Return the vibration amplitude produced by this source at the
		 * given position and time.
		 */
		virtual double getAmplitudeAt (const Point &position, double time) const = 0;
		/**
		 * Return the vibration intensity produced by this source at the
		 * given position and time.
		 */
		virtual double getIntensityAt (const Point &position) const = 0;
	};
}

#endif /* VIBRATION_SOURCE */

// Local Variables: 
// mode: c++
// mode: flyspell-prog
// ispell-local-dictionary: "british"
// End: 
