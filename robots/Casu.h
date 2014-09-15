/*! \file  Casu.h
    \brief CASU "robot" declaration.

 */

#ifndef ENKI_CASU_H
#define ENKI_CASU_H

#include <vector>

#include <PhysicalEngine.h>

#include <interactions/IRSensor.h>
#include "interactions/DiagnosticLed.h"
#include "interactions/LightSourceFromAbove.h"
#include "interactions/HeatActuatorMesh.h"
#include "interactions/WaveVibrationSource.h"
#include "interactions/VibrationSensor.h"

namespace Enki
{

    class Casu : public ExtendedRobot
    {
    public:
       static /*const*/ double VIBRATION_SOURCE_RANGE;
       static const Vector VIBRATION_SOURCE_POSITION;
       static /*const*/ double VIBRATION_SOURCE_MAXIMUM_AMPLITUDE;
       static /*const*/ double VIBRATION_SOURCE_PHASE;
       static /*const*/ double VIBRATION_SOURCE_FREQUENCY;
       static const double VIBRATION_SOURCE_VELOCITY;
       static /*const*/ double VIBRATION_SOURCE_AMPLITUDE_QUADRATIC_DECAY;
       static /*const*/ double VIBRATION_SOURCE_NOISE;

		 static double VIBRATION_SENSOR_RANGE;
		 static const Vector VIBRATION_SENSOR_POSITION;
		 static /*const*/ double VIBRATION_SENSOR_MAX_MEASURABLE_AMPLITUDE;
		 static /*const*/ double VIBRATION_SENSOR_MAX_MEASURABLE_FREQUENCY;
		 static /*const*/ double VIBRATION_SENSOR_AMPLITUDE_STANDARD_DEVIATION_GAUSSIAN_NOISE;
		 static /*const*/ double VIBRATION_SENSOR_FREQUENCY_STANDARD_DEVIATION_GAUSSIAN_NOISE;
    public:
        //! Create a CASU
        Casu(World* world);

        //! Destructor
        ~Casu();

        /* Sensors */

        typedef std::vector<IRSensor*> IRSensorVector;
        //! Proximity sensors.
        IRSensorVector range_sensors;                
        
        /* Actuators */

        //! Light source
        LightSourceFromAbove* light_source_blue;

        //! Diagnostic LED
        DiagnosticLed* top_led;

        //! Peltier heater
        HeatActuatorMesh *peltier;

        //! Vibration actuator
        WaveVibrationSource *vibration;

		  VibrationSensor *sensor;

    private:
        World* world_;
        
    };
}

#endif
