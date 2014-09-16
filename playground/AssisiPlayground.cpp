/*

 */

#include <iostream>

#include "AssisiPlayground.h"

namespace Enki
{
	AssisiPlayground::AssisiPlayground (ExtendedWorld *world, WorldHeat *worldHeat, double maxHeat, double maxVibration, QWidget *parent) :
		ViewerWidget(world, parent),
		extendedWorld (world),
		worldHeat (worldHeat),
		maxHeat (maxHeat),
		maxVibration (10),
		layerToDraw (NONE),
		transparency (0.5),
		useGradient (false),
		dataSize (ceil (2 * world->r / worldHeat->gridScale), ceil (2 * world->r / worldHeat->gridScale)),
		dataColour (dataSize.x, std::vector<std::vector<float> > (dataSize.y, std::vector<float> (3, 0) ) ),
		showHelp (true),
		dataLayerZ (5)
	{
		//ViewerWidget::pos = QPointF(-world->w*5, -world->h * 2);
		ViewerWidget::pitch = M_PI/2;
		ViewerWidget::altitude = 80;
		std::cout << "dataSize: " << dataSize << "\n";
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
}

using namespace Enki;

/* virtual */
void AssisiPlayground::sceneCompletedHook()
{
	glDisable (GL_LIGHTING);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix ();
	glTranslated (-this->world->r + this->worldHeat->gridScale, -this->world->r + this->worldHeat->gridScale, dataLayerZ);
	switch (this->layerToDraw) {
	case HEAT:
		drawHeatLegend ();
		if (this->useGradient)
		 	drawHeatLayer_Gradient ();
		else
		 	drawHeatLayer_Chequerboard ();
		break;
	case ELECTRIC_FIELD:
		// drawElectricFieldLayer ();
		break;
	case LIGHT:
		// if (this->useGradient)
		// 	drawLightLayer_Gradient ();
		// else
		// 	drawLightLayer_Chequerboard ();
		break;
	case VIBRATION:
		if (this->useGradient)
			drawVibrationLayer_Gradient ();
		else
			drawVibrationLayer_Chequerboard ();
		break;
	case NONE:
		break;
	default:
		break ;
	}
	glPopMatrix ();
	glDisable (GL_BLEND);
	glEnable (GL_LIGHTING);
	if (this->showHelp) {
		glColor3d(0,0,0);
		renderText (10, height () - 90, tr ("press F1 to toggle this help") );
		renderText (10, height () - 70, tr ("press H to show heat     press V to show vibration") );
		renderText(10, height()-50, tr("rotate camera by moving mouse while pressing ctrl+left mouse button"));
		renderText(10, height()-30, tr("move camera on x/y by moving mouse while pressing ctrl+shift+left mouse button"));
		renderText(10, height()-10, tr("move camera on z by moving mouse while pressing ctrl+shift+right mouse button"));
	}
}

void AssisiPlayground::drawHeatLegend ()
{
	char label[100];
	int i;

	glMatrixMode(GL_PROJECTION);
	glPushMatrix ();
	glLoadIdentity ();
	glOrtho (0, this->width (), 0, this->height (), -1.0f, 1.0f);

	glMatrixMode (GL_MODELVIEW);
	glPushMatrix ();
	glLoadIdentity ();
	glPushAttrib (GL_DEPTH_TEST);
	glDisable (GL_DEPTH_TEST);

	glTranslated (1, this->height () - 12 * 22, 0);
	for (i = -10; i <= 10; i++) {
		double heat = this->worldHeat->normalHeat + i * (this->maxHeat - this->worldHeat->normalHeat) / 10;
		sprintf (label, "%4.1f", heat);
		glColor3f (0, 0, 0);
		renderText (12, (11 - i) * 12, label);
		glTranslated (0, 12, 0);
		if (i < 0) {
			glColor3f (0, 0, -i / 10.0);
		}
		else {
			glColor3f (i / 10.0, 0, 0);
		}
		glBegin (GL_QUADS); {
			glVertex2f ( 0,  0);
			glVertex2f (10,  0);
			glVertex2f (10, 10);
			glVertex2f ( 0, 10);
		} glEnd ();
	}

	glPopAttrib ();
	glMatrixMode (GL_PROJECTION);
	glPopMatrix ();
	glMatrixMode (GL_MODELVIEW);
	glPopMatrix ();
}

void AssisiPlayground::drawHeatLayer_Gradient ()
{
	setDataToHeat ();
	drawDataAsGradient ();
}

void AssisiPlayground::drawHeatLayer_Chequerboard ()
{
	setDataToHeat ();
	drawDataAsCheckerBoard ();
}

void AssisiPlayground::drawVibrationLayer_Gradient ()
{
	setDataToVibration ();
	drawDataAsGradient ();
}

void AssisiPlayground::drawVibrationLayer_Chequerboard ()
{
	setDataToVibration ();
	drawDataAsCheckerBoard ();
}

void AssisiPlayground::setDataToHeat ()
{
	Vector pos, where;
	where.x = -this->world->r + this->worldHeat->gridScale;
	for (pos.x = 0; pos.x < this->dataSize.x; pos.x++) {
		where.y = -this->world->r + this->worldHeat->gridScale;
		for (pos.y = 0; pos.y < this->dataSize.y; pos.y++) {
			double heat = this->worldHeat->getHeatAt (where);
			double colour = std::max (-1.0, std::min ((heat - this->worldHeat->normalHeat) / this->maxHeat, 1.0));
			std::vector<float> &dc = this->dataColour [pos.x][pos.y];
			if (colour < 0) {
				dc [0] = 0;
				dc [1] = 0;
				dc [2] = -colour;
			}
			else {
				dc [0] = colour;
				dc [1] = 0;
				dc [2] = 0;
			}
			where.y += this->worldHeat->gridScale;
		}
		where.x += this->worldHeat->gridScale;
	}
}

void AssisiPlayground::setDataToVibration ()
{
	double time = this->extendedWorld->getAbsoluteTime ();
	Vector pos, where;
	where.x = -this->world->r + this->worldHeat->gridScale;
	for (pos.x = 0; pos.x < this->dataSize.x; pos.x++) {
		where.y = -this->world->r + this->worldHeat->gridScale;
		for (pos.y = 0; pos.y < this->dataSize.y; pos.y++) {
			double vibration = this->extendedWorld->getVibrationAmplitudeAt (where, time);
			double colour = std::min (vibration / this->maxVibration, 1.0);
			std::vector<float> &dc = this->dataColour [pos.x][pos.y];
			dc [0] = 0;
			dc [1] = colour;
			dc [2] = 0;
			where.y += this->worldHeat->gridScale;
		}
		where.x += this->worldHeat->gridScale;
	}
}

void AssisiPlayground::drawDataAsGradient ()
{
	glScaled (this->worldHeat->gridScale, this->worldHeat->gridScale, 1);
	Point delta[] = {
		Point (-1, -1),
		Point ( 0, -1),
		Point ( 0,  0),
		Point (-1,  0)
	};
	Point pos;
	for (pos.x = 1; pos.x < this->dataSize.x; pos.x++) {
		for (pos.y = 1; pos.y < this->dataSize.y; pos.y++) {
			glBegin (GL_QUADS); {
				for (int i = 0; i < 4; i++) {
					Point where = pos + delta [i];
					std::vector<float> &dc = this->dataColour [where.x][where.y];
					glColor4f (dc [0], dc [1], dc [2], transparency);
					glVertex2f (where.x, where.y);
				}
			} glEnd ();
		}
	}
}

void AssisiPlayground::drawDataAsCheckerBoard ()
{
	glScaled (this->worldHeat->gridScale, this->worldHeat->gridScale, 1);
	Point delta[] = {
		Point (-0.5, -0.5),
		Point ( 0.5, -0.5),
		Point ( 0.5,  0.5),
		Point (-0.5,  0.5)
	};
	Point pos;
	for (pos.x = 0; pos.x < this->dataSize.x; pos.x++) {
		for (pos.y = 0; pos.y < this->dataSize.y; pos.y++) {
			std::vector<float> &dc = this->dataColour [pos.x][pos.y];
			glColor4f (dc [0], dc [1], dc [2], transparency);
			glBegin (GL_QUADS); {
				for (int i = 0; i < 4; i++) {
					Point where = pos + delta [i];
					glVertex2f (where.x, where.y);
				}
			} glEnd ();
		}
	}
}

void AssisiPlayground::keyPressEvent (QKeyEvent *event)
{
	switch (event->key()) {
	case Qt::Key_PageUp:
		dataLayerZ = std::min (11, dataLayerZ + 1);
		updateGL ();
		break;
	case Qt::Key_PageDown:
		dataLayerZ = std::max (1, dataLayerZ - 1);
		updateGL ();
		break;
	case Qt::Key_Plus:
		transparency = std::min (1.0, transparency + 0.1);
		qDebug () << "Transparency: " << transparency;
		updateGL ();
		break;
	case Qt::Key_Minus:
		transparency = std::max (0.1, transparency - 0.1);
		qDebug () << "Transparency: " << transparency;
		updateGL ();
		break;
	case Qt::Key_G:
		this->useGradient = (this->useGradient ? false : true);
		qDebug () << "Use gradient: " << this->useGradient;
		updateGL ();
		break;
	case Qt::Key_H:
		qDebug () << "Switching heat";
		this->layerToDraw = (this->layerToDraw == HEAT ? NONE : HEAT);
		updateGL ();
		break;
	case Qt::Key_E:
		qDebug () << "Switching electric";
		this->layerToDraw = (this->layerToDraw == ELECTRIC_FIELD ? NONE : ELECTRIC_FIELD);
		updateGL ();
		break;
	case Qt::Key_L:
		qDebug () << "Switching light";
		this->layerToDraw = (this->layerToDraw == LIGHT ? NONE : LIGHT);
		updateGL ();
		break;
	case Qt::Key_V:
		qDebug () << "Switching vibration";
		this->layerToDraw = (this->layerToDraw == VIBRATION ? NONE : VIBRATION);
		updateGL ();
		break;
	case Qt::Key_F1:
		qDebug () << "Toggle help";
		this->showHelp = !this->showHelp;
		updateGL ();
		break;
	default:
		QGLWidget::keyPressEvent(event);
	}
}

