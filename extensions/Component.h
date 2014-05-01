/* 
 * File:   Component.hpp
 * Author: Pedro Mariano
 *
 * Created on 7 de Janeiro de 2014, 12:14
 */
#ifndef __COMPONENT__
#define __COMPONENT__

#include <enki/PhysicalEngine.h>
#include <enki/Interaction.h>

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
		 * Position of this component relative to the centre of mass of the
		 * owner object.
		 */
		const Vector relativePosition;
		/**
		 * Orientation of this component relative to the orientation of the
		 * owner object.
		 */
		const double relativeOrientation;
		/**
		 * Position of this component in world coordinates.  Updated every {@code w->step()}.
		 */
		Point absolutePosition;
		/**
		 * Orientation of this component in world coordinates.  Updated every {@code w->step()}.
		 */
		double absoluteOrientation;
	public:
		/**
		 * Construct a robot component at the given relative position.
		 */
		Component (const PhysicalObject *owner, Vector relativePosition, double relativeOrientation):
			owner(owner),
			relativePosition (relativePosition),
			relativeOrientation (relativeOrientation)
		{
		}
		/**
		 * Copy constructor.
		 */
		Component (const Component& orig):
			owner (orig.owner),
			relativePosition (orig.relativePosition),
			relativeOrientation (orig.relativeOrientation)
			// Component (orig.owner, orig.relativePosition, orig.relativeOrientation) // C++11 feature
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
		 * Get the relative orientation of this robot component.
		 */
		double getRelativeOrientation () const
		{
			return this->relativeOrientation;
		}	
		/**
		 *  Reset absolute position of this component.  Called every {@code w->step()}.
		 */
		void init ()
		{
			Matrix22 rot (this->owner->angle);
			this->absolutePosition = this->owner->pos + rot * this->relativePosition;
			this->absoluteOrientation = this->owner->angle + this->relativeOrientation;
		}
		/**
		 * Get the absolute position of this component.
		 */
		const Point &getAbsolutePosition () const
		{
			return absolutePosition;
		}
		/**
		 * Get the absolute orientation of this component.
		 */
		double getAbsoluteOrientation () const
		{
			return absoluteOrientation;
		}
	};
}

#endif

// Local Variables: 
// mode: c++
// mode: flyspell-prog
// ispell-local-dictionary: "british"
// End: 
