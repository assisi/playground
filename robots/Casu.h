/*! \file  Casu.h
    \brief CASU "robot" declaration.

 */

#ifndef ENKI_CASU_H
#define ENKI_CASU_H

#include <vector>

//#include <PhysicalEngine.h>

//#include <interactions/IRSensor.h>
//#include "interactions/DiagnosticLed.h"

namespace Enki
{
    // Forward declarations
    class Robot;
    class IRSensor;
    class LightSourceFromAbove;
    class DiagnosticLed;

    class Casu : public Robot
    {
    public:
        //! Create a CASU
        Casu(void);

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
        
    };
}

#endif
