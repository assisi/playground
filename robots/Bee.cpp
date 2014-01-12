/*

 */

#include <cmath>

#include <boost/foreach.hpp>
#include <boost/math/constants/constants.hpp>

#include <PhysicalEngine.h>

#include "Bee.h"

const double pi = boost::math::constants::pi<double>();

// Signum function
template <typename T>
int sign(T val)
{
    return (T(0) < val) - (val < T(0));
}

namespace Enki
{
    Bee::Bee(void) :
        DifferentialWheeled(0.4, 2, 0.0),
        range_sensors(8)
    {
        // Set shape & color
        double len = 2;
        double w = 0.4;
        double h = 0.4;
        Polygone head_footprint;
        Polygone body_footprint;
        
        for (double a = 0; a < 2*pi; a += pi/12)
        {
            head_footprint.push_back(Point(w * cos(a), w * sin(a)));
            body_footprint.push_back(Point(sign(cos(a)) * w + w * cos(a),
                                 w * sin(a)));
        }
        PhysicalObject::Hull head(PhysicalObject::Part(head_footprint, h));
        head.applyTransformation(Matrix22(0.0), Point(2.5 * w, 0));
        PhysicalObject::Hull body(PhysicalObject::Part(body_footprint, h));
        setCustomHull(head+body, 1);
        setColor(Color(0.93,0.79,0,1));

        // Set other physical properties
        PhysicalObject::dryFrictionCoefficient = 2.5;

        double da = pi/6; // Angle increment for range sensors
        for (double i = 0, a = -3*pi/4+da; a < 3*pi/4; a += da, i++)
        {
            range_sensors[i] = new IRSensor(this, 
                                            Vector(2.5*w + w*cos(a), w*sin(a)),
                                            0, a, 10, 3731, 0, 0.7, 0);
        }
    }

    /* virtual */
    Bee::~Bee()
    {
        BOOST_FOREACH(IRSensor* p, range_sensors)
        {
            delete p;
        }
    }
}
