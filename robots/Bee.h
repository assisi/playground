/*! \file  Bee.h
    \brief A simplistic bee model implementation.
*/
#ifndef ENKI_BEE_H
#define ENKI_BEE_H

#include <enki/robots/DifferentialWheeled.h>
#include <enki/interactions/IRSensor.h>

#include "interactions/LightSensor.h"
	
namespace Enki
{
	
	/**
	 * A simplistic bee model.
	 *
	 * Bees react to temperature and vibration thresholds.  During each time
	 * step, they sense temperature and vibration and depending on the
	 * threshold they either move away or they 
	 *
	 * \ingroup robot */
	class Bee : public DifferentialWheeled
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

        /**
         * Update the position of this bee.  
         */
        void step (int dt, const World *);
	};
}

#endif
