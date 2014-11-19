/* 
 * File:   HeatSensor.h
 * Author: Pedro Mariano
 *
 * Created on February 17 2014, 11:41
 */
#ifndef __HEAT_SENSOR_H
#define __HEAT_SENSOR_H

#include <enki/Interaction.h>

#include "extensions/Component.h"
#include "extensions/PhysicInteraction.h"

namespace Enki
{
	class HeatSensor:
		// public PhysicalObject,
		public PhysicInteraction,
		public Component
	{
		double measuredHeat;
	public:
		const double minMeasurableHeat;
		const double maxMeasurableHeat;
		/**
		 * How fast does this sensor responds to environmental changes to
		 * temperature.  This attribute is used with conjunction with the
		 * delta time parameter of {@code step(double,PhysicSimulation*)}
		 * method.  The thermal response used is
		 *
		 * <code>thermalResponseTime * dt</code>
		 *
		 * This value should be less than one.
		 */
		const double thermalResponseTime;
	public:
		HeatSensor (
			Enki::Robot* owner, Enki::Vector relativePosition,
			double minMeasurableHeat, double maxMeasurableHeat,
			double thermalResponseTime,
			double ambientTemperature);
		HeatSensor (
			Enki::Robot* owner, Enki::Vector relativePosition,
			double minMeasurableHeat, double maxMeasurableHeat);
		double getMeasuredHeat () const
		{
			return this->measuredHeat;
		}
		/**
		 * Update the measured heat if the object is a heat actuator.
		 *
		 * @param dt time step.
		 *
		 * @param w world where the interaction takes place.
		 */
		virtual void init (double dt, PhysicSimulation* w);
		virtual void step (double dt, PhysicSimulation* w);
	};
}

#endif

// Local Variables: 
// mode: c++
// mode: flyspell-prog
// ispell-local-dictionary: "british"
// End: 
