/*! \file   playground_ext.h
    \brief  A simple enki playground with an external interface.

 */

#ifndef PLAYGROUND_EXT_H
#define PLAYGROUND_EXT_H

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

class EnkiPlayground : public ViewerWidget
{

    Q_OBJECT;

protected:
	bool subjectiveView;
	QVector<EPuck*> epucks;
	QMap<PhysicalObject*, int> bullets;

public:
    EnkiPlayground(World *world, QWidget *parent = 0);	
    void addEPuck(World *world, Point pos);

	~EnkiPlayground()
	{

	}
	
    // Inherited from viewer 
    virtual void timerEvent(QTimerEvent * event);
    virtual void keyPressEvent ( QKeyEvent * event );	
    virtual void sceneCompletedHook();

};

} // namespace Enki

#endif
