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
#include "interactions/HeatActuator.h"

namespace Enki
{

    class Casu : public ExtendedRobot
    {
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
        HeatActuator *peltier;

    private:
        World* world_;
        
    };
}

#endif
