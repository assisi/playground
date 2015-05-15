#ifndef __ABSTRACT_GRID_PROPERTIES_H
#define __ABSTRACT_GRID_PROPERTIES_H

#include <vector>
#include <iostream>
#include <limits>

#include "extensions/ExtendedWorld.h"
#include "extensions/PhysicSimulation.h"

namespace Enki
{
	/**
	 * Extends {@code AbstractGrid} with a grid that represents constant
	 * properties.  These properties are not updated by a difference
	 * equation.  Use class {@code AbstractGridSimulation} for physical
	 * properties that are update by a difference equation.

	 * <p> This class provides methods to fill grid cells with specific
	 * values.  These methods are based on computer graphics raster
	 * techniques.  They can be used to simulate moving objects.
	 */
	template<class T>
	class AbstractGridProperties :
		public virtual AbstractGrid
	{
	public:
		/**
		 * Function used to update grid cells.
		 */
		typedef void (*UpdateFunction) (T &value); 

	protected:
		/**
		 * Grid with the physical properties.
		 */
		std::vector<std::vector<T> > prop;
		/**
		 * This constructor should be used by a class that inherit multiple
		 * times class {@code AbstractGrid}.
		 */
		AbstractGridProperties ():
			AbstractGrid (-1, -1)
		{
		}
		/**
		 * Construct a new grid.
		 */
		AbstractGridProperties (double gridScale, double borderSize):
			AbstractGrid (gridScale, borderSize)
		{
		}

		virtual ~AbstractGridProperties () {}

	public:
		/**
		 * Initialise this physic interaction with the given world.

		 * <p> {@code AbstractGrid::initParameters(const ExtendedWorld *)} is
		 * not called by this method as a class may inherit multiple times
		 * through classes {@code AbstractGridProperties} and {@code
		 * AbstractGridSimulation}.
		 */
		virtual void initParameters (const ExtendedWorld *world)
		{
			this->prop.resize (this->size.x);
			for (int x = 0; x < this->size.x; x++) {
				this->prop [x].resize (this->size.y);
			}
		}
		/**
		 * Fill the grid with the given value.
		 */
		void fillGrid (T value)
		{
			for (int x = this->size.x - 1; x >= 0; x--) {
				for (int y = this->size.y - 1; y >= 0; y--) {
					this->prop [x][y] = value;
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
					(*update) (this->prop [x][y]);
				}
			}
		}
		/**
		 * Update the grid cells that form a line between points (ax,ay) and
		 * (bx,by).  This function uses the raster line drawing algorithm.
		 */
		void drawLine (UpdateFunction update, int ax, int ay, int bx, int by)
		{
			if (ax == bx) {
				int y;
				for (y = ay; y <= by; y++) {
					(*update) (this->prop [ax][y]);
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
					(*update) (this->prop [x][y]);
					error += deltaError;
					while (error >= 0.5) {
						(*update) (this->prop [x][y]);
						y += sy;
						error -= 1.0;
					}
				}
			}
		}
		/**
		 * Update the grid cells that form an upright rectangle between lower left
		 * point (ax,ay) and upper right point (bx,by).  This function uses the
		 * raster rectangle drawing algorithm.
		 */
		void drawUprightRectangle (const T &value, const Point &lowerLeft, const Point &upperRight)
		{
			int ax, ay, bx, by;
			toIndex (lowerLeft, ax, ay);
			toIndex (upperRight, bx, by);
			int x, y;
			for (x = ax; x <= bx; x++) {
				for (y = ay; y <= by; y++) {
					this->prop [x][y] = value;
				}
			}
		}
		/**
		 * Update the grid cells that form an upright rectangle between lower left
		 * point (ax,ay) and upper right point (bx,by).  This function uses the
		 * raster rectangle drawing algorithm.
		 */
		void drawUprightRectangle (UpdateFunction update, const Point &lowerLeft, const Point &upperRight)
		{
			int ax, ay, bx, by;
			toIndex (lowerLeft, ax, ay);
			toIndex (upperRight, bx, by);
			int x, y;
			for (x = ax; x <= bx; x++) {
				for (y = ay; y <= by; y++) {
					(*update) (this->prop [x][y]);
				}
			}
		}
		/**
		 * Draws and fills a circle.  We use a version of the Bresenham
		 * (1977) algorithm for drawing the circumference.
		 *
		 * @param update The function used to update grid cells.
		 *
		 * @þaram center Circle center in world coordinates.
		 *
		 * @param worldRadius Circle radius in world coordinates.
		 */
		void drawCircle (const T &value, const Point &center, double worldRadius)
		{
			int cx, cy, radius;
			int x, y, d, deltaE, deltaSE;
			toIndex (center, cx, cy);
			radius = this->scale (worldRadius);
			x = 0;
			y = radius;
			d = 1 - radius;
			deltaE = 3;
			deltaSE = 5 - radius * 2;
			circlePoints (value, cx, cy, x, y);
			while (y > x) {
				if (d < 0) {
					d += deltaE;
					deltaE += 2;
					deltaSE += 3;
					x++;
				}
				else {
					d += (x - y) << 1 + 5;
					deltaE += 2;
					deltaSE += 4;
					x++;
					y--;
				}
				circlePoints (value, cx, cy, x, y);
			}
		}
		/**
		 * Draws and fills a circle.  We use a version of the Bresenham
		 * (1977) algorithm for drawing the circumference.
		 *
		 * @param update The function used to update grid cells.
		 *
		 * @þaram center Circle center in world coordinates.
		 *
		 * @param worldRadius Circle radius in world coordinates.
		 */
		void drawCircle (UpdateFunction update, const Point &center, double worldRadius)
		{
			int cx, cy, radius;
			int x, y, d, deltaE, deltaSE;
			toIndex (center, cx, cy);
			radius = this->scale (worldRadius);
			x = 0;
			y = radius;
			d = 1 - radius;
			deltaE = 3;
			deltaSE = 5 - radius * 2;
			circlePoints (update, cx, cy, x, y);
			while (y > x) {
				if (d < 0) {
					d += deltaE;
					deltaE += 2;
					deltaSE += 3;
					x++;
				}
				else {
					d += (x - y) << 1 + 5;
					deltaE += 2;
					deltaSE += 4;
					x++;
					y--;
				}
				circlePoints (update, cx, cy, x, y);
			}
		}
	private:
		/**
		 * Draw circle points and do some filling.  Points are drawn taking
		 * advantage of eight-way symmetry.  Since method {@code
		 * drawCircles()} always increments the x coordinate, filling is done
		 * by drawing vertical lines.

		 * <p> TODO: draw vertical lines in the 1, 4, 5 and 8 octant.
		 */
		inline void circlePoints (const T &value, int cx, int cy, int x, int y)
		{
			for (int iy = cy - y, ly = cy + y; iy <= ly; iy++) {
				this->prop [cx + x][iy] = value;
				this->prop [cx - x][iy] = value;
			}
			if (x != y) {
				this->prop [cx + y][cy + x] = value;
				this->prop [cx + y][cy - x] = value;
				this->prop [cx - y][cy + x] = value;
				this->prop [cx - y][cy - x] = value;
			}
		}
		/**
		 * Draw circle points and do some filling.  Points are drawn taking
		 * advantage of eight-way symmetry.  Since method {@code
		 * drawCircles()} always increments the x coordinate, filling is done
		 * by drawing vertical lines.

		 * <p> TODO: draw vertical lines in the 1, 4, 5 and 8 octant.
		 */
		inline void circlePoints (UpdateFunction update, int cx, int cy, int x, int y)
		{
			for (int iy = cy - y, ly = cy + y; iy <= ly; iy++) {
				(*update) (this->prop [cx + x][iy]);
				(*update) (this->prop [cx - x][iy]);
			}
			if (x != y) {
				(*update) (this->prop [cx + y][cy + x]);
				(*update) (this->prop [cx + y][cy - x]);
				(*update) (this->prop [cx - y][cy + x]);
				(*update) (this->prop [cx - y][cy - x]);
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
