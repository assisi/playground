/*

 */

#include "AssisiPlayground.h"

namespace Enki
{
    EnkiPlayground::EnkiPlayground(World *world, QWidget *parent) :
		ViewerWidget(world, parent),
		subjectiveView(false)
	{
        // Three blue cylinders
		for (double d = 40; d < 60; d += 8)
		{
			PhysicalObject* o = new PhysicalObject;
			o->pos = Point(d, 20);
			o->setCylindric(4, 2, 10);
			o->setColor(Color(0.2, 0.2, 0.6));
			o->dryFrictionCoefficient = 0.;
			world->addObject(o);
		}

	}

    
    void EnkiPlayground::addEPuck(World *world, Point pos)
    {
        EPuck *epuck = new EPuck;
        epuck->pos = pos;
        world->addObject(epuck);
    }
	

    // Inherited from viewer 
    /* virtual */
    void EnkiPlayground::timerEvent(QTimerEvent * event)
	{
		static int fireCounter = 0;
		QMap<PhysicalObject*, int>::iterator i = bullets.begin();
		while (i != bullets.end())
		{
			QMap<PhysicalObject*, int>::iterator oi = i;
			++i;
			if (oi.value())
			{
				oi.value()--;
			}
			else
			{
				PhysicalObject* o = oi.key();
				world->removeObject(o);
				bullets.erase(oi);
				delete o;
			}
		}
		ViewerWidget::timerEvent(event);
	}

	/* virtual */
    void EnkiPlayground::keyPressEvent ( QKeyEvent * event )
	{
		if (event->key() == Qt::Key_C)
		{
			subjectiveView = !subjectiveView;
			if (subjectiveView)
				pitch = M_PI/8;
			event->accept();
		}
		else
			ViewerWidget::keyPressEvent(event);
	}

   	/* virtual */
    void EnkiPlayground::sceneCompletedHook()
	{
		glColor3d(0,0,0);
       	renderText(10, height()-50, tr("rotate camera by moving mouse while pressing ctrl+left mouse button"));
		renderText(10, height()-30, tr("move camera on x/y by moving mouse while pressing ctrl+shift+left mouse button"));
		renderText(10, height()-10, tr("move camera on z by moving mouse while pressing ctrl+shift+right mouse button"));
	}

}
