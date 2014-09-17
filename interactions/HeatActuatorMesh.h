/* 
 * File:   HeatActuatorMesh.h
 * Author: pedro
 *
 * Created on 9 de Junho de 2014, 16:31
 */

#ifndef HEATACTUATORMESH_H
#define	HEATACTUATORMESH_H

#include <enki/PhysicalEngine.h>
#include <enki/Geometry.h>

#include <vector>

#include "extensions/Component.h"
#include "extensions/PhysicInteraction.h"

#include "extensions/PointMesh.h"

#include "HeatActuatorPointSource.h"

namespace Enki
{
	/**
	 * A heat actuator that emits heat through a mesh of points.  This can be used to represent an actuator with different shapes.
    */
	class HeatActuatorMesh :
		public HeatActuatorPointSource
	{
		/**
		 * The points that compose this heat actuator.
		 */
		const PointMesh *mesh;
	public:
		/**
		 * Create a circular heat source with the given radius.  The mesh is composed of {@code numberPoints} randomly created.
       * @param owner
       * @param relativePosition
       * @param heat
       * @param radius
		 * @param numberPoints
       */
		HeatActuatorMesh (
			Enki::Robot* owner,
			Enki::Vector relativePosition,
			double thermalResponseTime,
			double ambientTemperature,
			double radius,
			int numberPoints);
		/**
		 * Create a ring heat source with the given radiuses.
       * @param owner
       * @param relativePosition
       * @param heat
       * @param innerRadius
       * @param outerRadius
       * @param numberPoints
       */
		HeatActuatorMesh (
			Enki::Robot* owner,
			Enki::Vector relativePosition,
			double thermalResponseTime,
			double ambientTemperature,
			double innerRadius,
			double outerRadius,
			int numberPoints);
		HeatActuatorMesh (
			Enki::Robot* owner,
			Enki::Vector relativePosition,
			double thermalResponseTime,
			double ambientTemperature,
			const PointMesh *mesh);
		HeatActuatorMesh(const HeatActuatorMesh& orig);
		virtual ~HeatActuatorMesh();
		virtual void step (double dt, PhysicSimulation* w);
	private:

	};
}

#endif	/* HEATACTUATORMESH_H */


// Local Variables: 
// mode: c++
// mode: flyspell-prog
// ispell-local-dictionary: "british"
// End: 
