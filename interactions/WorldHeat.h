#ifndef __WORLD_HEAT_H
#define __WORLD_HEAT_H

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#include "extensions/ExtendedWorld.h"
#include "extensions/PhysicSimulation.h"
#include "interactions/AbstractGridParallelSimulation.h"
#include "interactions/AbstractGridProperties.h"

namespace Enki
{
	/**
	 * Provides a simulation of heat to be used in Enki.

	 * <p> World heat is represented by a lattice grid.  The size of this
	 * grid depends on world dimensions, scale and border size.  The
	 * temperature in a grid point depends on its current value and the
	 * temperature and heat diffusivity of the four Moore neighbours.  We
	 * also implement heat dissipation, meaning the temperature will tend to
	 * world temperature.

	 * <p> We use two grids.  A {@code AbstractGridSimulation} instance is
	 * used to computed the temperature in the next iteration.  An {@code
	 * AbstractGridProperties} instance is used to store grid properties,
	 * namely heat diffusivity.
	 */
	class WorldHeat :
#ifdef WORLDHEAT_SERIAL
		public AbstractGridSimulation<double>,
#else
		public AbstractGridParallelSimulation<WorldHeat, double>,
#endif
		public AbstractGridProperties<double>
	{
		/**
		 * Value of alpha without the value of parameter {@code deltaTime}.  Alpha
		 * is used in the discrete equation that models heat propagation.
		 * state.
		 */
		const double partialAlpha;
		/**
		 * Output stream where heat information is logged.
		 */
		std::ofstream *logStream;
		/**
		 * Rate at which world heat is logged.  This is used with field
		 * {@code iterationsToNextLog} to produce logs.
		 *
		 * <p> When field {@code iterationsToNextLog} reaches zero, we log
		 * the world heat and reset this field to value {@code logRate}.
		 */
		const int logRate;
		/**
		 * How many iterations should we wait before logging the next
		 * iteration.  This is used with field {@code logRate} to
		 * produce logs.
		 *
		 * <p> When field {@code iterationsToNextLog} reaches zero, we log
		 * the world heat and reset this field to value {@code logRate}.
		 */
		int iterationsToNextLog;
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
		WorldHeat (const ExtendedWorld *world, double normalHeat, double gridScale, double borderSize, double concurrencyLevel, int logRate = 1);
		virtual ~WorldHeat ();
		/**
		 * Checks if this instance of the heat model combined with the given
		 * {@code deltaTime} parameter is valid.  A valid set of parameters does
		 * not cause instability or overshooting in the heat model.

		 * @param deltaTime parameter used in the discrete equation that models
		 * heat propagation.
		 */
		bool validParameters (double deltaTime) const;

		double getHeatAt (const Vector &pos) const;
		void setHeatAt (const Vector &pos, double value);

		double getHeatDiffusivityAt (const Point &position) const;
		void setHeatDiffusivityAt (const Point &position, double value);

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
		 *
		 * @param  deltaTime parameter used in the discrete equation that models
		 * heat propagation.
		 *
		 * @pre validParameters(deltaTime)
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
		/**
		 * Reset temperature to given value.  Heat dissipation is NOT changed.
		 */
		void resetTemperature (double value);
	// protected:
	// 	/**
	// 	 * Update the heat grid and return the largest difference between two
	// 	 * adjacent grid cells.
	// 	 */
	// 	double updateGrid (double deltaTime);
	public:
		/**
		 * Update part of the grid.
		 */
		void updateGrid (double deltaTime, int xmin, int ymin, int xmax, int ymax);
	};
}

#endif

// Local Variables: 
// mode: c++
// mode: flyspell-prog
// ispell-local-dictionary: "british"
// End: 
