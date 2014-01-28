/*! \file  Bee.h
    \brief A simplistic bee model implementation.
*/
#ifndef ENKI_BEE_H
#define ENKI_BEE_H

#include <enki/robots/DifferentialWheeled.h>
#include <enki/interactions/IRSensor.h>

	
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

        typedef std::vector<IRSensor*> IRSensorVector;
        IRSensorVector range_sensors;

		  /**
			* Update the position of this bee.  
			*/
		  void step (int dt, const World *);
	};
}

#endif
