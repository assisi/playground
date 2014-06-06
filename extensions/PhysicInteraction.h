/* 
 * File:   PhysicInteraction.h
 * Author: pedro
 *
 * Created on 17 de Fevereiro de 2014, 18:10
 */

#ifndef __PHYSIC_INTERACTION_H
#define __PHYSIC_INTERACTION_H

#include <enki/PhysicalEngine.h>

#include "extensions/ExtendedWorld.h"
#include "PhysicSimulation.h"

namespace Enki
{
	class PhysicSimulation;

	class PhysicInteraction
	{
	protected:
		//! The physical object that owns the interaction.
		Robot *owner;
	public:
		PhysicInteraction ()
		{
		}
		PhysicInteraction (Robot* owner) :
			owner (owner)
		{
		}
		PhysicInteraction (const PhysicInteraction& orig) :
			owner (orig.owner)
		{
		}
		virtual ~PhysicInteraction ()
		{
		}
		//! Init at each step
		virtual void init (double dt, PhysicSimulation *w) { }
		//! Interact with world
		virtual void step (double dt, PhysicSimulation *w) { }
		//! Finalize at each step
		virtual void finalize (double dt, PhysicSimulation *w) { }
	private:
		
	};
}

#endif	/* PHYSICINTERACTION_H */

// Local Variables: 
// mode: c++
// mode: flyspell-prog
// ispell-local-dictionary: "british"
// End: 
