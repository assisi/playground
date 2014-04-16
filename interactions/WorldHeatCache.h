#ifndef __WORLD_HEAT_H
#define __WORLD_HEAT_H

#include <vector>
#include <iostream>

#include "ExtendedWorld.h"
#include "PhysicSimulation.h"
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
	public:
		typedef enum {CHECKING_STEADY_STATE, USING_CACHED_VALUES, COMPUTE} TimeState;
	private:
		/**
		 * What this instance is doing during a sequence of {@code
		 * computeNextState(double)} calls.  Either we are checking if we
		 * have reached steady state and in method {@code
		 * setHeat(Vector,double)} we only set the corresponding cell heat,
		 * or we use cached values and do not update the values of {@code
		 * heat} matrix and in method {@code setHeat()} we change state if
		 * the difference is higher than {@code HEAT_EPSILON}.
		 */
		TimeState timeState;
		int clock;
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
		 * If two heat values are less than this value, they are considered
		 * equivalent.  This constant is used to see if we have reached the
		 * steady state.
		 */
		static const double HEAT_DIFF_EPSILON;

	public:
		WorldHeat (double normalHeat, double gridScale, double borderSize);
		virtual ~WorldHeat () {}

		double getHeatAt (Vector &pos) const;
		void setHeatAt (Vector &pos, double value);
		TimeState getTimeState () const
		{
			return this->timeState;
		}
		/**
		 * When a heat actuator turns off, we have to recompute the heat
		 * distribution in the world.  We do this for a certain number of
		 * iterations and then we start checking if we have reached a steady
		 * state.
		 */
		void computeHeatDistribution ();


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

	private:
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
