/* 
 * File:   Component.hpp
 * Author: Pedro Mariano
 *
 * Created on 7 de Janeiro de 2014, 12:14
 */
#ifndef __COMPONENT__
#define __COMPONENT__

#include "PhysicalEngine.h"
#include "Interaction.h"

namespace Enki
{
	/**
	 * Represents a component of some robot or object.  Provides common
	 * functionality for sensors and actuators that are part of some robot.
	 * These components' position is relative to the robot absolute
	 * position.  The component absolute position is computed every
	 * simulation step.
	 */
	class Component
	{
	public:
		/**
		 * The owner of this component.
		 */
		const PhysicalObject *owner;
		/**
		 * Position of this component relative to the centre of mass of the object.
		 */
		const Vector relativePosition;
		/**
		 * Position of this component in world coordinates.  Updated every {@code w->step()}.
		 */
		Vector absolutePosition;
	public:
		/**
		 * Construct a robot component at the given relative position.
		 */
		Component (const PhysicalObject *owner, Vector relativePosition):
			owner(owner),
			relativePosition (relativePosition)
		{
		}
		Component (const Component& orig):
			Component (orig.owner, orig.relativePosition)
		{
		}
		/**
		 * Get the relative position of this robot component.
		 */
		const Vector &getRelativePosition () const
		{
			return this->relativePosition;
		}
		/**
		 *  Reset absolute position of this component.  Called every {@code w->step()}.
		 */
		void init ()
		{
			Matrix22 rot (this->owner->angle);
			this->absolutePosition = this->owner->pos + rot * this->relativePosition;
		}
		/**
		 * Get the absolute position of this component.
		 */
		Point getAbsolutePosition () { return absolutePosition; }
	};
}

#endif
