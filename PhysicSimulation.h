/* 
 * File:   PhysicSimulation.h
 * Author: pedro
 *
 * Created on 17 de Fevereiro de 2014, 15:46
 */

#ifndef __PHYSIC_SIMULATION_H
#define __PHYSIC_SIMULATION_H

#include <enki/PhysicalEngine.h>
#include "ExtendedWorld.h"

namespace Enki
{
	class ExtendedWorld;
	/**
	 * Base class of physic simulation.  This allows extending Enki with
	 * other physical simulation besides collision detection.
	 */
	class PhysicSimulation {
	public:
		PhysicSimulation ();
		PhysicSimulation (const PhysicSimulation& orig);
		virtual ~PhysicSimulation ();
		/**
		 * Initialise this physic interaction with the given world.
		 */
		virtual void initParameters (const ExtendedWorld *) = 0;
		/**
		 * Initialise the computation of the next state of this physic
		 * interaction.
		 */
		virtual void initStateComputing (double deltaTime) = 0;
		/**
		 * Computes the next state of this physic interaction.
		 */
		virtual void computeNextState (double deltaTime) = 0;
	private:

	};
}

#endif

// Local Variables: 
// mode: c++
// mode: flyspell-prog
// ispell-local-dictionary: "british"
// End: 
