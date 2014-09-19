/*! \file  Bee.h
    \brief A simplistic bee model implementation.
*/
#ifndef ENKI_BEE_H
#define ENKI_BEE_H

#include <enki/robots/DifferentialWheeled.h>
#include "extensions/ExtendedRobot.h"
#include "interactions/ObjectSensor.h"
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
		static double SCALE_FACTOR;

	public:
        //! Create a Bee
		Bee(double scaleSize = Bee::SCALE_FACTOR);
        
        //! destructor
        virtual ~Bee();

        /* Sensors */

        typedef std::vector<ObjectSensor*> ObjectSensorVector;
        //! object proximity sensors.
        ObjectSensorVector object_sensors;

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
