#include <limits>

#ifdef DEBUG
#include <iostream>
#endif

#include "AbstractGrid.h"

using namespace std;
using namespace Enki;

Point gridSize (const ExtendedWorld *world, double gridScale, double borderSize);
Point gridOrigin (const ExtendedWorld *world, double gridScale, double borderSize);

AbstractGrid::
AbstractGrid (const ExtendedWorld *world, double gridScale, double borderSize):
	gridScale (gridScale),
	borderSize (borderSize),
	size (gridSize (world, gridScale, borderSize)),
	origin (gridOrigin (world, gridScale, borderSize))
{
}

void gridProperties (const ExtendedWorld *world, double gridScale, double borderSize, Point *size, Point *origin)
{
	Enki::Vector min, max;
	// check type of wall
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
		throw new string ("AbstractGrid::initParameters(const ExtendedWorld*): Unhandled wall type");
	}
	// adjust to border size
	min.x -= borderSize + gridScale;
	min.y -= borderSize + gridScale;
	max.x += borderSize + gridScale;
	max.y += borderSize + gridScale;
	// compute output parameters
	if (size) {
		*size = (max - min) / gridScale;
		size->x = ceil (size->x);
		size->y = ceil (size->y);
	}
	if (origin) {
		*origin = min;
	}
#ifdef DEBUG
	std::cout
		<< "Abstract Grid\nmin: " << min
		<< "\nmax: " << max
		<< "\norigin: " << origin
		<< "\nsize: " << size
		<< "\ngrid scale: " << gridScale << std::endl;
#endif
}

Point gridSize (const ExtendedWorld *world, double gridScale, double borderSize)
{
	Point result;
	gridProperties (world, gridScale, borderSize, &result, NULL);
	return result;
}

Point gridOrigin (const ExtendedWorld *world, double gridScale, double borderSize)
{
	Point result;
	gridProperties (world, gridScale, borderSize, NULL, &result);
	return result;
}
