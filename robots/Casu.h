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
#include "interactions/HeatSensor.h"

namespace Enki
{

    class Casu : public ExtendedRobot
    {
    public:
       static /*const*/ double VIBRATION_SOURCE_RANGE;
       static const Vector VIBRATION_SOURCE_POSITION;
       static /*const*/ double VIBRATION_SOURCE_MAXIMUM_AMPLITUDE;
       static /*const*/ double VIBRATION_SOURCE_PHASE;
       static const double VIBRATION_SOURCE_VELOCITY;
       static /*const*/ double VIBRATION_SOURCE_AMPLITUDE_QUADRATIC_DECAY;
       static /*const*/ double VIBRATION_SOURCE_NOISE;

		 static const int NUMBER_VIBRATION_SENSORS;
		 static double VIBRATION_SENSOR_RANGE;
		 /**
		  * Casu vibration sensors relative position in cm.
		  */
		 static const Vector VIBRATION_SENSOR_POSITION[];
		 /**
		  * Maximum vibration frequency measured by casu vibration sensors.
		  */
		 static /*const*/ double VIBRATION_SENSOR_MAX_MEASURABLE_FREQUENCY;
		 static /*const*/ double VIBRATION_SENSOR_AMPLITUDE_STANDARD_DEVIATION_GAUSSIAN_NOISE;
		 static /*const*/ double VIBRATION_SENSOR_FREQUENCY_STANDARD_DEVIATION_GAUSSIAN_NOISE;

       /* Temperature sensors' parameters and configuration */
       static const double TEMP_SENS_COUNT;
       static const double MIN_MEASURABLE_HEAT;
       static const double MAX_MEASURABLE_HEAT;
       static const Vector TEMP_SENS_CENTER_POS;
       static const Vector TEMP_SENS_NORTH_POS;
       static const Vector TEMP_SENS_EAST_POS;
       static const Vector TEMP_SENS_SOUTH_POS;
       static const Vector TEMP_SENS_WEST_POS;

		 /* peltier's parameters and configuration */
		 static const Vector PELTIER_POSITION;
		 static /*const*/ double PELTIER_THERMAL_RESPONSE_TIME;

    public:
        //! Create a CASU
		 Casu (World* world, double ambientTemperature);

        //! Destructor
        ~Casu();

        /* Sensors */

        typedef std::vector<IRSensor*> IRSensorVector;
        //! Proximity sensors.
        IRSensorVector range_sensors;    

        typedef std::vector<HeatSensor*> TempSensorVector;
        TempSensorVector temp_sensors;
        
        /* Actuators */

        //! Light source
        LightSourceFromAbove* light_source_blue;

        //! Diagnostic LED
        DiagnosticLed* top_led;

        //! Peltier heater
        HeatActuatorMesh *peltier;

        //! Vibration actuator
        WaveVibrationSource *vibration_source;

        typedef std::vector<VibrationSensor *> VibrationSensorVector;
		  VibrationSensorVector vibration_sensors;

    private:
        World* world_;
        
    };
}

#endif
