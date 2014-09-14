/*

 */

#include <cmath>

#include <boost/foreach.hpp>
#include <boost/math/constants/constants.hpp>

#include <PhysicalEngine.h>
#include <interactions/IRSensor.h>

#include "Casu.h"
#include "interactions/LightSourceFromAbove.h"
#include "interactions/LightConstants.h"
#include "interactions/DiagnosticLed.h"

const double pi = boost::math::constants::pi<double>();

namespace Enki
{

	/*const*/ double Casu::VIBRATION_RANGE = 100;
	const Vector Casu::VIBRATION_POSITION = Vector (0, 0);
	/*const*/ double Casu::VIBRATION_MAXIMUM_AMPLITUDE = 8.5; // units??
	/*const*/ double Casu::VIBRATION_PHASE = 1;
	/*const*/ double Casu::VIBRATION_FREQUENCY = 350;
	const double Casu::VIBRATION_VELOCITY = 357000;  // sound speed in copper cm/s
	/*const*/ double Casu::VIBRATION_AMPLITUDE_QUADRATIC_DECAY = 2;
	/*const*/ double Casu::VIBRATION_NOISE = 1;

    // Temperature sensors configuration
    const double Casu::TEMP_SENS_COUNT = 5;
    const double Casu::MIN_MEASURABLE_HEAT = 0.0;
    const double Casu::MAX_MEASURABLE_HEAT = 100.0;
    const Vector Casu::TEMP_SENS_CENTER_POS = Vector(0.0,0.0);
    const Vector Casu::TEMP_SENS_NORTH_POS = (2.5,0.0);
    const Vector Casu::TEMP_SENS_EAST_POS = (0.0,-2.5);
    const Vector Casu::TEMP_SENS_SOUTH_POS = (-2.5,0.0);
    const Vector Casu::TEMP_SENS_WEST_POS = (0.0,2.5);

Casu::Casu(World* world) :
    world_(world),
    range_sensors(6),
    temp_sensors(TEMP_SENS_COUNT)
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

    // Add peltier actuator
    peltier = new HeatActuatorMesh (this, Vector(0,0), 23, 1.2, 1.6, 20);
    this->addPhysicInteraction(this->peltier);

	 // Add vibration actuator
	 this->vibration = new WaveVibrationSource
		(Casu::VIBRATION_RANGE, this,
		 Casu::VIBRATION_POSITION,
		 Casu::VIBRATION_MAXIMUM_AMPLITUDE,
		 Casu::VIBRATION_PHASE,
		 Casu::VIBRATION_FREQUENCY,
		 Casu::VIBRATION_VELOCITY,
		 Casu::VIBRATION_AMPLITUDE_QUADRATIC_DECAY,
		 Casu::VIBRATION_NOISE);
	 this->vibration->setCylindric(0, 0, -1); // Set to point object
	 world_->addObject (this->vibration);
	 addLocalInteraction (this->vibration);
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
}

// -----------------------------------------------------------------------------

}
