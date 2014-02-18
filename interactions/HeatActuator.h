#ifndef __HEAT_ACTUATOR_H
#define __HEAT_ACTUATOR_H

#include <enki/PhysicalEngine.h>
#include <enki/Geometry.h>

#include "../Component.h"
#include "../PhysicInteraction.h"

namespace Enki
{
	class HeatActuator :
		// public PhysicalObject,
		public PhysicInteraction,
		public Component
	{
	public:
		/**
		 * Heat emitted by this actuator.
		 */
		double heat;
		bool switchedOn;
	public:
		HeatActuator (
			Enki::Robot* owner,
			Enki::Vector relativePosition,
			double heat);
		//! Init at each step
		virtual void init (double dt, PhysicSimulation *w);
		virtual void step (double dt, PhysicSimulation* w);
		// //! Interact with world
		// virtual void step (double dt, PhysicSimulation *w) { }
	};
}

#endif

// Local Variables: 
// mode: c++
// mode: flyspell-prog
// ispell-local-dictionary: "british"
// End: 
