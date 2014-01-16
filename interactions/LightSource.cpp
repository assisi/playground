/* 
 * File:   LightSource.cpp
 * Author: Pedro Mariano
 *
 * Created on 7 de Janeiro de 2014, 12:14
 */

#include "LightSource.h"

using namespace Enki;

LightSource::
LightSource (double range, Robot* owner, Vector relativePosition):
	LocalInteraction (range, owner),
	Component (owner, relativePosition)
{
}
