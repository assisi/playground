#ifndef __ABSTRACT_GRID_SIMULATION_H
#define __ABSTRACT_GRID_SIMULATION_H

#include <vector>
#include <iostream>
#include <limits>

#include "extensions/ExtendedWorld.h"
#include "extensions/PhysicSimulation.h"

namespace Enki
{
	/**
	 * Abstract class of physical models based in a grid.  The grid
	 * coordinate system depends on world walls type.  If the walls are
	 * circular, the centre of world is at (0,0).  If the walls are square
	 * world centre is at (w/2,h/2) where (w,h) is the square dimension.  If
	 * there are no walls surrounding the world, we find the objects
	 * farthest apart in the world at initialisation time.

	 * <p> Note that in the absence of world walls if objects can move
	 * behind the grid coordinates, care must be taken to return a valid
	 * physical quantity or physical quantity.
	 */
	template<class T>
	class AbstractGridSimulation :
		public PhysicSimulation
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
		 * Grid with the physical quantity.
		 */
		std::vector<std::vector<T> > grid [2];
		/**
		 * Index of the current grid in field {@code grid}.
		 */
		int adtIndex;
	protected:
		AbstractGridSimulation (double gridScale, double borderSize):
			gridScale (gridScale),
			borderSize (borderSize),
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
			Enki::Vector min, max;

			switch (world->wallsType) {
			case World::WALLS_SQUARE:
				min = Point (0, 0);
				max = Point (world->w, world->h);
				break;
			case World::WALLS_CIRCULAR:
				min = Point (-world->r, -world->r);
				max = Point (world->r, world->r);
				break;
			case World::WALLS_NONE: {
				min = Vector (std::numeric_limits<double>::max (), std::numeric_limits<double>::max ());
				max = Vector (std::numeric_limits<double>::min (), std::numeric_limits<double>::min ());
				Enki::World::ObjectsIterator iterator = world->objects.begin ();
				while (iterator != world->objects.end ()) {
					Enki::PhysicalObject* po = *iterator;
					min.x = std::min (min.x, po->pos.x);
					min.y = std::min (min.y, po->pos.y);
					max.x = std::max (max.x, po->pos.x);
					max.y = std::max (max.y, po->pos.y);
					iterator++;
				}
				break;
			}
			default:
				throw 0; //new string ("AbstractGridSimulation::initParameters(const ExtendedWorld*): Unhandled wall type");
			}

			min.x -= borderSize + gridScale;
			min.y -= borderSize + gridScale;
			max.x += borderSize + gridScale;
			max.y += borderSize + gridScale;
			this->size = (max - min) / gridScale;
			this->size.x = ceil (this->size.x);
			this->size.y = ceil (this->size.y);
			this->grid [0].resize (this->size.x);
			this->grid [1].resize (this->size.x);
			for (int x = 0; x < this->size.x; x++) {
				this->grid [0][x].resize (this->size.y);
				this->grid [1][x].resize (this->size.y);
			}
			this->origin = min;
#ifndef DEBUG
			std::cout
				<< "Abstract Grid Simulation\nmin: " << min
				<< "\nmax: " << max
				<< "\nsize: " << size
				<< "\ngrid scale: " << gridScale << std::endl;
#endif
		}

	protected:
		void toIndex (const Enki::Vector& position, int &x, int &y) const
		{
			x = round ((position.x - this->origin.x) / this->gridScale);
			y = round ((position.y - this->origin.y) / this->gridScale);
#ifdef DEBUG
			std::cout << position << " <=> " << x << "," << y << " => " << value << '\n';
#endif
		}
	};
}

#endif

// Local Variables: 
// mode: c++
// mode: flyspell-prog
// ispell-local-dictionary: "british"
// End: 
