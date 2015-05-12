#include <algorithm>
#include <limits>

#include "AbstractGridSimulation.h"

using namespace std;
using namespace Enki;

struct Edge1
{
	double x;
	int index;
};

// const int MAX_NUMBER_EDGES = 10;

template<class T>
bool almost_equal (const T x, const T y, int ulp = 10)
{
	return
		// the machine epsilon has to be scaled to the magnitude of the values used
		// and multiplied by the desired precision in ULPs (units in the last place)
		std::abs (x - y) < std::numeric_limits<T>::epsilon () * std::abs (x + y) * ulp
		// unless the result is subnormal
		|| std::abs (x - y) < std::numeric_limits<T>::min ();
}

/**
 * Compute the intersection of the horizontal line with y coordinate
 * with the line between points A (ax,ay) and B (bx,by).
 */
void intersect (double ax, double ay, double bx, double by, double y, double &x, bool &yes)
{
	double dy = by - ay;
	if (abs (dy) <= numeric_limits<double>::epsilon ()) {
		yes = false;
	}
	else {
		double t = (y - by) / dy;
		if (t < 0.0 || t > 1.0) {
			yes = false;
				}
		else {
			yes = true;
			x = ax + (bx - ax) * t;
		}
	}
}

/**
 * Compute the intersection of the horizontal line with y coordinate
 * with the line between points A (ax,ay) and B (bx,by).
 */
void intersect (double ax, double ay, double bx, double by, double y, int index, vector<Edge1> intersections)
{
	if (!almost_equal (by, ay)) {
		double t = (y - by) / (by - ay);
		if (t >= 0.0 && t <= 1.0) {
			int s = intersections.size ();
			intersections [s].x = ax + (bx - ax) * t;
			intersections [s].index = index;
		}
	}
}

template <class T>
//void leftEdgeScan (AbstractGridSimulation<T> grid, AbstractGridSimulation<T>::UpdateFunction updateFunction, const Point &a, const Point &b)
void leftEdgeScan (AbstractGridSimulation<T> *sim, T value, const Point &a, const Point &b)
{
	int xmin, ymin, xmax, ymax;
	xmin = round (min (a.x, b.x));
	xmax = round (max (a.x, b.x));
	ymin = round (min (a.y, b.y));
	ymax = round (max (a.y, b.y));
	int x, y, numerator, denominator, increment;
	x = xmin;
	numerator = xmax - xmin;
	denominator = ymax - ymin;
	increment = denominator;
	for (y = ymin; y <= ymax; y++) {
		sim->grid [sim->adtIndex][x][y] = value;
		increment += numerator;
		if (increment > numerator) {
			x++;
			increment -= denominator;
		}
	}
}


template <class T>
//void scanLineAlgorithm (AbstractGridSimulation<T> grid, AbstractGridSimulation<T>::UpdateFunction updateFunction, vector<Point> points)
void scanLineAlgorithm (AbstractGridSimulation<T> *sim, T value, const vector<Point> &points)
{
}

bool edgeSorter (const Edge1 &e1, const Edge1 &e2)
{
	return
		(e1.x < e2.x)
		|| (e1.x == e2.x && (e1.index < e2.index));
}

template <class T>
void fillPolygone (AbstractGridSimulation<T> *sim, const T &value, const vector<Point> &points)
{
	int x, y;
	int ymin, ymax;
	int i;
	vector<Edge1> intersections;
	intersections.reserve (points.size ());
	const int ps1 = points.size () - 1;
	// compute minimum and maximum y coordinates
	ymin = ymax = round (points [0].y);
	for (i = ps1; i > 0; i--) {
		y = points [i].y;
		ymin = std::min (ymin, y);
		ymax = std::max (ymax, y);
	}
	for (y = ymin; y <= ymax; y++) {
		intersections.resize (0);
		// find intersections
		intersect (points [0].x, points [0].y, points [ps1].x, points [ps1].x, y, 0, intersections);
		for (i = ps1; i > 0; i--) {
			intersect (points [i].x, points [i].y, points [i - 1].x, points [i - 1].x, y, i, intersections);
		}
		// sort intersections by x 
		std::sort (intersections.begin (), intersections.end (), edgeSorter);
		// fill cells using parity flag
		i = intersections.size () - 1;
		if (i < 0) {
			continue;
		}
		x = round (intersections [i].x);
		double xmin = intersections [0].x;
		bool parity = true;
		i--;
		while (x > xmin) {
			if (parity) {
				sim->grid [sim->grid->adtIndex][x][y] = value;
			}
			x--;
			while (i >= 0 && x < intersections [i].x) {
				i--;
				parity = !parity;
			}
		}
	}
}

