#ifndef __ABSTRACT_GRID_SIMULATION_H
#define __ABSTRACT_GRID_SIMULATION_H

#include <vector>
#include <iostream>
#include <limits>

#include "extensions/ExtendedWorld.h"
#include "extensions/PhysicSimulation.h"

namespace Enki
{

	// extern const int MAX_NUMBER_EDGES;
	struct Edge
	{
		int ymax;
		int xxmin;
		double m1;
	};

	template<class T>
	class AbstractGridSimulation;

	template <class T>
	void fillPolygone (AbstractGridSimulation<T> *sim, const T &value, const std::vector<Point> &points);


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
		std::vector<std::vector<Edge> > edgeTable;
		std::vector<std::vector<Edge> > activeEdgeTable;
		
	public:
		/**
		 * Function used to update grid cells.
		 */
		typedef void (*UpdateFunction) (const T &value); 
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
		/**
		 * Fill the grid using the given function.
		 */
		void fillGrid (UpdateFunction update)
		{
			for (int x = this->size.x - 1; x >= 0; x--) {
				for (int y = this->size.y - 1; y >= 0; y--) {
					(*update) (this->grid [this->adtIndex][x][y]);
				}
			}
		}
		void fillLine (UpdateFunction update, int ax, int ay, int bx, int by)
		{
			if (ax == bx) {
				int y;
				for (y = ay; y <= by; y++) {
					(*update) (this->grid [this->adtIndex][ax][y]);
				}
			}
			else {
				int mx = bx - ax;
				int my = by - ay;
				double deltaError = ((double) my) / mx;
				double error = 0;
				int x, y;
				int sy = (by > ay ? 1 : -1);
				y = ay;
				for (x = ax; x <= bx; x++) {
					(*update) (this->grid [this->adtIndex][x][y]);
					error += deltaError;
					while (error >= 0.5) {
						(*update) (this->grid [this->adtIndex][x][y]);
						y += sy;
						error -= 1.0;
					}
				}
			}
		}
		/*
		void fillPolygone (UpdateFunction update, std::vector<Point> points)
		{
			int x, y;
			int ymin, ymax;
			int i;
			vector<double> intersections (points.size ());
			vector<bool> use (points.size ());
			double t;
			const int ps1 = points.size () - 1;
			// compute minimum and maximum y coordinates
			ymin = ymax = round (points [0].y);
			for (i = ps1; i > 0; i--) {
				y = points [i].y;
				ymin = std::min (ymin, y);
				ymax = std::max (ymax, y);
			}
			for (y = ymin; y <= ymax; y++) {
				// find intersections
				intersect (points [0].x, points [0].y, points [ps1].x, points [ps1].x, y, intersections [0], use [0]);
				for (i = ps1; i > 0; i--) {
					intersect (points [i].x, points [i].y, points [i - 1].x, points [i - 1].x, y, intersections [i], use [i]);
				}
				// sort intersections by x 
			}
		}
		*/
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
