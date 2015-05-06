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
	mesh (PointMesh::makeCircleMesh (radius, numberPoints)),
	shape (PointMesh::makeCircleMesh (radius, numberPoints))
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
	mesh (PointMesh::makeRingMesh (innerRadius, outerRadius, numberPoints)),
	shape (PointMesh::makeRingMesh (innerRadius, outerRadius, numberPoints))
{
}

HeatActuatorMesh::HeatActuatorMesh (
	Enki::Robot* owner,
	Enki::Vector relativePosition,
	double thermalResponseTime,
	double ambientTemperature,
	const PointMesh *mesh,
	const PointMesh *shape
	):
	HeatActuatorPointSource (owner, relativePosition, thermalResponseTime, ambientTemperature), 
	mesh (mesh),
	shape (shape)
{
}

HeatActuatorMesh::HeatActuatorMesh (const HeatActuatorMesh& orig):
	HeatActuatorPointSource (orig),
	mesh (orig.mesh),
	shape (orig.shape)
{
}

HeatActuatorMesh::~HeatActuatorMesh ()
{
	delete this->mesh;
	delete this->shape;
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

#include <iostream>

void HeatActuatorMesh::
setHeatDiffusivity (ExtendedWorld *world, double heatDiffusivity)
{
	Component::init ();
	BOOST_FOREACH (PhysicSimulation *ps, world->physicSimulations) {
		WorldHeat *worldHeat = dynamic_cast<WorldHeat *> (ps);
		if (worldHeat != NULL) {
			for (int i = this->shape->size () - 1; i >= 0; i--) {
				worldHeat->setHeatDiffusivityAt (this->absolutePosition + (*(this->shape)) [i], heatDiffusivity);
			}
			Point p = this->absolutePosition + (*(this->shape)) [0];
			std::cout
				<< "Set heat diffusivity @ "<< this->shape->size ()
				<< " points equal to " << heatDiffusivity
				<< ", first was " << p << "\n";
		}
	}
}
