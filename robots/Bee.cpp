/*

 */

#include <cmath>

#include <boost/foreach.hpp>
#include <boost/math/constants/constants.hpp>

#include <PhysicalEngine.h>

#include "Bee.h"
#include "interactions/LightConstants.h"

const double pi = boost::math::constants::pi<double>();

// Signum function
template <typename T>
int sign(T val)
{
    return (T(0) < val) - (val < T(0));
}

namespace Enki
{
	const double Bee::SCALE_FACTOR = 1.0;

    Bee::Bee(double scaleFactor) :
        DifferentialWheeled(0.4, 2, 0.0),
        object_sensors(5)
    {
        // Set shape & color

        double len = 1.35; // Body length
        double w = 0.5; // Body width
        double h = 0.4; // Body height
        double m = 1;   // Body mass

        //setRectangular(len, w, h, 1);
        Polygone footprint;
        footprint.push_back(Point(len/2,w/4) * scaleFactor);
        footprint.push_back(Point(len/2-w/(4*sqrt(2)),w/2) * scaleFactor);
        footprint.push_back(Point(-len/2+w/(4*sqrt(2)),w/2) * scaleFactor);
        footprint.push_back(Point(-len/2,w/4) * scaleFactor);
        footprint.push_back(Point(-len/2,-w/4) * scaleFactor);
        footprint.push_back(Point(-len/2+w/(2*sqrt(2)),-w/2) * scaleFactor);
        footprint.push_back(Point(len/2-w/(2*sqrt(2)),-w/2) * scaleFactor);
        footprint.push_back(Point(len/2,-w/4) * scaleFactor);
        PhysicalObject::Hull hull(PhysicalObject::Part(footprint, h));
        setCustomHull(hull, m);
        setColor(Color(0.93,0.79,0,1));

        // Set other physical properties
        PhysicalObject::dryFrictionCoefficient = 2.5;

        int i = 0;
        for (double a = -pi/2; i < 5; i++, a += pi/4) 
        {
            object_sensors[i] = new ObjectSensor(this, 
                                            Vector(len/2-sin(a)*w/2, sin(a)*w/2),
                                            0, a, 10, 3731, 0, 0.7, 0);
            addLocalInteraction(object_sensors[i]);
        }

        double light_sensor_range = 10.0;
        light_sensor_blue = new LightSensor(light_sensor_range, this,
                                            Vector(0,0), 0.0, Light::Blue);
        addLocalInteraction(light_sensor_blue);

        // Check in the model why is this necessary
        double minMeasurableHeat = 0.0;
        double maxMeasurableHeat = 100.0;
        heat_sensor = new HeatSensor(this, Vector(0,0),
                                     minMeasurableHeat,
                                     maxMeasurableHeat);
        addPhysicInteraction(heat_sensor);
    }

    /* virtual */
    Bee::~Bee()
    {
        BOOST_FOREACH(ObjectSensor* p, object_sensors)
        {
            delete p;
        }
        delete light_sensor_blue;
    }
}
