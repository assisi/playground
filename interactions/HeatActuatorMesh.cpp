/* 
 * File:   HeatActuatorMesh.cpp
 * Author: pedro
 * 
 * Created on 9 de Junho de 2014, 16:31
 */

#include <enki/Random.h>

#include "HeatActuatorMesh.h"

#include "WorldHeat.h"

using namespace Enki;

HeatActuatorMesh::HeatActuatorMesh (
	Enki::Robot* owner,
	Enki::Vector relativePosition,
	double heat,
	double radius,
	int numberPoints
	):
	HeatActuatorPointSource (owner, relativePosition, heat), 
	mesh (numberPoints)
{
	while (numberPoints > 0) {
		numberPoints--;
		double angle = uniformRand () * 2 * M_PI;
		double distance = uniformRand () * radius;
		this->mesh [numberPoints].x = distance * cos (angle);
		this->mesh [numberPoints].y = distance * sin (angle);
	}
}

HeatActuatorMesh::HeatActuatorMesh (
	Enki::Robot* owner,
	Enki::Vector relativePosition,
	double heat,
	double innerRadius,
	double outerRadius,
	int numberPoints
	):
	HeatActuatorPointSource (owner, relativePosition, heat), 
	mesh (numberPoints)
{
	
	while (numberPoints > 0) {
		numberPoints--;
		double angle = uniformRand () * 2 * M_PI;
		double distance = uniformRand () * (outerRadius - innerRadius) + innerRadius;
		this->mesh [numberPoints].x = distance * cos (angle);
		this->mesh [numberPoints].y = distance * sin (angle);
	}
}

HeatActuatorMesh::HeatActuatorMesh (const HeatActuatorMesh& orig):
	HeatActuatorPointSource (orig),
	mesh (orig.mesh)
{
}

HeatActuatorMesh::~HeatActuatorMesh() {
}

void HeatActuatorMesh::
step (double dt, PhysicSimulation *ps)
{
	WorldHeat *worldHeat = dynamic_cast<WorldHeat *> (ps);
	if (worldHeat != NULL) {
		if (this->switchedOn) {
			for (int i = this->mesh.size() - 1; i >= 0; i--) {
				worldHeat->setHeatAt (this->absolutePosition + this->mesh [i], this->heat);
			}
		}
	}
}
