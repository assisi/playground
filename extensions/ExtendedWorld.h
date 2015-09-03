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
#include <boost/timer/timer.hpp>

#include "PhysicSimulation.h"
#include "ExtendedRobot.h"

namespace Enki
{
	class ExtendedRobot;
	class PhysicSimulation;
	class WorldHeat;
	/**
	 * Extends world class with other physic interactions besides collision
	 * detection.  Robots can also interact with these physic simulations by
	 * means of class {@code PhysicSimulation}.
	 */
	class ExtendedWorld:
		public World
	{
	private:
		/**
		 * Rate at which we check skewness between wall clock time and
		 * simulation time.
		 */
		const double SKEW_MONITOR_RATE;
		/**
		 * How skew must be present so that we report it.  The message is
		 * printed in the standard error stream.
		 */
		const double SKEW_REPORT_THRESHOLD;
		/**
		 * Elapsed time since the last check on skewness between wall clock
		 * and simulation time.
		 */
		double simulatedElapsedTime;
		/**
		 * Timer used to monitor used to measure simulation skewness.
		 */
		boost::timer::cpu_timer skewTimer;
	public:
		typedef std::vector<PhysicSimulation *> PhysicSimulations;
		typedef PhysicSimulations::iterator PhysicSimulationsIterator;
		//! Vector of physic simulations.
		PhysicSimulations physicSimulations;
		/**
		 * Current heat model used in the world.
		 */
		WorldHeat *worldHeat;

	protected:
		typedef std::set<ExtendedRobot *> ExtendedRobots;
		typedef ExtendedRobots::iterator ExtendedRobotsIterator;
		
		//! All the extended objects in the world.
		ExtendedRobots extendedRobots;
		/**
		 * Holds the total absolute time which is a sum of all values of
		 * parameter {@code dt} of method {@code step()}.
		 */
		double absoluteTime;
	public:
		/**
		 * Construct a world with square walls, takes width and height of the
		 * world arena in cm.
		 *
		 * @param skewMonitorRate Rate at which the skew monitor checks the
		 * simulated time against wall time.  Unit is second.  By default the
		 * rate is 1 minute.
		 *
		 * @param skewReportThreshold How much skew must be present in order
		 * to print a message.  By default is 5%.
		 */
		ExtendedWorld (double width, double height, const Color& wallsColor = Color::gray, const World::GroundTexture& groundTexture = World::GroundTexture(), unsigned int skewMonitorRate = 60, double skewReportThreshold = 0.05);
		/**
		 * Construct a world with circle walls, takes radius of the world
		 * arena in cm.
		 *
		 * @param skewMonitorRate Rate at which the skew monitor checks the
		 * simulated time against wall time.  Unit is second.  By default the
		 * rate is 1 minute.
		 *
		 * @param skewReportThreshold How much skew must be present in order
		 * to print a message.  By default is 5%.
		 */
		ExtendedWorld (double r, const Color& wallsColor = Color::gray, const World::GroundTexture& groundTexture = World::GroundTexture(), unsigned int skewMonitorRate = 60, double skewReportThreshold = 0.05);
		/**
		 * Construct a world with no walls.
		 *
		 * @param skewMonitorRate Rate at which the skew monitor checks the
		 * simulated time against wall time.  Unit is second.  By default the
		 * rate is 1 minute.
		 *
		 * @param skewReportThreshold How much skew must be present in order
		 * to print a message.  By default is 5%.
		 */
		ExtendedWorld (unsigned int skewMonitorRate = 60, double skewReportThreshold = 0.05);
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

		/**
		 * Return the vibration amplitude sensed at the given position and
		 * time.
		 */
		virtual double getVibrationAmplitudeAt (const Point &position, double time) const;

		/**
		 * Return the air flow intensity at the given position.
		 */
		virtual double getAirFlowIntensityAt (const Point &position) const;
		// /**
		//  * Return the vibration intensity sensed at the given position and
		//  * time.
		//  */
		// virtual double getVibrationIntensityAt (const Point &position) const;

		double getAbsoluteTime () const
		{
			return this->absoluteTime;
		}
	private:

	};
}
#endif	/* EXTENDEDWORLD_H */


// Local Variables: 
// mode: c++
// mode: flyspell-prog
// ispell-local-dictionary: "british"
// End: 
