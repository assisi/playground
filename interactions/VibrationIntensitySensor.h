/* 
 * File:   VibrationSensor.h
 * Author: Pedro Mariano
 *
 * Created on 21 de Janeiro de 2014, 12:14
 */

#ifndef __VIBRATION_INTENSITY_SENSOR__
#define __VIBRATION_INTENSITY_SENSOR__

#include <iostream>

#include <enki/Interaction.h>

#include "extensions/Component.h"

namespace Enki
{
	/**
	 * Represents a vibration sensor.
	 *
	 * @author Pedro Mariano
	 */
	class VibrationIntensitySensor
		: public LocalInteraction,
		  public Component
	{
		/**
		 * Maximum measurable amplitude of this sensor.
		 */
		const double maxMeasurableIntensity;
		/**
		 * Maximum measurable frequency of this sensor.
		 */
		const double minMeasurableIntensity;
		/**
		 * Standard deviation of the Gaussian noise that is applied to each
		 * perceived vibration intensity.
		 */
		const double standardDeviationGaussianNoise;
		/**
		 * Measured vibration intensity in the current simulation iteration.
		 *
		 * <p> This field is set to zero at the beginning of each time step.
		 * For each object that is a vibration actuator we update this
		 * attribute with the sensed vibration according to the vibration
		 * model.
		 */
		double intensity;
	public:
		VibrationIntensitySensor (
			double range, Enki::Robot* owner,
			Enki::Vector relativePosition,
			double minMeasurableIntensity, double maxMeasurableIntensity, double standardDeviationGaussianNoise);
		/**
		 * Copy constructor.
		 */
		VibrationIntensitySensor (const VibrationIntensitySensor& orig);

		virtual ~VibrationIntensitySensor ();
		/**
		 * Return the amplitude of the vibration sensed by this sensor.
		 */
		double getIntensity () const
		{
			return this->intensity;
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
	};

}

#endif /* __VIBRATION_SENSOR__ */


// Local Variables: 
// mode: c++
// mode: flyspell-prog
// ispell-local-dictionary: "british"
// End: 
