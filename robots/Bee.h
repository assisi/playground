/*! \file  Bee.h
    \brief A simplistic bee model implementation.
*/
#ifndef ENKI_BEE_H
#define ENKI_BEE_H

#include <enki/robots/DifferentialWheeled.h>
#include <enki/interactions/IRSensor.h>

/*!	\file EPuck.h
	\brief Header of the E-puck robot
*/
	
namespace Enki
{
	
	//! A simplistic bee model.
	/*! \ingroup robot */
	class Bee : public DifferentialWheeled
	{
	public:
        //! Create a Bee
        Bee(void);
        
        //! destructor
        virtual ~Bee();

        typedef std::vector<IRSensor*> IRSensorVector;
        IRSensorVector range_sensors;

	};
}

#endif
