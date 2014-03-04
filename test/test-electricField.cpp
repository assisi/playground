/* 
 * File:   test-electricField.cpp
 * Author: pedro
 *
 * Created on 3 de Março de 2014, 22:56
 */

#include <cstdlib>
#include <fstream>
#include <string.h>

#include <GL/glut.h>


#include <enki/PhysicalEngine.h>
#include <enki/Geometry.h>
#include "../interactions/PointElectricFieldActuator.h"

using namespace std;
using namespace Enki;

static const double RADIUS = 0.01;

class ElectricRobot :
public Robot
{
public:
	PointElectricFieldActuator *electricFieldActuator;
	ElectricRobot (const Vector &position, double electricCharge);
			  
};

ElectricRobot::ElectricRobot (const Vector& position, double electricCharge)
{
	this->pos = position;
	this->electricFieldActuator = new PointElectricFieldActuator (this, Vector (), electricCharge);
	this->addGlobalInteraction (this->electricFieldActuator);
	PhysicalObject::dryFrictionCoefficient = 1000;
	this->setCylindric (RADIUS, 1, 1);
}

static double WINDOW_WIDTH = 600, WINDOW_HEIGHT = 600;

static World *world;
static double worldWidth, worldHeight;
static double gridScale;
static double maxElectricField;
static vector<ElectricRobot *> robots;
static double deltaTime;
static unsigned int simulationSpeed;

static double currentTime = 0;

void readConfigFile ()
{
	ifstream ifs ("config.txt");
	ifs >> worldWidth >> worldHeight;
	world = new World (worldWidth, worldHeight);
	double maxElectricCharge;
	ifs >> gridScale >> maxElectricCharge;
	maxElectricField = 1.0 / (4 * M_PI * AbstractElectricFieldActuator::PERMITTIVITY) * maxElectricCharge;
	if (gridScale < 1)
		maxElectricField /= gridScale * gridScale;
	int numberElectricRobots, i;
	ifs >> numberElectricRobots;
	for (i = 0; i < numberElectricRobots; i++) {
		Vector location;
		double electricCharge;
		ifs >> location.x >> location.y >> electricCharge;
		ElectricRobot *electricRobot = new ElectricRobot (location, electricCharge);
		robots.push_back (electricRobot);
		world->addObject (electricRobot);
	}
	ifs >> deltaTime >> simulationSpeed;
	ifs.close ();
}

/*
 * Taken from http://www.daniweb.com/software-development/cpp/threads/277785/text-in-opengl
 */
void printText_Viewport (int x, int y, const char *String)
{
	//(x,y) is from the bottom left of the window
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity ();
	glOrtho (0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1.0f, 1.0f);
	glMatrixMode (GL_MODELVIEW);
	glPushMatrix ();
	glLoadIdentity ();
	glPushAttrib (GL_DEPTH_TEST);
	glDisable (GL_DEPTH_TEST);
	glRasterPos2i (x, y);
	int i;
	for (i = 0; i < strlen (String); i++)
		{
			glutBitmapCharacter (GLUT_BITMAP_9_BY_15, String[i]);
		}
	glPopAttrib ();
	glMatrixMode (GL_PROJECTION);
	glPopMatrix ();
	glMatrixMode (GL_MODELVIEW);
	glPopMatrix ();
}

void displayCallBack ()
{
	// clear the window with current clearing color
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// set viewing transformation
	glLoadIdentity ();  // clear MODELVIEW matrix
	Vector pos;
	pos.x = 0;
	do {
		pos.y = 0;
		do {
			double sumValue = 0;
			Point sumDirection (0, 0);
			for (size_t i = 0; i < robots.size (); i++) {
				double value;
				Point direction;
				robots [i]->electricFieldActuator->measureAt (pos, &value, &direction);
				sumValue += value;
				sumDirection += direction;
			}
			double colour = std::max (-1.0, std::min (sumValue / maxElectricField, 1.0));
			if (colour < 0) {
				glColor3f (0, 0, -colour);
			}
			else {
				glColor3f (colour, 0, 0);
			}
			glBegin (GL_QUADS); {
				glVertex2f (pos.x - gridScale / 2, pos.y - gridScale / 2);
				glVertex2f (pos.x + gridScale / 2, pos.y - gridScale / 2);
				glVertex2f (pos.x + gridScale / 2, pos.y + gridScale / 2);
				glVertex2f (pos.x - gridScale / 2, pos.y + gridScale / 2);
			} glEnd ();
			pos.y += gridScale;
		} while (pos.y < world->h);
		pos.x += gridScale;
	} while (pos.x < world->w);

	glColor3f (1, 1, 1);
	for (size_t i = 0; i < robots.size (); i++) {
		glBegin (GL_LINE_LOOP); {
			for (double t = 0; t < 2 * M_PI; t += M_PI / 6) //<-- Change this Value
				glVertex3f (robots [i]->pos.x + cos (t) * RADIUS, robots [i]->pos.y + sin (t) * RADIUS, 0.0);
		} glEnd ();
		if (robots [i]->electricFieldActuator->switchedOn) {
		}
		else {
			glBegin (GL_LINE_LOOP); {
				glVertex2f (robots [i]->pos.x + RADIUS, robots [i]->pos.y + RADIUS);
				glVertex2f (robots [i]->pos.x - RADIUS, robots [i]->pos.y - RADIUS);
			} glEnd ();
			glBegin (GL_LINE_LOOP); {
				glVertex2f (robots [i]->pos.x - RADIUS, robots [i]->pos.y + RADIUS);
				glVertex2f (robots [i]->pos.x + RADIUS, robots [i]->pos.y - RADIUS);
			} glEnd ();
		}
	}

	glFlush ();
	glutSwapBuffers ();
}

void reshapeCallBack (GLsizei windowWidth, GLsizei windowHeight)
{
	WINDOW_WIDTH = windowWidth;
	WINDOW_HEIGHT = windowHeight;
	glViewport (0, 0, windowWidth, windowHeight);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glOrtho (0, world->w, 0, world->h, 1, -1);

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
}

void keyboardCallBack (unsigned char key, int x, int y)
{
	switch (key) {
	case '+':
		deltaTime += 0.1;
		break;
	case '-':
		if (deltaTime > 0.1) {
			deltaTime -= 0.1;
		}
		else {
			deltaTime = 0;
		}
		break;
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
		robots [key - '1']->electricFieldActuator->toogle ();
		glutPostRedisplay ();
		break;
	}
}

/*
 * 
 */
int main(int argc, char** argv)
{
	readConfigFile ();
	world->step (deltaTime);
	currentTime += deltaTime;

   /* Initialize GLUT */

   glutInit( &argc, argv );

   /* window in the screen */

   glutInitWindowSize( 800, 800 );
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowPosition( 100, 100 );

   glutCreateWindow( "electric simulation" );

   /*  "callback function" */
   glutDisplayFunc (displayCallBack);
	glutReshapeFunc (reshapeCallBack);
	glutKeyboardFunc (keyboardCallBack);

   /* waiting events */
   glutMainLoop();
	return 0;
}

