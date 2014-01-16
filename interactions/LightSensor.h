/* 
 * File:   LightSensor.h
 * Author: Pedro Mariano
 *
 * Created on 7 de Janeiro de 2014, 16:41
 */

#ifndef __LIGHTSENSOR_H
#define __LIGHTSENSOR_H

#include "../Interaction.h"
#include "../Component.h"

namespace Enki
{
	/**
	 * A light sensor that reacts maximally to a single wavelength.

	 * <p> Light sensors only interact with {@code LightSource} objects.  We
	 * measure the {@code LightSource}'s intensity at the {@code
	 * LightSensor}'s position and added to attribute {@code intensity}.
	 * This attribute is initialised in method {@code
	 * init(double,Enki::World*)} called at every simulation step.
	 */
	class LightSensor :
		public LocalInteraction,
		public Component
	{
	public:
		/**
		 * Wavelength that this light sensor reacts maximally.
		 */
		const double wavelength;
	public:
		LightSensor (double range, Enki::Robot* owner, Enki::Vector relativePosition, double wavelength);
		LightSensor (const LightSensor& orig);
		virtual ~LightSensor ();
		/**
		 *  Reset light intensity.  Called every {@code w->step()}.
		 */
		virtual void init (double dt, Enki::World* w);
		/**
		 * Interact with the given object only if it is a {@code LightSource}
		 * object.
		 */
		virtual void objectStep (double dt, Enki::World* w, Enki::PhysicalObject *po);
		/**
		 * Get the light intensity measured by this light sensor.
		 */
		double getIntensity () const { return this->intensity; }
	private:
		/**
		 * Light intensity measured by this light sensor.
		 */
		double intensity;
	};
}

#endif	/* __LIGHTSENSOR_H */

