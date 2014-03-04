/* 
 * File:   AbstractElectricFieldActuator.h
 * Author: pedro
 *
 * Created on 3 de Março de 2014, 19:09
 */

#ifndef ABSTRACTELECTRICFIELDACTUATOR_H
#define	ABSTRACTELECTRICFIELDACTUATOR_H

#include <enki/PhysicalEngine.h>
#include "../Component.h"

namespace Enki {

	class AbstractElectricFieldActuator:
		public Component,
		public GlobalInteraction
	{
	public:
		static double PERMITTIVITY;
		bool switchedOn;
	public:
		AbstractElectricFieldActuator (Robot *owner, Vector relativePosition, double relativeOrientation);
		AbstractElectricFieldActuator (const AbstractElectricFieldActuator& orig);
		virtual ~AbstractElectricFieldActuator ();
	public:
		virtual void measureAt (const Point &, double *value, Point *direction) = 0;
		virtual void init (double dt, World *w) = 0;
		void toogle ();
	};
}

#endif	/* ABSTRACTELECTRICFIELDACTUATOR_H */



// Local Variables: 
// mode: c++
// mode: flyspell-prog
// ispell-local-dictionary: "british"
// End: 
