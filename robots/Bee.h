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
#include "interactions/AirFlowSensor.h"

namespace Enki
{
	
	/**
	 * A simplistic bee model.
	 *
	 * \ingroup robot */
	class Bee : public DifferentialWheeled, public ExtendedRobot
	{
	public:

		static const Vector AIR_FLOW_SENSOR_POSITION;
		static /*const*/ double AIR_FLOW_SENSOR_RANGE;
		static const double AIR_FLOW_SENSOR_ORIENTATION;

	public:
        //! Create a Bee
		Bee(double body_length, double body_width, double body_height,
            double body_mass, double max_speed);
        
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

        //! Air flow sensor.
        AirFlowSensor *air_flow_sensor;

        //! Bee color (useful for diagnostic purposes)
        double color_r_;
        double color_g_;
        double color_b_;

        //! Bee dimensions
        double len_;
        double w_;
        double h_;
        double m_;
        double v_max_;

        /**
         * Update the position of this bee.  
         */
        void step (int dt, const World *);

        /**
         * Update the bee color (useful for diagnostic purposes).
         */
        void setColor(double r, double g, double b);
	};
}

#endif
