/* 
 * File:   ExtendedRobot.h
 * Author: pedro
 *
 * Created on 17 de Fevereiro de 2014, 18:32
 */

#ifndef __EXTENDED_ROBOT_H
#define __EXTENDED_ROBOT_H

#include <enki/PhysicalEngine.h>

#include "PhysicInteraction.h"
#include "PhysicSimulation.h"

namespace Enki
{
	class PhysicInteraction;
	class PhysicSimulation;

	/**
	 * An extended robot that is capable of physical interactions besides
	 * collisions.
	 *
	 * <p> This class provides three methods similar to local and global
	 * interactions that handle physical interactions.  These methods
	 * initialise, perform the actual computation and finish the physical
	 * interaction.  These methods are called in class {@code
	 * ExtendedWorld}.
	 */
	class ExtendedRobot:
		public Robot
	{
	protected:
		/**
		 * Physical interactions that this robot is capable of.
		 */
		std::vector<PhysicInteraction *> physicInteractions;
	public:
		ExtendedRobot ();
		ExtendedRobot (const ExtendedRobot& orig);
		virtual ~ExtendedRobot();

		void addPhysicInteraction (PhysicInteraction *pi)
		{
			this->physicInteractions.push_back (pi);
		}

		virtual void initPhysicInteractions (double dt, PhysicSimulation* ps);
		virtual void doPhysicInteractions (double dt, PhysicSimulation *ps);
		virtual void finalizePhysicInteractions (double dt, PhysicSimulation* ps);
	private:
		
	};
}

#endif	/* EXTENDEDROBOT_H */


// Local Variables: 
// mode: c++
// mode: flyspell-prog
// ispell-local-dictionary: "british"
// End: 
