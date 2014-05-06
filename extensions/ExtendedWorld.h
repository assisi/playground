/* 
 * File:   ExtendedWorld.h
 * Author: pedro
 *
 * Created on 17 de Fevereiro de 2014, 15:13
 */

#include <vector>

#ifndef __EXTENDED_WORLD_H
#define __EXTENDED_WORLD_H

#include <enki/PhysicalEngine.h>

#include "PhysicSimulation.h"
#include "ExtendedRobot.h"

namespace Enki
{
	class ExtendedRobot;
	class PhysicSimulation;
	/**
	 * Extends world class with other physic interactions besides collision
	 * detection.  Robots can also interact with these physic simulations by
	 * means of class {@code PhysicSimulation}.
	 */
	class ExtendedWorld:
		public World
	{
		typedef std::vector<PhysicSimulation *> PhysicSimulations;
		typedef PhysicSimulations::iterator PhysicSimulationsIterator;
		//! Vector of physic simulations.
		PhysicSimulations physicSimulations;

		typedef std::set<ExtendedRobot *> ExtendedRobots;
		typedef ExtendedRobots::iterator ExtendedRobotsIterator;
		
		//! All the extended objects in the world.
		ExtendedRobots extendedRobots;
	public:
		/**
		 * Construct a world with square walls, takes width and height of the
		 * world arena in cm. */
		ExtendedWorld (double width, double height, const Color& wallsColor = Color::gray, unsigned texWidth = 0, unsigned texHeight = 0, const uint32_t* texData = 0);
		/**
		 * Construct a world with circle walls, takes radius of the world
		 * arena in cm.
		 */
		ExtendedWorld (double r, const Color& wallsColor = Color::gray, unsigned texWidth = 0, unsigned texHeight = 0, const uint32_t* texData = 0);
		/**
		 * Construct a world with no walls. */
		ExtendedWorld ();
		virtual ~ExtendedWorld ();
		/**
		 * Add an object to this extended world.  Checks if it is an
		 * extended object.
		 */
		void addObject (PhysicalObject *po);
		/**
		 * Add a physic simulation.
		 */
		void addPhysicSimulation (PhysicSimulation *ps);
		/**
		 * Simulate a timestep of dt. dt should be below 1 (typically
		 * .02-.1); physicsOversampling is the amount of time the physics is
		 * run per step, as usual collisions require a more precise
		 * simulation than the sensor-motor loop frequency. */
		virtual void step (double dt, unsigned physicsOversampling = 1);
	private:

	};
}
#endif	/* EXTENDEDWORLD_H */


// Local Variables: 
// mode: c++
// mode: flyspell-prog
// ispell-local-dictionary: "british"
// End: 
