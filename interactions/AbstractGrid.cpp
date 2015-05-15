#include <limits>

#include "AbstractGrid.h"

using namespace std;
using namespace Enki;

void AbstractGrid::
initParameters (const ExtendedWorld *world)
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
		throw new string ("AbstractGrid::initParameters(const ExtendedWorld*): Unhandled wall type");
	}

	min.x -= borderSize + gridScale;
	min.y -= borderSize + gridScale;
	max.x += borderSize + gridScale;
	max.y += borderSize + gridScale;
	this->size = (max - min) / gridScale;
	this->size.x = ceil (this->size.x);
	this->size.y = ceil (this->size.y);
	this->origin = min;
#ifdef DEBUG
	std::cout
		<< "Abstract Grid Simulation\nmin: " << min
		<< "\nmax: " << max
		<< "\nsize: " << size
		<< "\ngrid scale: " << gridScale << std::endl;
#endif
}
