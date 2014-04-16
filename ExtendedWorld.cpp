/* 
 * File:   ExtendedWorld.cpp
 * Author: pedro
 * 
 * Created on 17 de Fevereiro de 2014, 15:13
 */

#include "ExtendedWorld.h"

using namespace Enki;

ExtendedWorld::ExtendedWorld (double width, double height, const Color& wallsColor, unsigned texWidth, unsigned texHeight, const uint32_t* texData):
	World (width, height, wallsColor, texWidth, texHeight, texData)
{
}

ExtendedWorld::ExtendedWorld (double r, const Color& wallsColor, unsigned texWidth, unsigned texHeight, const uint32_t* texData):
	World (r, wallsColor, texWidth, texHeight, texData)
{
}

ExtendedWorld::ExtendedWorld ():
	World ()
{
}

ExtendedWorld::~ExtendedWorld ()
{
}
void ExtendedWorld::addObject (PhysicalObject *o)
{
	World::addObject (o);
	ExtendedRobot *er = dynamic_cast<ExtendedRobot *> (o);
	if (er != NULL) {
		this->extendedRobots.insert (er);
	}
}

void ExtendedWorld::addPhysicSimulation (PhysicSimulation *pi)
{
	this->physicSimulations.push_back (pi);
	pi->initParameters (this);
}

void ExtendedWorld::step (double dt, unsigned physicsOversampling)
{
	const double overSampledDt = dt / (double) physicsOversampling;
	for (unsigned po = 0; po < physicsOversampling; po++) {
		// init physics interactions
		for (PhysicSimulationsIterator pi = physicSimulations.begin (); pi != physicSimulations.end (); ++pi) {
			(*pi)->initStateComputing (overSampledDt);
			for (ExtendedRobotsIterator eri = extendedRobots.begin (); eri != extendedRobots.end (); ++eri) {
				(*eri)->initPhysicInteractions (overSampledDt, *pi);
				(*eri)->doPhysicInteractions (overSampledDt, *pi);
				(*eri)->finalizePhysicInteractions (overSampledDt, *pi);
			}
			(*pi)->computeNextState (overSampledDt);
		}
	}
	World::step (dt, physicsOversampling);
}
