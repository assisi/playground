/*! \file   playground_ext.h
    \brief  A simple enki playground with an external interface.

 */

#ifndef ASSISI_PLAYGROUND_H
#define ASSISI_PLAYGROUND_H

// Enki includes
#include <Viewer.h>
#include <PhysicalEngine.h>
#include <robots/e-puck/EPuck.h>

// Qt includes
//#include <QApplication>
#include <QtGui>


/*!	\file AssisiPlayground.h
	\brief The ASSISI Simulator.
*/

namespace Enki
{

class AssisiPlayground : public ViewerWidget
{

    Q_OBJECT;

public:
    AssisiPlayground(World *world, QWidget *parent = 0);	
    void addEPuck(World *world, Point pos);

	~AssisiPlayground()
	{

	}
	
    // Inherited from viewer 
    virtual void sceneCompletedHook();
    virtual void renderObjectHook(PhysicalObject *object);
};

} // namespace Enki

#endif
