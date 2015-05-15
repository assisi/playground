#ifndef __ABSTRACT_GRID_SIMULATION_H
#define __ABSTRACT_GRID_SIMULATION_H

#include <vector>
#include <iostream>
#include <limits>

#include "extensions/ExtendedWorld.h"
#include "extensions/PhysicSimulation.h"

#include "AbstractGrid.h"

namespace Enki
{
	/**
	 * Extends {@code AbstractGrid} with a grid that represents modifiable
	 * properties.  These properties are updated by a difference equation.
	 * The next value of a grid cell can depend on the current value and on
	 * neighbouring cells.  This property means that the update can be
	 * parallelised.
	 */
	template<class T>
	class AbstractGridSimulation :
		public virtual AbstractGrid
	{
	protected:
		/**
		 * Grid with the physical quantity.
		 */
		std::vector<std::vector<T> > grid [2];
		/**
		 * Index of the current grid in field {@code grid}.
		 */
		int adtIndex;

		/**
		 * This constructor should be used by a class that inherit multiple
		 * times class {@code AbstractGrid}.
		 */
		AbstractGridSimulation ():
			AbstractGrid (-1, -1),
			adtIndex (0)
		{
		}
		/**
		 * Construct a new grid.
		 */
		AbstractGridSimulation (double gridScale, double borderSize):
			AbstractGrid (gridScale, borderSize),
			adtIndex (0)
		{
		}

		virtual ~AbstractGridSimulation () {}

	public:
		/**
		 * Initialise this physic interaction with the given world.
		 */
		virtual void initParameters (const ExtendedWorld *world)
		{
			this->grid [0].resize (this->size.x);
			this->grid [1].resize (this->size.x);
			for (int x = 0; x < this->size.x; x++) {
				this->grid [0][x].resize (this->size.y);
				this->grid [1][x].resize (this->size.y);
			}
		}
		/**
		 * Fill the grid with the given value.
		 */
		void fillGrid (T value)
		{
			for (int x = this->size.x - 1; x >= 0; x--) {
				for (int y = this->size.y - 1; y >= 0; y--) {
					this->grid [this->adtIndex][x][y] = value;
				}
			}
		}
	};
}

#endif

// Local Variables: 
// mode: c++
// mode: flyspell-prog
// ispell-local-dictionary: "british"
// End: 
