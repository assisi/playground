#ifndef __HEAT_ACTUATOR_H
#define __HEAT_ACTUATOR_H

#include <enki/PhysicalEngine.h>
#include <enki/Geometry.h>

#include "extensions/Component.h"
#include "extensions/PhysicInteraction.h"

namespace Enki
{
	class HeatActuator :
		// public PhysicalObject,
		public PhysicInteraction,
		public Component
	{
	private:
		/**
		 * Heat emitted by this actuator.
		 */
		double heat;
		/**
		 * Is this actuator turned on.
		 */
		bool switchedOn;
		/**
		 * Whether we should recompute the heat distribution in the world.
		 */
		bool recomputeHeatDistribution;
	public:
		HeatActuator (
			Enki::Robot* owner,
			Enki::Vector relativePosition,
			double heat);
		void setHeat (double value);
		bool isSwitchedOn () const
		{
			return this->switchedOn;
		}
		void setSwitchedOn (bool value);
		void toogleSwitchedOn ();
		//! Init at each step
		virtual void init (double dt, PhysicSimulation *w);
		virtual void step (double dt, PhysicSimulation* w);
	};
}

#endif

// Local Variables: 
// mode: c++
// mode: flyspell-prog
// ispell-local-dictionary: "british"
// End: 
