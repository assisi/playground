/*! \file  Bee.h
    \brief A simplistic bee model implementation.
*/
#ifndef ENKI_BEE_H
#define ENKI_BEE_H

#include <enki/robots/DifferentialWheeled.h>
#include "extensions/ExtendedRobot.h"
#include <enki/interactions/IRSensor.h>
#include "interactions/HeatSensor.h"
#include "interactions/LightSensor.h"
	
namespace Enki
{
	
	/**
	 * A simplistic bee model.
	 *
	 * \ingroup robot */
	class Bee : public DifferentialWheeled, public ExtendedRobot
	{

	public:
        //! Create a Bee
        Bee(void);
        
        //! destructor
        virtual ~Bee();

        /* Sensors */

        typedef std::vector<IRSensor*> IRSensorVector;
        //! IR proximity sensors.
        IRSensorVector range_sensors;

        //! Light sensor.
        LightSensor* light_sensor_blue;

        //! Heat sensor.
        HeatSensor* heat_sensor;

        /**
         * Update the position of this bee.  
         */
        void step (int dt, const World *);
	};
}

#endif
