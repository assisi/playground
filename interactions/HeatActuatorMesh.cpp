/* 
 * File:   HeatActuatorMesh.cpp
 * Author: pedro
 * 
 * Created on 9 de Junho de 2014, 16:31
 */

#include "HeatActuatorMesh.h"

#include "WorldHeat.h"

#include "extensions/PointMesh.h"

using namespace Enki;

HeatActuatorMesh::HeatActuatorMesh (
	Enki::Robot* owner,
	Enki::Vector relativePosition,
	double heat,
	double radius,
	int numberPoints
	):
	HeatActuatorPointSource (owner, relativePosition, heat), 
	mesh (PointMesh::makeCircleMesh (radius, numberPoints))
{
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
	mesh (PointMesh::makeRingMesh (innerRadius, outerRadius, numberPoints))
{
}

HeatActuatorMesh::HeatActuatorMesh (
	Enki::Robot* owner,
	Enki::Vector relativePosition,
	double heat,
	const PointMesh *mesh
	):
	HeatActuatorPointSource (owner, relativePosition, heat), 
	mesh (mesh)
{
}

HeatActuatorMesh::HeatActuatorMesh (const HeatActuatorMesh& orig):
	HeatActuatorPointSource (orig),
	mesh (orig.mesh)
{
}

HeatActuatorMesh::~HeatActuatorMesh ()
{
	delete this->mesh;
}

#include <iostream>

void HeatActuatorMesh::
step (double dt, PhysicSimulation *ps)
{
	WorldHeat *worldHeat = dynamic_cast<WorldHeat *> (ps);
	if (worldHeat != NULL) {
		if (this->switchedOn) {
			// std::cout << "Setting heat @ " << this->absolutePosition << " to " << this->heat << '\n';
			// // std::cout << " + ";
			// // // std::cout << (*(this->mesh));
			// // // seca (std::cout, *(this->mesh));
			// // this->mesh->print (std::cout);
			// // // std::cout << *mesh;
			// // std::cout << "\n";
			for (int i = this->mesh->size () - 1; i >= 0; i--) {
				worldHeat->setHeatAt (this->absolutePosition + (*(this->mesh)) [i], this->heat);
			}
		}
	}
}
