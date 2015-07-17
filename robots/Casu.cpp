/*

 */

#include <cmath>
#include <iostream>

#include <boost/foreach.hpp>
#include <boost/math/constants/constants.hpp>

#include <PhysicalEngine.h>
#include <interactions/IRSensor.h>

#include "Casu.h"
#include "interactions/LightSourceFromAbove.h"
#include "interactions/LightConstants.h"
#include "interactions/DiagnosticLed.h"
#include "extensions/PointMesh.h"

const double pi = boost::math::constants::pi<double>();

namespace Enki
{
    // Bee-Casu geometry
    const double Casu::BRIDGE_LENGTH = 4.5;
    const double Casu::BRIDGE_WIDTH = 1.0;

    const double Casu::THERMAL_DIFFUSIVITY_COPPER_BRIDGE = 0.5 * WorldHeat::THERMAL_DIFFUSIVITY_COPPER;

    const int Casu::AIR_PUMP_QUANTITY = 1;
    const double Casu::AIR_PUMP_DISTANCE = 0;
    /*const*/ double Casu::AIR_PUMP_RANGE = 5;
    const double Casu::AIR_PUMP_ORIENTATION = 0;
    const double Casu::AIR_PUMP_APERTURE = 2 * pi / AIR_PUMP_QUANTITY / 2;

    /*const*/ double Casu::VIBRATION_SOURCE_RANGE = 100;
    const Vector Casu::VIBRATION_SOURCE_POSITION = Vector (0, 0);
    /*const*/ double Casu::VIBRATION_SOURCE_MAXIMUM_AMPLITUDE = 8.5; // units??
    /*const*/ double Casu::VIBRATION_SOURCE_PHASE = 1;
    const double Casu::VIBRATION_SOURCE_VELOCITY = 357000;  // sound speed in copper cm/s
    /*const*/ double Casu::VIBRATION_SOURCE_AMPLITUDE_QUADRATIC_DECAY = 2;
    /*const*/ double Casu::VIBRATION_SOURCE_NOISE = 1;


    const int Casu::NUMBER_VIBRATION_SENSORS = 4;
    double Casu::VIBRATION_SENSOR_RANGE = 100;
    const Vector Casu::VIBRATION_SENSOR_POSITION[] = {
        Vector (-2.5,  0),
        Vector ( 2.5,  0),
        Vector (   0, -2.5),
        Vector (   0,  2.5)
    };
    /*const*/ double Casu::VIBRATION_SENSOR_MAX_MEASURABLE_FREQUENCY = 500;
    /*const*/ double Casu::VIBRATION_SENSOR_AMPLITUDE_STANDARD_DEVIATION_GAUSSIAN_NOISE = 0;
    /*const*/ double Casu::VIBRATION_SENSOR_FREQUENCY_STANDARD_DEVIATION_GAUSSIAN_NOISE = 0;

    // Temperature sensors configuration
    const double Casu::TEMP_SENS_COUNT = 5;
    const double Casu::MIN_MEASURABLE_HEAT = 0.0;
    const double Casu::MAX_MEASURABLE_HEAT = 100.0;

    const Vector Casu::TEMP_SENS_NORTH_POS = Vector(2.5,0.0);
    const Vector Casu::TEMP_SENS_EAST_POS = Vector(0.0,-2.5);
    const Vector Casu::TEMP_SENS_SOUTH_POS = Vector(-2.5,0.0);
    const Vector Casu::TEMP_SENS_WEST_POS = Vector(0.0,2.5);
    const Vector Casu::TEMP_SENS_CENTER_POS = Vector(0.0,0.0);

    /* peltier's parameters and configuration */
    const Vector Casu::PELTIER_POSITION = Vector (0, 0);
    /*const*/ double Casu::PELTIER_THERMAL_RESPONSE = 0.3;
    const double Casu::PELTIER_RADIUS = 1.6;

    Casu::Casu(double yaw, ExtendedWorld* world, double ambientTemperature, int bridgeMask) :
        world_(world),
        range_sensors(6),
        vibration_sensors (Casu::NUMBER_VIBRATION_SENSORS),
        temp_sensors(TEMP_SENS_COUNT),
        air_pumps (Casu::AIR_PUMP_QUANTITY)
    {
  
        // Set physical properties
        double radius = 1;
        double height = 2;
        Polygone hex;
        for (double a = pi/6; a < 2*pi; a += pi/3)
            {
                hex.push_back(Point(radius * cos(a), radius * sin(a)));
            }
        PhysicalObject::Hull hull(PhysicalObject::Part(hex, height));
        setCustomHull(hull, 1000);
        setColor(Color(0.8,0.8,0.8,0.3));
        PhysicalObject::dryFrictionCoefficient = 1000; // Casus are immovable

        // Add range sensors
        range_sensors[0] = new IRSensor(this, Vector(0.866,0), 0, 0, 
                                                  2, 3731, 0.3, 0.7, 0);
        range_sensors[1] = new IRSensor(this, Vector(0.43,0.75), 0, pi/3, 
                                                  2, 3731, 0.3, 0.7, 0);
        range_sensors[2] = new IRSensor(this, Vector(-0.43,0.75), 0, 2*pi/3, 
                                                  2, 3731, 0.3, 0.7, 0);
        range_sensors[3] = new IRSensor(this, Vector(-0.866,0), 0, pi, 
                                                  2, 3731, 0.3, 0.7, 0);
        range_sensors[4] = new IRSensor(this, Vector(-0.43,-0.75), 0, -2*pi/3, 
                                                  2, 3731, 0.3, 0.7, 0);
        range_sensors[5] = new IRSensor(this, Vector(0.43,-0.75), 0, -pi/3, 
                                                  2, 3731, 0.3, 0.7, 0);

        BOOST_FOREACH(IRSensor* p, range_sensors)
            {
                addLocalInteraction(p);
            }

        // Add temperature senosrs
        temp_sensors[0] = new HeatSensor(this, TEMP_SENS_NORTH_POS,
                                                    MIN_MEASURABLE_HEAT,
                                                    MAX_MEASURABLE_HEAT);
        temp_sensors[1] = new HeatSensor(this, TEMP_SENS_EAST_POS,
                                                    MIN_MEASURABLE_HEAT,
                                                    MAX_MEASURABLE_HEAT);
        temp_sensors[2] = new HeatSensor(this, TEMP_SENS_SOUTH_POS,
                                                    MIN_MEASURABLE_HEAT,
                                                    MAX_MEASURABLE_HEAT);
        temp_sensors[3] = new HeatSensor(this, TEMP_SENS_WEST_POS,
                                                    MIN_MEASURABLE_HEAT,
                                                    MAX_MEASURABLE_HEAT);
        temp_sensors[4] = new HeatSensor(this, TEMP_SENS_CENTER_POS,
                                                    MIN_MEASURABLE_HEAT,
                                                    MAX_MEASURABLE_HEAT);

        BOOST_FOREACH(HeatSensor* p, temp_sensors)
            {
                addPhysicInteraction(p);
            }

        // TODO: Move these definitions to the Casu declaration
        //       and make them static symbolic constants!
        double light_radius = 5.0; // 5cm
        double light_k = 1;            // Corresponds to k = 100 for lengths in meters
        double I_max = 0.0;        // TODO: Figure out correct values
        double light_sigma = 1.0;
        this->light_source_blue = new LightSourceFromAbove(2*light_radius, this, Vector(0,0), 0,
                                                                            light_k, light_radius, Light::Blue, 
                                                                            I_max, light_sigma);
    
        this->light_source_blue->setCylindric(0, 0, -1); // Set to point object
        world_->addObject(this->light_source_blue);
                                                
        // Add diagnostic led
        top_led = new DiagnosticLed(this);

        // Add bridges
        Matrix22 rot (yaw);
        if (bridgeMask & NORTH) {
           createBridge (world, rot * Point (0, 1));
        }
        if (bridgeMask & SOUTH) {
           createBridge (world, rot * Point (0, -1));
        }
        if (bridgeMask & EAST) {
           createBridge (world, rot * Point (1, 0));
        }
        if (bridgeMask & WEST) {
           createBridge (world, rot * Point (-1, 0));
        }

        // Add peltier actuator
        // TODO: make peltier parameters CASU constants
        peltier = new HeatActuatorMesh
           (this, Vector(0,0),
            PELTIER_THERMAL_RESPONSE, ambientTemperature,
            PELTIER_RADIUS, 16);
        this->addPhysicInteraction(this->peltier);
        world->worldHeat->drawCircle (WorldHeat::THERMAL_DIFFUSIVITY_COPPER, this->pos, PELTIER_RADIUS);

        // Add vibration actuator

        this->vibration_source = new WaveVibrationSource
            (Casu::VIBRATION_SOURCE_RANGE, this,
             Casu::VIBRATION_SOURCE_POSITION,
             Casu::VIBRATION_SOURCE_MAXIMUM_AMPLITUDE,
             Casu::VIBRATION_SOURCE_PHASE,
             Casu::VIBRATION_SOURCE_VELOCITY,
             Casu::VIBRATION_SOURCE_AMPLITUDE_QUADRATIC_DECAY,
             Casu::VIBRATION_SOURCE_NOISE);
        this->vibration_source->setCylindric(0, 0, -1); // Set to point object
        world_->addObject (this->vibration_source);
        addLocalInteraction (this->vibration_source);

        // Add vibration sensors
        for (int i = 0; i < Casu::NUMBER_VIBRATION_SENSORS; i++) {
            VibrationSensor *vs = new VibrationSensor 
                (Casu::VIBRATION_SENSOR_RANGE, this,
                 Casu::VIBRATION_SENSOR_POSITION [i], 0,
                 Casu::VIBRATION_SENSOR_MAX_MEASURABLE_FREQUENCY,
                 Casu::VIBRATION_SENSOR_AMPLITUDE_STANDARD_DEVIATION_GAUSSIAN_NOISE,
                 Casu::VIBRATION_SENSOR_FREQUENCY_STANDARD_DEVIATION_GAUSSIAN_NOISE);
            this->vibration_sensors [i] = vs;
        }

        // Add air pump actuator
        for (int i = 0; i < Casu::AIR_PUMP_QUANTITY; i++) {
            double angle = Casu::AIR_PUMP_ORIENTATION + i * 2 * pi / Casu::AIR_PUMP_QUANTITY;
            Vector position (Casu::AIR_PUMP_DISTANCE * cos (angle), Casu::AIR_PUMP_DISTANCE * sin (angle));
            AirPump *airPump = new AirPump
                (Casu::AIR_PUMP_RANGE,
                 this,
                 position,
                 angle,
                 Casu::AIR_PUMP_APERTURE);
            airPump->setCylindric(0, 0, -1); // Set to point object
            world_->addObject (airPump);
            addLocalInteraction (airPump);
            this->air_pumps [i] = airPump;
        }
    }

// -----------------------------------------------------------------------------

    Casu::~Casu()
    {
        BOOST_FOREACH(IRSensor* p, range_sensors)
            {
                delete p;
            }

        world_->removeObject(this->light_source_blue);

        delete top_led;

        BOOST_FOREACH(AirPump* p, air_pumps)
            {
                delete p;
            }
    }

// -----------------------------------------------------------------------------

void Casu::
createBridge (ExtendedWorld* world, Vector direction)
{
	std::vector<Point> polygon;
	polygon.reserve (4);
	Point p1, p2;
	p2 = direction.perp ();
	p2 *= Casu::BRIDGE_WIDTH / 2;
	p2 += this->pos;
	polygon.push_back (p2);
	p1 = direction;
	p1 *= Casu::BRIDGE_LENGTH;
	p1 += p2;
	polygon.push_back (p1);
	p1 = direction;
	p1 *= Casu::BRIDGE_LENGTH;
	p2 = direction.perp ();
	p2 *= -Casu::BRIDGE_WIDTH / 2;
	p1 += p2;
	p1 += this->pos;
	polygon.push_back (p1);
	p2 += this->pos;
	polygon.push_back (p2);
	world->worldHeat->drawPolygon (Casu::THERMAL_DIFFUSIVITY_COPPER_BRIDGE, polygon);
}

}
