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
    Casu::Casu(World* world) :
        range_sensors(6)
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

        // TODO: Move these definitions to the Casu declaration
        //       and make them static symbolic constants!
        double light_radius = 5.0; // 5cm
        double light_k = 1;            // Corresponds to k = 100 for lengths in meters
        double I_max = 1.0;        // TODO: Figure out correct value
        double light_sigma = 1.0;
        this->light_source_blue = new LightSourceFromAbove(2*light_radius, this, Vector(0,0), 0,
                                                      light_k, light_radius, Light::Blue, 
                                                      I_max, light_sigma);
        
        this->light_source_blue->setCylindric(0, 0, -1); // Set to point object
        world->addObject(this->light_source_blue);
                                                    
        

        // Add diagnostic led
        top_led = new DiagnosticLed(this);
        
    }

    Casu::~Casu()
    {
        BOOST_FOREACH(IRSensor* p, range_sensors)
        {
            delete p;
        }

        delete top_led;
    }

}
