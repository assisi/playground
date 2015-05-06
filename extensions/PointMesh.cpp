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

void PointMesh::addPointMesh (const PointMesh& orig, const Point &delta)
{
	std::cout << "Adding point mesh with delta " << delta << "\n";
	for (int i = orig.size () - 1; i >= 0; i--) {
		this->points.push_back (orig [i] + delta);
	}
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

PointMesh *PointMesh::makeRectangularMesh (double width, double height, int numberPoints)
{
	numberPoints -= numberPoints % 4;
	PointMesh *result = new PointMesh (numberPoints);
	double sc = numberPoints / 4;
	while (numberPoints > 3) {
		double scale = (4 * numberPoints) / sc;
		numberPoints--;
		result->points [numberPoints].x =   width / 2;
		result->points [numberPoints].y = -height / 2 + scale * height;
		numberPoints--;
		result->points [numberPoints].x =  -width / 2;
		result->points [numberPoints].y = -height / 2 + scale * height;
		numberPoints--;
		result->points [numberPoints].x =  -width / 2 + scale * width;
		result->points [numberPoints].y = -height / 2;
		numberPoints--;
		result->points [numberPoints].x =  -width / 2 + scale * width;
		result->points [numberPoints].y =  height / 2;
	}
	return result;
}

PointMesh *PointMesh::makeLineMesh (double x, double y, int numberPoints)
{
	PointMesh *result = new PointMesh (numberPoints);
	double sc = numberPoints - 1;
	while (numberPoints > 0) {
		numberPoints--;
		double scale = numberPoints / sc;
		result->points [numberPoints].x = scale * x;
		result->points [numberPoints].y = scale * y;
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

std::ostream &operator<< (std::ostream &os, PointMesh const &mesh)
{
	os << mesh [0];
	for (int i = 1; i < mesh.size (); i++) {
		os << ' ' << mesh [i];
	}
	return os;
}
