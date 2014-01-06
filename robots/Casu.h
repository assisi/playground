/*! \file  Casu.h
    \brief CASU "robot" declaration.

 */

#ifndef ENKI_CASU_H
#define ENKI_CASU_H

#include <vector>

#include <PhyscalEngine.h>

#include <interactions/IRSensor.h>

namespace Enki
{
    class Casu : public Robot
    {
    public:
        //! Create a CASU
        Casu();

        //! Destructor
        ~Casu() { }

        //! Range
        std::vector<IRSensor> range_sensors;

        
    };
}

#endif
