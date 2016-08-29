/*! \file  Casu.h
    \brief CASU "robot" declaration.

 */

#ifndef ENKI_CASU_H
#define ENKI_CASU_H

#include <vector>

#include <PhysicalEngine.h>

#include "extensions/ExtendedWorld.h"
#include <interactions/IRSensor.h>
#include "interactions/DiagnosticLed.h"
#include "interactions/LightSourceFromAbove.h"
#include "interactions/HeatActuatorMesh.h"
#include "interactions/WaveVibrationSource.h"
#include "interactions/VibrationSensor.h"
#include "interactions/HeatSensor.h"
#include "interactions/AirPump.h"

namespace Enki
{

    enum {NORTH = 1, SOUTH = 2, WEST = 4, EAST = 8};

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
       static const Vector TEMP_SENS_FRONT_POS;
       static const Vector TEMP_SENS_RIGHT_POS;
       static const Vector TEMP_SENS_BACK_POS;
       static const Vector TEMP_SENS_LEFT_POS;

       /* peltier's parameters and configuration */
       static const Vector PELTIER_POSITION;
       static /*const*/ double PELTIER_THERMAL_RESPONSE;
       static const double PELTIER_RADIUS;
       static const double BRIDGE_LENGTH;
       static const double BRIDGE_WIDTH;
       static const double THERMAL_DIFFUSIVITY_COPPER_BRIDGE;

       /* air pump's parameters and configuration */
       static const int AIR_PUMP_QUANTITY;
       static const double AIR_PUMP_DISTANCE;
       static /*const*/ double AIR_PUMP_RANGE;
       static const double AIR_PUMP_ORIENTATION;
       static const double AIR_PUMP_APERTURE;

    public:
        //! Create a CASU
        Casu (Vector pos, double yaw, ExtendedWorld* world, double ambientTemperature, int bridgeMask = 0);

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
        //! Vibration sensors
        VibrationSensorVector vibration_sensors;

        typedef std::vector<AirPump *> AirPumpVector;
        //! Air pump
        AirPumpVector air_pumps;

    private:
        World* world_;
        void createBridge (ExtendedWorld* world, Vector direction);
    };
}

#endif
