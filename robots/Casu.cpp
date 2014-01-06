/*

 */

#include <boost/foreach.h>

#include <Casu.h>

namespace Enki
{
    Casu::Casu(void) :
        range_sensors(6)
    {
      
        // Add range sensors
        range_sensors[0] = new IRSensor(this, Vector(0.75,0), 0.5, deg2rad(0), 
                                        2, 3731, 0.3, 0.7, 0);
        range_sensors[1] = new IRSensor(this, Vector(0.38,0.65), 0.5, deg2rad(60), 
                                        2, 3731, 0.3, 0.7, 0);
        range_sensors[2] = new IRSensor(this, Vector(-0.38,0.65), 0.5, deg2rad(120), 
                                        2, 3731, 0.3, 0.7, 0);
        range_sensors[3] = new IRSensor(this, Vector(-0.75,0), 0.5, deg2rad(180), 
                                        2, 3731, 0.3, 0.7, 0);
        range_sensors[4] = new IRSensor(this, Vector(-0.35,-0.65), 0.5, -deg2rad(120), 
                                        2, 3731, 0.3, 0.7, 0);
        range_sensors[5] = new IRSensor(this, Vector(0.35,-0.65), 0.5, -deg2rad(60), 
                                        2, 3731, 0.3, 0.7, 0);

        
        
        BOOST_FOREACH(IRSensor* p, range_sensors)
        {
            addLocalInteraction(p);
        }
        
        setCustomHull(,);
        setColor(Color(0.8,0.8,0.8,0.5));
    }

    Casu::~Casu()
    {
        BOOST_FOREACH(IRSensor* p, range_sensors)
        {
            delete p;
        }
    }

}
