/* 
 * File:   VibrationSensor.h
 * Author: Pedro Mariano
 *
 * Created on 7 de Janeiro de 2014, 12:14
 */

#include <iostream>

#include "../Interaction.h"
#include "../Geometry.h"
#include "../PhysicalEngine.h"

#include "../Component.h"

namespace Enki
{
	/**
	 * Represents a vibration sensor.
	 *
	 * @author Pedro Mariano
	 */
	class VibrationSensor
		: public LocalInteraction,
		  public Component
	{
		/**
		 * Maximum measurable amplitude of this sensor.
		 */
		const double maxMeasurableAmplitude;
		/**
		 * Maximum measurable frequency of this sensor.
		 */
		const double maxMeasurableFrequency;
		/**
		 * Measured amplitude in the current simulation iteration.
		 *
		 * <p> This field is set to zero at the beginning of each time step.
		 * For each object that is a vibration actuator we update this
		 * attribute with the sensed vibration according to the vibration
		 * model.
		 */
		double amplitude;
		/**
		 * Measured frequency in the current simulation iteration.
		 *
		 * <p> This field is set to zero at the beginning of each time step.
		 * For each object that is a vibration actuator we update this
		 * attribute with the sensed vibration according to the vibration
		 * model.
		 */
		double frequency;
	public:
		VibrationSensor (
			double range, Enki::Robot* owner,
			Enki::Vector relativePosition,
			 double maxMeasurableAmplitude, double maxMeasurableFrequency);
		/**
		 * Return the amplitude of the vibration sensed by this sensor.
		 */
		double getAmplitude () const
		{
			return this->amplitude;
		}
		/**
		 * Return the frequency of the vibration sensed by this sensor.
		 */
		double getFrequency () const
		{
			return this->frequency;
		}
		/**
		 * Initialise the measured amplitude and frequency in the current
		 * iteration step.
		 *
		 * @param dt time step.
		 *
		 * @param w world where the interaction takes place.
		 */
		virtual void init (double dt, Enki::World* w);
		/**
		 * Update the measured amplitude and frequency if the object is a
		 * vibration actuator.
		 *
		 * @param dt time step.
		 *
		 * @param w world where the interaction takes place.
		 */
		virtual void objectStep (double dt, Enki::World* w, Enki::PhysicalObject *po);


		virtual void finalize (double dt, Enki::World* w);
	};


	std::ostream & operator << (std::ostream & outs, const VibrationSensor &vs);
}
