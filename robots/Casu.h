/*! \file  Casu.h
    \brief CASU "robot" declaration.

 */

#ifndef ENKI_CASU_H
#define ENKI_CASU_H

#include <vector>

//#include <PhysicalEngine.h>

#include <interactions/IRSensor.h>

namespace Enki
{
    // Forward declarations
    class Robot;
    class IRSensor;
    
    class Casu : public Robot
    {
    public:
        //! Create a CASU
        Casu(void);

        //! Destructor
        ~Casu();

        //! Range
        typedef std::vector<IRSensor*> IRSensorVector;
        IRSensorVector range_sensors;

        
    };
}

#endif
