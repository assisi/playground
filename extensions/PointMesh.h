/* 
 * File:   PointMesh.h
 * Author: pedro
 *
 * Created on 23 de Junho de 2014, 16:32
 */

#ifndef POINTMESH_H
#define	POINTMESH_H

#include <enki/Geometry.h>

#include <iostream>
#include <vector>

namespace Enki
{
	/**
	 * Represents a mesh of points.
	 */
	class PointMesh {
	public:
		std::vector<Point> points;
	private:
		PointMesh ();
		PointMesh (int size);
	public:
		PointMesh (const PointMesh& orig);

		void addPointMesh (const PointMesh& orig, const Point &delta);

		virtual ~PointMesh();
		/**
		 * Create a random mesh of points arranged in a circle of the given
		 * radius.
		 */
		static PointMesh *makeCircleMesh (double radius, int numberPoints);
		/**
		 * Create a random mesh of points arranged in a ring of the given
		 * radii.
		 */
		static PointMesh *makeRingMesh (double innerRadius, double outerRadius, int numberPoints);
		/**
		 * Create a mesh of points arranged in a circumference of the given
		 * radii.
		 */
		static PointMesh *makeCircumferenceMesh (double radius, int numberPoints);
		/**
		 * Create a mesh of points arranged as a rectangular with the given
		 * width and height.
		 */
		static PointMesh *makeRectangularMesh (double width, double height, int numberPoints);
		/**
		 * Create a mesh of points arranged as a rectangular with the given
		 * width and height.
		 */
		static PointMesh *makeLineMesh (double x, double y, int numberPoints);
		/**
		 * Return the point at position {@code i}.
		 */
		Point &operator[] (int i)
		{
			return this->points [i];
		}
		/**
		 * Return the point at position {@code i}.
		 */
		const Point &operator[] (int i) const
		{
			return this->points [i];
		}
		/**
		 * Return the number of points in this mesh.
		 */
		int size () const
		{
			return this->points.size ();
		}
		void print (std::ostream &os) const;
	private:

	};

	std::ostream &operator<< (std::ostream &os, PointMesh const &mesh);
}


#endif	/* POINTMESH_H */


// Local Variables: 
// mode: c++
// mode: flyspell-prog
// ispell-local-dictionary: "british"
// End: 
