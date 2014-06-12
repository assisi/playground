/*

 */

#include <iostream>

#include "AssisiPlayground.h"

static const int DATA_Z_LAYER = 11;

namespace Enki
{
	AssisiPlayground::AssisiPlayground (ExtendedWorld *world, WorldHeat *worldHeat, QWidget *parent) :
		ViewerWidget(world, parent),
		extendedWorld (world),
		worldHeat (worldHeat),
		maxHeat (40),
		maxVibration (10),
		layerToDraw (NONE),
		transparency (0.5),
		useGradient (false),
		dataSize (ceil (2 * world->r / worldHeat->gridScale), ceil (2 * world->r / worldHeat->gridScale)),
		dataColour (dataSize.x, std::vector<std::vector<float> > (dataSize.y, std::vector<float> (3, 0) ) ),
		showHelp (true)
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
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix ();
	glTranslated (-this->world->r, -this->world->r, DATA_Z_LAYER);
	switch (this->layerToDraw) {
	case HEAT:
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
	if (this->showHelp) {
		glColor3d(0,0,0);
		renderText (10, height () - 90, tr ("press F1 to toggle this help") );
		renderText (10, height () - 70, tr ("press H to show heat     press V to show vibration") );
		renderText(10, height()-50, tr("rotate camera by moving mouse while pressing ctrl+left mouse button"));
		renderText(10, height()-30, tr("move camera on x/y by moving mouse while pressing ctrl+shift+left mouse button"));
		renderText(10, height()-10, tr("move camera on z by moving mouse while pressing ctrl+shift+right mouse button"));
	}
}


/*

void AssisiPlayground::drawElectricFieldLayer ()
{
	Point delta[] = {
		Point ( 0,  0),
		Point (-1,  0),
		Point (-1, -1),
		Point ( 0, -1)
	};
	glPushMatrix ();
	glTranslated (-this->world->r, -this->world->r, 0);
	glScaled (this->worldElectricField->gridScale, this->worldElectricField->gridScale, 1);
	Vector pos;
	pos.x = 1;
	double radius2 = this->worldElectricField->size.x * this->worldElectricField->size.x / 4;
	do {
		pos.y = 1;
		do {
			if ((pos - this->worldElectricField->size / 2).norm2 () < radius2) {
				glBegin (GL_QUADS); {
					for (int i = 0; i < 4; i++) {
						Point where = pos + delta [i];
						double value = this->worldElectricField->getElectricFieldAt (where.x, where.y).value;
						double colour = std::max (-1.0, std::min (value / this->maxElectricField, 1.0));
						if (colour < 0) {
							glColor4f (0, 0, -colour, transparency);
						}
						else {
							glColor4f (colour, 0, 0, transparency);
						}
						glVertex2f (where.x, where.y);
					}
				} glEnd ();
			}
			pos.y++;
		} while (pos.y < this->worldElectricField->size.y);
		pos.x++;
	} while (pos.x < this->worldElectricField->size.x);
	glPopMatrix ();
}

void AssisiPlayground::drawLightLayer_Gradient ()
{
	Point delta[] = {
		Point (-1, -1),
		Point ( 0, -1),
		Point ( 0,  0),
		Point (-1,  0)
	};
	glPushMatrix ();
	glTranslated (this->worldLight->origin.x, this->worldLight->origin.y, 0);
	glScaled (this->worldLight->gridScale, this->worldLight->gridScale, 1);
	Vector pos;
	pos.x = 1;
	double radius2 = this->worldLight->size.x * this->worldLight->size.x / 4;
	do {
		pos.y = 1;
		do {
			if ((pos - this->worldLight->size / 2).norm2 () < radius2) {
				glBegin (GL_QUADS); {
					for (int idx = 0; idx < 4; idx++) {
						Point where = pos + delta [idx];
						double value = this->worldLight->getIntensityAt (where.x, where.y);
						double colour = std::min (value / this->maxLightIntensity, 1.0);
						glColor4f (colour, colour, colour, transparency);
						glVertex2f (where.x, where.y);
					}
				} glEnd ();
			}
			pos.y += 1;
			} while (pos.y < this->worldLight->size.y);
		pos.x += 1;
	} while (pos.x < this->worldLight->size.x);
	glPopMatrix ();
}

void AssisiPlayground::drawLightLayer_Chequerboard ()
{
	Point delta[] = {
		Point (-0.5, -0.5),
		Point ( 0.5, -0.5),
		Point ( 0.5,  0.5),
		Point (-0.5,  0.5)
	};
	glPushMatrix ();
	glTranslated (this->worldLight->origin.x, this->worldLight->origin.y, 0);
	glScaled (this->worldLight->gridScale, this->worldLight->gridScale, 1);
	Vector pos;
	pos.x = 0;
	double radius2 = this->worldLight->size.x * this->worldLight->size.x / 4;
	do {
		pos.y = 0;
		do {
			if ((pos - this->worldLight->size / 2).norm2 () < radius2) {
				double value = this->worldLight->getIntensityAt (pos.x, pos.y);
				double colour = std::min (value / this->maxLightIntensity, 1.0);
				glColor4f (colour, colour, colour, transparency);
				glBegin (GL_QUADS); {
					for (int idx = 0; idx < 4; idx++) {
						Point where = pos + delta [idx];
						glVertex2f (where.x, where.y);
					}
				} glEnd ();
			}
			pos.y += 1;
			} while (pos.y < this->worldLight->size.y);
		pos.x += 1;
	} while (pos.x < this->worldLight->size.x);
	glPopMatrix ();
}

*/



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
	Vector pos, where;
	where.x = -this->world->r + this->worldHeat->gridScale;
	for (pos.x = 0; pos.x < this->dataSize.x; pos.x++) {
		where.y = -this->world->r + this->worldHeat->gridScale;
		for (pos.y = 0; pos.y < this->dataSize.y; pos.y++) {
			double vibration = this->extendedWorld->getVibrationIntensityAt (where);
			double colour = std::max (vibration / this->maxVibration, 1.0);
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
	glTranslated (this->worldHeat->gridScale / 2.0, this->worldHeat->gridScale / 2.0, 0);
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
	glTranslated (this->worldHeat->gridScale, this->worldHeat->gridScale, 0);
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

