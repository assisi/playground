#ifndef __ABSTRACT_GRID_PROPERTIES_H
#define __ABSTRACT_GRID_PROPERTIES_H

#include <vector>
#include <algorithm>
//#include <iostream>
//#include <iomanip>
#include <limits>
#include <boost/foreach.hpp>

#include "extensions/ExtendedWorld.h"
#include "extensions/PhysicSimulation.h"
#include "interactions/AbstractGrid.h"

namespace Enki
{
	/**
	 * Elements in the edge table and in the active-edge table.  These
	 * tables are used by the fill polygon methods.
	 */
	struct Edge {
		int yMax;
		int xCurrent;
		int numerator;
		int denominator;
		int increment;
		short octant;
		Edge *nextElementEdgeTable;
		Edge *nextElementActiveEdgeTable;
	};
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
#define NULL_EDGE NULL
		/**
		 * Edges that can be processed.
		 */
		std::vector<Edge> edges;
		/**
		 * Edge table used by the fill polygon methods.  The array contains
		 * pointers to elements in vector {@code edges}.
		 */
		std::vector<Edge *> edgeTable;
		/**
		 * Active edge table.  This is a pointer to an element in vector
		 * {@code edges}.
		 */
		Edge *activeEdgeTable;
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
			AbstractGrid (NULL, -1, -1)
		{
			this->initData ();
		}
		/**
		 * Construct a new grid.
		 */
		AbstractGridProperties (const ExtendedWorld *world, double gridScale, double borderSize):
			AbstractGrid (world, gridScale, borderSize)
		{
			this->initData ();
		}

		virtual ~AbstractGridProperties () {}

	private:
		/**
		 * Initialise the data after the constructor has calculated the grid
		 * size.
		 *
		 * <p> With C++11 this would be in the most general constructor.
		 */
		void initData ()
		{
			this->prop.resize (this->size.x);
			for (int x = 0; x < this->size.x; x++) {
				this->prop [x].resize (this->size.y);
			}
			this->edgeTable.resize (this->size.y, NULL_EDGE);
			this->edges.resize (this->size.y);
			BOOST_FOREACH (Edge &edge, this->edges) {
				edge.nextElementEdgeTable = NULL;
				edge.nextElementActiveEdgeTable = NULL;
			}
			this->activeEdgeTable = NULL_EDGE;
		}
	public:
		/**
		 * Fill the grid with the given value.
		 *
		 * @param value the value to fill the grid.
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
		 *
		 * @param update the function used to update the grid.
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
		 *
		 * @param update the function used to draw the line.
		 *
		 * @param ax horizontal coordinate of starting point in grid
		 * coordinates.
		 *
		 * @param ay vertical coordinate of starting point in grid
		 * coordinates.
		 *
		 * @param bx horizontal coordinate of final point in grid
		 * coordinates.
		 *
		 * @param by vertical coordinate of final point in grid coordinates.
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
		 *
		 * @param value the value used to fill the upright rectangle.
		 *
		 * @param lowerLeft Coordinate of lower left point in world coordinates.
		 *
		 * @param upperRight Coordinate of upper right point in world coordinates.
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
		 *
		 * @param update the function used to fill the upright rectangle.
		 *
		 * @param lowerLeft Coordinate of lower left point in world coordinates.
		 *
		 * @param upperRight Coordinate of upper right point in world coordinates.
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
		 * @param center Circle center in world coordinates.
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
		/**
		 * Draws and fills a polygon.
		 *
		 * @param value The value used to draw and fill the polygon.
		 *
		 * @param polygon The coordinates of the polygon in world coordinates.
		 */
		void drawPolygon (const T &value, const std::vector<Point> &polygon)
		{
			this->initEdgeTable (polygon);
			int edgeTableSize = polygon.size ();
			int y = 0;
			while (this->edgeTable [y] == NULL_EDGE) {
				y++;
			}
			edgeTableSize -= this->moveToActiveEdgeTable (y);
			this->removeActiveEdgeTable (y);
			while (edgeTableSize > 0 || this->activeEdgeTable != NULL_EDGE) {
				this->fillGridCells (value, y);
				y++;
				this->updateActiveEdgeTable ();
				edgeTableSize -= this->moveToActiveEdgeTable (y);
				this->removeActiveEdgeTable (y);
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
		/**
		 * Initialise the Edge Table with the edges of the given polygon.
		 *
		 * @param polygon
		 */
		inline void initEdgeTable (const std::vector<Point> &polygon)
		{
			const std::vector<Point>::const_iterator end = polygon.end ();
			std::vector<Point>::const_iterator previous = polygon.end ();
			previous--;
			std::vector<Point>::const_iterator current = polygon.begin ();
			std::vector<Edge>::iterator edge = this->edges.begin ();
			while (current != end) {
				int x1, y1, x2, y2;
				this->toIndex (*previous, x1, y1);
				this->toIndex (*current, x2, y2);
				int yMin;
				// calculate edge data
				if (y1 < y2) {
					yMin = y1;
					edge->yMax = y2;
					edge->xCurrent = x1;
					edge->numerator = y2 - y1;
					edge->denominator = x2 - x1;
				}
				else {
					yMin = y2;
					edge->yMax = y1;
					edge->xCurrent = x2;
					edge->numerator = y1 - y2;
					edge->denominator = x1 - x2;
				}
				if (edge->denominator < 0) {
					if (edge->numerator > -edge->denominator) {
						edge->octant = 3;
					}
					else {
						edge->octant = 4;
					}
				}
				else {
					if (edge->numerator > edge->denominator) {
						edge->octant = 2;
					}
					else {
						edge->octant = 1;
					}
				}
				edge->increment = 0;
				edge->nextElementEdgeTable = this->edgeTable [yMin];
				// insert edge in edge-table
				this->edgeTable [yMin] = &(*edge);
				// increment
				edge++;
				previous = current;
				current++;
			}
		}
		/**
		 * Move edges from the edge table to the active-edge table such that
		 * edge yMin is equal to the parameter.
		 *
		 * @return The number of elements that were moved to the AET
		 */
		inline int moveToActiveEdgeTable (int y)
		{
			int result = 0;
			while (this->edgeTable [y] != NULL_EDGE) {
				Edge *tEdge = this->edgeTable [y];
				// insertion sort
				if (this->activeEdgeTable == NULL_EDGE) {
					this->activeEdgeTable = tEdge;
				}
				else if (this->activeEdgeTable->xCurrent > tEdge->xCurrent
							|| (this->activeEdgeTable->xCurrent == tEdge->xCurrent
								 && this->activeEdgeTable->denominator > tEdge->denominator)) {
					tEdge->nextElementActiveEdgeTable = this->activeEdgeTable;
					this->activeEdgeTable = tEdge;
				}
				else {
					Edge *insert = this->activeEdgeTable;
					while (insert->nextElementActiveEdgeTable != NULL_EDGE &&
							 (insert->nextElementActiveEdgeTable->xCurrent < tEdge->xCurrent
							  || (insert->nextElementActiveEdgeTable->xCurrent == tEdge->xCurrent
									&& insert->nextElementActiveEdgeTable->denominator < tEdge->denominator))) {
						insert = insert->nextElementActiveEdgeTable;
					}
					tEdge->nextElementActiveEdgeTable = insert->nextElementActiveEdgeTable;
					insert->nextElementActiveEdgeTable = tEdge;
				}
				// remove from ET
				this->edgeTable [y] = tEdge->nextElementEdgeTable;
				tEdge->nextElementEdgeTable = NULL_EDGE;
				// increment
				result++;
			}
			return result;
		}
		/**
		 * Remove from the Active Edge Table the edges whose yMax field is
		 * equal to the parameter.  The field yMax corresponds to the maximum
		 * vertical coordinate of the edge.
		 *
		 * @param y Edges whose yMax is equal to this parameter are removed.
		 */
		int removeActiveEdgeTable (int y)
		{
			int result = 0;
			Edge *tEdge;
			while (this->activeEdgeTable != NULL_EDGE && this->activeEdgeTable->yMax == y) {
				tEdge = this->activeEdgeTable;
				this->activeEdgeTable = this->activeEdgeTable->nextElementActiveEdgeTable;
				tEdge->nextElementActiveEdgeTable = NULL_EDGE;
				result++;
			}
			tEdge = this->activeEdgeTable;
			if (tEdge != NULL_EDGE) {
				Edge *next = tEdge->nextElementActiveEdgeTable;
				while (next != NULL_EDGE) {
					if (next->yMax == y) {
						tEdge->nextElementActiveEdgeTable = next->nextElementActiveEdgeTable;
						next->nextElementActiveEdgeTable = NULL_EDGE;
						result++;
					}
					else {
						tEdge = next;
					}
					next = tEdge->nextElementActiveEdgeTable;
				}
			}
			return result;
		}
		/**
		 * Fill grid cells using pairs of x coordinates from the Active Edge
		 * Table.
		 *
		 * @param value The value used to draw and fill the polygon.
		 *
		 * @param y vertical coordinate of the grid cells to be filled.
		 */
		inline void fillGridCells (const T &value, int y)
		{
			Edge *index = this->activeEdgeTable;
			do {
				int x0 = index->xCurrent;
				index = index->nextElementActiveEdgeTable;
				int x1 = index->xCurrent;
				index = index->nextElementActiveEdgeTable;
				for (int x = x0; x < x1; x++) {
					this->prop [x][y] = value;
				}
			} while (index != NULL_EDGE);
		}
		/**
		 * Updates field xCurrent of the edges in the Active Edge Table.
		 * This field is incremented using the fraction represented by fields
		 * numerator and denominator.  The update method depends on which
		 * octant the edge lies.
		 */
		inline void updateActiveEdgeTable ()
		{
			Edge *index = this->activeEdgeTable;
			do {
				switch (index->octant) {
				case 1:
					index->increment += index->denominator;
					while (index->increment > index->numerator) {
						index->xCurrent++;
						index->increment -= index->numerator;
					}
					break;
				case 2:
					index->increment += index->denominator;
					if (index->increment > index->numerator) {
						index->xCurrent++;
						index->increment -= index->numerator;
					}
					break;
				case 3:
					index->increment -= index->denominator;
					if (index->increment > 0) {
						index->xCurrent--;
						index->increment -= index->numerator;
					}
					break;
				case 4:
					index->increment -= index->denominator;
					while (index->increment > 0) {
						index->xCurrent--;
						index->increment -= index->numerator;
					}
					break;
				}
				index = index->nextElementActiveEdgeTable;
			} while (index != NULL_EDGE);
		}
#ifdef DEBUG
		void debugAET (int y, int numberEdges)
		{
			Edge *index;
			std::cout << "\n@" << y << " -------------\n";
			for (int y = 0; y < this->size.y; y++) {
				index = this->edgeTable [y];
				if (index != NULL_EDGE) {
					std::cout << "ET y: " << y << " [";
					do {
						std::cout << '\t' << (index - &(this->edges [0]));
						index = index->nextElementEdgeTable;
					} while (index != NULL_EDGE);
					std::cout << "]\n";
				}
			}
			std::cout << "AET ";
			index = this->activeEdgeTable;
			while (index != NULL_EDGE) {
				std::cout << '\t' << (index - &(this->edges [0]));
				index = index->nextElementActiveEdgeTable;
			}
			std::cout << "\nid  yMX xCu num/dem inc oct EAET EET\n";
			for (int idx = 0; idx < numberEdges; idx++) {
				const Edge &edge = this->edges [idx];
				std::cout
					<< '#' << idx << " { "
					<< std::setw (2) << edge.yMax << std::resetiosflags (std::ios_base::adjustfield)
					<< ' '
					<< std::setw (2) << edge.xCurrent << std::resetiosflags (std::ios_base::adjustfield)
					<< "  "
					<< std::setw (3) << edge.numerator << std::resetiosflags (std::ios_base::adjustfield)
					<< '/'
					<< std::setw (3) << edge.denominator << std::resetiosflags (std::ios_base::adjustfield)
					<< ' '
					<< std::setw (3) << edge.increment << std::resetiosflags (std::ios_base::adjustfield)
					<< "  "
					<< edge.octant
					;
				if (edge.nextElementActiveEdgeTable != NULL_EDGE) {
					std::cout
						<< "  ^"
						<< std::setw (2) << (edge.nextElementActiveEdgeTable - &(this->edges [0])) << std::resetiosflags (std::ios_base::adjustfield);
				}
				else {
					std::cout << "  NIL";
				}
				if (edge.nextElementEdgeTable != NULL_EDGE) {
					std::cout
						<< "  ^"
						<< std::setw (2) << (edge.nextElementEdgeTable - &(this->edges [0])) << std::resetiosflags (std::ios_base::adjustfield);
				}
				else {
					std::cout << "  NIL";
				}
				std::cout << "}\n";
			}
		}
#endif
	};
}

#endif

// Local Variables: 
// mode: c++
// mode: flyspell-prog
// ispell-local-dictionary: "british"
// End: 
