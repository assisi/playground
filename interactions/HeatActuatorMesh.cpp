/* 
 * File:   HeatActuatorMesh.cpp
 * Author: pedro
 * 
 * Created on 9 de Junho de 2014, 16:31
 */

#include <boost/foreach.hpp>

#include "HeatActuatorMesh.h"

#include "WorldHeat.h"

#include "extensions/PointMesh.h"
#include "extensions/PhysicSimulation.h"

using namespace Enki;

HeatActuatorMesh::HeatActuatorMesh (
	Enki::Robot* owner,
	Enki::Vector relativePosition,
	double thermalResponseTime,
	double ambientTemperature,
	double radius,
	int numberPoints
	):
	HeatActuatorPointSource (owner, relativePosition, thermalResponseTime, ambientTemperature),
	mesh (PointMesh::makeCircleMesh (radius, numberPoints))
{
}

HeatActuatorMesh::HeatActuatorMesh (
	Enki::Robot* owner,
	Enki::Vector relativePosition,
	double thermalResponseTime,
	double ambientTemperature,
	double innerRadius,
	double outerRadius,
	int numberPoints
	):
	HeatActuatorPointSource (owner, relativePosition, thermalResponseTime, ambientTemperature), 
	mesh (PointMesh::makeRingMesh (innerRadius, outerRadius, numberPoints))
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
	// delete this->shape;
}

#include <iostream>

void HeatActuatorMesh::
step (double dt, PhysicSimulation *ps)
{
	WorldHeat *worldHeat = dynamic_cast<WorldHeat *> (ps);
	if (worldHeat != NULL) {
		if (this->switchedOn) {
			double value = this->getRealHeat (dt, worldHeat);
			for (int i = this->mesh->size () - 1; i >= 0; i--) {
				worldHeat->setHeatAt (this->absolutePosition + (*(this->mesh)) [i], value);
			}
		}
	}
}
