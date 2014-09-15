/* 
 * File:   PointMesh.cpp
 * Author: pedro
 * 
 * Created on 23 de Junho de 2014, 16:32
 */

#include "PointMesh.h"

#include <enki/Random.h>

using namespace Enki;

PointMesh::PointMesh ()
{
}

PointMesh::PointMesh (int size):
	points (size)
{
}

PointMesh::PointMesh (const PointMesh& orig):
	points (orig.points)
{
}

PointMesh::~PointMesh ()
{
}

PointMesh *PointMesh::makeCircleMesh (double radius, int numberPoints)
{
	PointMesh *result = new PointMesh (numberPoints);
	while (numberPoints > 0) {
		numberPoints--;
		double angle = uniformRand () * 2 * M_PI;
		double distance = uniformRand () * radius;
		result->points [numberPoints].x = distance * cos (angle);
		result->points [numberPoints].y = distance * sin (angle);
	}
	return result;
}

PointMesh *PointMesh::makeRingMesh (double innerRadius, double outerRadius, int numberPoints)
{
	PointMesh *result = new PointMesh (numberPoints);
	std::cout << numberPoints << "=> ";
	while (numberPoints > 0) {
		numberPoints--;
		double angle = uniformRand () * 2 * M_PI;
		double distance = uniformRand () * (outerRadius - innerRadius) + innerRadius;
		result->points [numberPoints].x = distance * cos (angle);
		result->points [numberPoints].y = distance * sin (angle);
	}
	result->print (std::cout);
	std::cout << '\n';

	return result;
}

PointMesh *PointMesh::makeCircumferenceMesh (double radius, int numberPoints)
{
	PointMesh *result = new PointMesh (numberPoints);
	int range = numberPoints;
	while (numberPoints > 0) {
		numberPoints--;
		double angle = numberPoints * 2 * M_PI / range;
		result->points [numberPoints].x = radius * cos (angle);
		result->points [numberPoints].y = radius * sin (angle);
	}
	return result;
}

void PointMesh::print (std::ostream &os) const
{
	os << this->points [0];
	for (int i = 1; i < this->points.size (); i++) {
		os << ' ' << this->points [i];
	}
}

std::ostream &operator<< (std::ostream &os, const PointMesh  &mesh)
{
	os << mesh [0];
	for (int i = 1; i < mesh.size (); i++) {
		os << ' ' << mesh [i];
	}
	return os;
}
