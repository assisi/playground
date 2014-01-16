/* 
 * File:   VibrationActuator.h
 * Author: Pedro Mariano
 *
 * Created on 7 de Janeiro de 2014, 12:14
 */

#include <iostream>

#include "../Interaction.h"
#include "../PhysicalEngine.h"
#include "../Geometry.h"

#include "../Component.h"

namespace Enki
{
	class VibrationSensor;
	/**
	 * Represents an actuator capable of emitting vibrations characterised
	 * by an amplitude and a frequency.
	 *
	 * @author Pedro Mariano
	 */
	class VibrationActuator :
		public LocalInteraction,
		public Component
	{
		friend VibrationSensor;
		/**
		 * Current amplitude of the vibration made by this actuator.
		 */
		double amplitude;
		/**
		 * Current frequency of the vibration made by this actuator.
		 */
		double frequency;
	public:
		VibrationActuator (double range, Enki::Robot* owner, Enki::Vector relativePosition, double amplitude, double frequency);
		
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
		 * Write the properties of this vibration actuator to the given
		 * output stream.
		 */
		void writePropertiesCSV (std::ostream &os) const;
		/**
		 * Write the state of this vibration actuator to the given output
		 * stream.  The state is the current vibration amplitude and
		 * frequency.
		 */
		void writeStateCSV (std::ostream &os) const;
	};

	std::ostream & operator << (std::ostream & outs, const VibrationActuator &va);
}
