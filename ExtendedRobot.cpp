/* 
 * File:   ExtendedRobot.cpp
 * Author: pedro
 * 
 * Created on 17 de Fevereiro de 2014, 18:32
 */

#include "ExtendedRobot.h"

using namespace Enki;

ExtendedRobot::ExtendedRobot() {
}

ExtendedRobot::ExtendedRobot(const ExtendedRobot& orig) {
}

ExtendedRobot::~ExtendedRobot() {
}

void ExtendedRobot::
initPhysicInteractions (double dt, PhysicSimulation* ps)
{
	for (size_t i = 0; i < physicInteractions.size (); i++ ) {
		physicInteractions [i]->init (dt, ps);
	}
}

void ExtendedRobot::
doPhysicInteractions (double dt, PhysicSimulation *ps)
{
	for (size_t i = 0; i < this->physicInteractions.size (); i++) {
		physicInteractions [i]->step (dt, ps);
	}
}

void ExtendedRobot::
finalizePhysicInteractions (double dt, PhysicSimulation* ps)
{
	for (size_t i = 0; i < physicInteractions.size (); i++ ) {
		physicInteractions [i]->finalize (dt, ps);
	}
}
