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
#include "interactions/HeatSensor.h"

namespace Enki
{

    class Casu : public ExtendedRobot
    {
    public:
       static /*const*/ double VIBRATION_RANGE;
       static const Vector VIBRATION_POSITION;
       static /*const*/ double VIBRATION_MAXIMUM_AMPLITUDE;
       static /*const*/ double VIBRATION_PHASE;
       static /*const*/ double VIBRATION_FREQUENCY;
       static const double VIBRATION_VELOCITY;
       static /*const*/ double VIBRATION_AMPLITUDE_QUADRATIC_DECAY;
       static /*const*/ double VIBRATION_NOISE;

       /* Temperature sensors' parameters and configuration */
       static const double TEMP_SENS_COUNT;
       static const double MIN_MEASURABLE_HEAT;
       static const double MAX_MEASURABLE_HEAT;
       static const Vector TEMP_SENS_CENTER_POS;
       static const Vector TEMP_SENS_NORTH_POS;
       static const Vector TEMP_SENS_EAST_POS;
       static const Vector TEMP_SENS_SOUTH_POS;
       static const Vector TEMP_SENS_WEST_POS;


    public:
        //! Create a CASU
        Casu(World* world);

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
        WaveVibrationSource *vibration;

    private:
        World* world_;
        
    };
}

#endif
