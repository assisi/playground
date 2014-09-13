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
       static /*const*/ double PELTIER_RANGE;
       static const Vector PELTIER_POSITION;
       static /*const*/ double PELTIER_MAXIMUM_AMPLITUDE;
       static /*const*/ double PELTIER_PHASE;
       static /*const*/ double PELTIER_FREQUENCY;
       static const double PELTIER_VELOCITY;
       static /*const*/ double PELTIER_AMPLITUDE_QUADRATIC_DECAY;
       static /*const*/ double PELTIER_NOISE;

		 static double VIBRATION_RANGE;
		 static const Vector VIBRATION_POSITION;
		 static /*const*/ double VIBRATION_MAX_MEASURABLE_AMPLITUDE;
		 static /*const*/ double VIBRATION_MAX_MEASURABLE_FREQUENCY;
		 static /*const*/ double VIBRATION_AMPLITUDE_STANDARD_DEVIATION_GAUSSIAN_NOISE;
		 static /*const*/ double VIBRATION_FREQUENCY_STANDARD_DEVIATION_GAUSSIAN_NOISE;
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
