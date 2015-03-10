#ifndef __WORLD_HEAT_H
#define __WORLD_HEAT_H

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#include "extensions/ExtendedWorld.h"
#include "extensions/PhysicSimulation.h"
#include "interactions/AbstractGridSimulation.h"

namespace Enki
{
	/**
	 * Provides a simulation of heat to be used in Enki.

	 * <p> World heat is represented by a lattice grid.  The size of this
	 * grid depends on world dimensions, scale and border size.  The heat in
	 * a grid point depends on its current value and the value of the four
	 * Moore neighbours.
	 */
	class WorldHeat :
		public AbstractGridSimulation<double>
	{
		/**
		 * Maximum admissible delta time when computing the next state.
		 */
		const double maxDeltaTime;
		/**
		 * Output stream where heat information is logged.
		 */
		std::ofstream *logStream;
		/**
		 * How much time has passed since the simulation started.  Unit is
		 * simulation time.
		 */
		double relativeTime;
	public:
		/**
		 * Normal environmental heat used to compute heat at world borders.
		 */
		const double normalHeat;
		/**
		 * Heat in enki diffuses through air.
		 */
		static const double THERMAL_DIFFUSIVITY_AIR;
		/**
		 * Heat in enki can diffuse through copper connections between CASUs.
		 */
		static const double THERMAL_DIFFUSIVITY_COPPER;

	public:
		WorldHeat (double normalHeat, double gridScale, double borderSize);
		virtual ~WorldHeat ();

		double getHeatAt (const Vector &pos) const;
		void setHeatAt (const Vector &pos, double value);
		/**
		 * When a heat actuator turns off, we have to recompute the heat
		 * distribution in the world.  We do this for a certain number of
		 * iterations and then we start checking if we have reached a steady
		 * state.
		 */
		virtual void computeHeatDistribution () {}
		/**
		 * Initialise this physic interaction with the given world.
		 */
		virtual void initParameters (const ExtendedWorld *);
		/**
		 * Initialise the computation of the next state of this physic
		 * interaction.
		 */
		virtual void initStateComputing (double deltaTime);
		// /**
		//  * Handles the action of the given physical object in this physic
		//  * interaction.
		//  */
		// virtual void handleObjectAction (const PhysicalObject *po);
		/**
		 * Computes the next state of this physic interaction.
		 */
		virtual void computeNextState (double deltaTime);
		// /**
		//  * Updates the physical sensors of the given object.
		//  */
		// virtual void handleObjectSense (PhysicalObject *po);

		void dumpState (std::ostream &os);

		/**
		 * Turn on heat log.  The heat grid will be written to the given output
		 * stream.
		 */
		void logToStream (std::string fileName)
		{
			if (this->logStream != NULL) {
				this->logStream->flush ();
				delete this->logStream;
			}
			this->logStream = new std::ofstream (
				fileName.c_str (),
				std::ofstream::out | std::ofstream::trunc);
		}

		void turnOffLog ()
		{
			if (this->logStream != NULL) {
				// this->logStream->close ();
				delete this->logStream;
			}
			this->logStream = NULL;
		}

	protected:
		/**
		 * Update the heat grid and return the largest difference between two
		 * adjacent grid cells.
		 */
		double updateGrid (double deltaTime);
	};
}

#endif

// Local Variables: 
// mode: c++
// mode: flyspell-prog
// ispell-local-dictionary: "british"
// End: 
