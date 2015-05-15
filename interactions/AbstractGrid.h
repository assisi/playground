#ifndef __ABSTRACT_GRID_H
#define __ABSTRACT_GRID_H

#include <iostream>

#include "extensions/ExtendedWorld.h"

namespace Enki
{
	/**
	 * Abstract class of physical models based in a grid.  The grid
	 * coordinate system depends on world walls type.  If the walls are
	 * circular, the centre of world is at (0,0).  If the walls are square
	 * world centre is at (w/2,h/2) where (w,h) is the square dimension.  If
	 * there are no walls surrounding the world, we find the objects
	 * farthest apart in the world at initialisation time.

	 * <p> This class does not contain the grid data but only its
	 * characteristics.  Classes {@code AbstractGridSimulation} and {@code
	 * AbstractGridProperties} contain the grid data.

	 * <p> Note that in the absence of world walls if objects can move
	 * behind the grid coordinates, care must be taken to return a valid
	 * physical quantity or physical quantity.
	 */
	class AbstractGrid:
		public virtual PhysicSimulation
	{
	public:
		/**
		 * Length in world coordinates of a grid point.
		 */
		const double gridScale;
		/**
		 * Grid size.
		 *
		 * <p> Should be constant but has to be initialised in method {@code
		 * initParameters(const ExtendedWorld*)}.
		 */
		Enki::Vector size;
		/**
		 * Smallest world coordinates of cell (0,0) in the grid.
		 *
		 * <p> Should be constant but has to be initialised in method {@code
		 * initParameters(const ExtendedWorld*)}.
		 */
		Enki::Vector origin;

	protected:
		/**
		 * Distance between world limits and environment.
		 */
		const double borderSize;
		/**
		 * @param gridScale
		 * @param borderSize
		 */
		AbstractGrid (double gridScale, double borderSize):
			gridScale (gridScale),
			borderSize (borderSize)
		{
		}
		virtual ~AbstractGrid () {}

	public:
		/**
		 * Initialise grid fields with the given world.
		 */
		virtual void initParameters (const ExtendedWorld *world);

	protected:
		/**
		 * Convert a world position to grid position.
		 */
		void toIndex (const Enki::Vector& position, int &x, int &y) const
		{
			x = round ((position.x - this->origin.x) / this->gridScale);
			y = round ((position.y - this->origin.y) / this->gridScale);
		}
		/**
		 * Scale a world value to grid index.
		 */
		int scale (double value)
		{
			return round (value / this->gridScale);
		}
	};
}

#endif

// Local Variables: 
// mode: c++
// mode: flyspell-prog
// ispell-local-dictionary: "british"
// End: 
