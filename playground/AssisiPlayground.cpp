/*

 */

#include "AssisiPlayground.h"

namespace Enki
{
    AssisiPlayground::AssisiPlayground(World *world, QWidget *parent) :
		ViewerWidget(world, parent)
	{

	}
    
    /* virtual */ 
    void AssisiPlayground::renderObjectHook(PhysicalObject *object)
    {
        // Override the default direction arrow drawn over robots.
        if (dynamic_cast<Robot*>(object))
        {
            glColor3d(0, 0, 0);
            glBegin(GL_TRIANGLES);
            glVertex3d(0.4, 0, object->getHeight() + 0.01);
            glVertex3d(-0.4, 0.2, object->getHeight() + 0.01);
            glVertex3d(-0.4, -0.2, object->getHeight() + 0.01);
            glEnd();
        }
    }


   	/* virtual */
    void AssisiPlayground::sceneCompletedHook()
	{
		glColor3d(0,0,0);
       	renderText(10, height()-50, tr("rotate camera by moving mouse while pressing ctrl+left mouse button"));
		renderText(10, height()-30, tr("move camera on x/y by moving mouse while pressing ctrl+shift+left mouse button"));
		renderText(10, height()-10, tr("move camera on z by moving mouse while pressing ctrl+shift+right mouse button"));
	}

}
