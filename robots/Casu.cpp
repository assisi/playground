/*

 */

#include <cmath>

#include <boost/foreach.hpp>
#include <boost/math/constants/constants.hpp>

#include <PhysicalEngine.h>

#include "Casu.h"

const double pi = boost::math::constants::pi<double>();

namespace Enki
{
    Casu::Casu(void) :
        range_sensors(6),
        top_led(this)
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
        
    }

    Casu::~Casu()
    {
        BOOST_FOREACH(IRSensor* p, range_sensors)
        {
            delete p;
        }
    }

}
