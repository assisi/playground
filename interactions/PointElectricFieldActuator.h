/* 
 * File:   ElectricFieldActuator.h
 * Author: pedro
 *
 * Created on 3 de Março de 2014, 19:05
 */

#ifndef ELECTRICFIELDACTUATOR_H
#define	ELECTRICFIELDACTUATOR_H

#include "AbstractElectricFieldActuator.h"

namespace Enki {
	
	class PointElectricFieldActuator:
		public AbstractElectricFieldActuator
	{
	public:
		/**/
		double electricCharge;
	public:
		PointElectricFieldActuator (
			Robot *owner, Vector relativePosition,
			double electricCharge);
		PointElectricFieldActuator (const PointElectricFieldActuator& orig);
		virtual ~PointElectricFieldActuator();
	public:
		virtual void init (double dt, World *w);
		virtual void measureAt (const Point &, double *value, Point *direction);

	};
}

#endif	/* ELECTRICFIELDACTUATOR_H */




// Local Variables: 
// mode: c++
// mode: flyspell-prog
// ispell-local-dictionary: "british"
// End: 
