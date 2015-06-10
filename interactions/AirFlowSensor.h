/* 
 * File:   AirFlowSensor.h
 * Author: Pedro Mariano
 *
 * Created on April 21, 2015
 */

#ifndef __AIR_FLOW_SENSOR__
#define __AIR_FLOW_SENSOR__

#include <enki/Interaction.h>
#include "Component.h"

namespace Enki {

	/**
	 * Simple implementation of an air flow sensor.  We add the intensity of all
	 * air pumps within the radius of this sensor and the radius of the air pump.
	 */
	class AirFlowSensor:
		public LocalInteraction,
		public Component
	{
	public:
		/**
		 * Air flow intensity measured by this sensor.
		 */
		Vector intensity;
	public:
		/**
		 * @param range Maximum distance an air pump can be in order to sense
		 * the generated air flow.

		 * @param owner The robot that this air flow sensor belongs to.

		 * @param relativePosition The position of this air flow sensor relative
		 * to robot origin.
		 */
		AirFlowSensor (double range, Enki::Robot* owner, Enki::Vector relativePosition);
		/**
		 * Destructor
		 */
		virtual ~AirFlowSensor ();
		/**
		 *  Reset air flow intensity.  Called every {@code w->step()}.
		 */
		virtual void init (double dt, Enki::World* w);
		/**
		 * Interact with the given object only if it is an {@code AirPump} object.
		 */
		virtual void objectStep (double dt, Enki::World* w, Enki::PhysicalObject *po);
	};

}

#endif

// Local Variables: 
// mode: c++
// mode: flyspell-prog
// ispell-local-dictionary: "british"
// End: 
