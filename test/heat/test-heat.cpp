
#include <GL/glut.h>

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string.h> 

#include "../../extensions/ExtendedWorld.h"
#include "../../extensions/ExtendedRobot.h"
#include "../../interactions/HeatActuatorMesh.h"
#include "../../interactions/WorldHeat.h"

using namespace Enki;
using namespace std;

#define RADIUS 0.01

class HeatingRobot:
	public ExtendedRobot
{
public:
	HeatActuatorMesh *heatActuator;
public:
	HeatingRobot (Vector location, double heat)
	{
		this->pos = location;
		this->heatActuator = new HeatActuatorMesh (this, Vector (0, 0), heat, 1.2, 1.6, 10);
		this->addPhysicInteraction (this->heatActuator);

		PhysicalObject::dryFrictionCoefficient = 1000;
		this->setCylindric (RADIUS, 1, 1);
	}
	void toogleHeat ()
	{
		this->heatActuator->toogleSwitchedOn ();
	}
};

double WINDOW_WIDTH = 600, WINDOW_HEIGHT = 600;

double width, height;
ExtendedWorld *world;
double normalHeat, gridScale, borderSize;
WorldHeat *worldHeat;
vector<HeatingRobot *> heatingRobots;
double deltaTime;
unsigned int simulationSpeed = 100;
double maxHeat;

double currentTime = 0;
bool displayGradient = true;
bool blackFlag = false;

void readConfigFile ()
{
	ifstream ifs ("config-heat.txt");
	ifs >> width >> height;
	world = new ExtendedWorld (width, height);
	ifs >> normalHeat >> gridScale >> borderSize;
	worldHeat = new WorldHeat (normalHeat, gridScale, borderSize);
	world->addPhysicSimulation (worldHeat);
	int numberHeatRobots, i;
	ifs >> numberHeatRobots;
	for (i = 0; i < numberHeatRobots; i++) {
		Vector location;
		double heat;
		ifs >> location.x >> location.y >> heat;
		HeatingRobot *heatingRobot = new HeatingRobot (location, heat);
		heatingRobots.push_back (heatingRobot);
		world->addObject (heatingRobot);
	}
	ifs >> deltaTime >> simulationSpeed;
	ifs >> maxHeat;
	ifs.close ();
}

// void usage (int error)
// {
// 	fprintf (stderr, "Usage:\ntest-heat WIDTH HEIGHT GRIDSCALE HEATING_ROBOT_1_POS_X HEATING_ROBOT_1_POS_Y HEATING_ROBOT_1_HEAT...\n");
// 	exit (error);
// }


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

void heat2color (double heat)
{
	double colour = std::max (-1.0, std::min ((heat - normalHeat) / maxHeat, 1.0));
	if (blackFlag) {
		if (colour < 0) {
			glColor3f (0, 0, -colour);
		}
		else {
			glColor3f (colour, 0, 0);
		}
	}
	else {
		if (colour < 0) {
			glColor3f (1 + colour, 1 + colour, 1);
		}
		else {
			glColor3f (1, 1 - colour, 1 - colour);
		}
	}
}

void displayCallBack ()
{
	// clear the window with current clearing color
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// set viewing transformation
	glLoadIdentity ();  // clear MODELVIEW matrix
	Vector pos;
	pos.x = gridScale;
	do {
		pos.y = gridScale;
		do {
			if (displayGradient) {
				Point delta[] = {
					Point (0, 0),
					Point (-gridScale, 0),
					Point (-gridScale, -gridScale),
					Point (0, -gridScale)
				};
				glBegin (GL_QUADS); {
					for (int i = 0; i < 4; i++) {
						Point where = pos + delta [i];
						heat2color (worldHeat->getHeatAt (where));
						glVertex2f (where.x, where.y);
					}
				} glEnd ();
			}
			else {
				heat2color (worldHeat->getHeatAt (pos));
				glBegin (GL_QUADS); {
					glVertex2f (pos.x - gridScale / 2, pos.y - gridScale / 2);
					glVertex2f (pos.x + gridScale / 2, pos.y - gridScale / 2);
					glVertex2f (pos.x + gridScale / 2, pos.y + gridScale / 2);
					glVertex2f (pos.x - gridScale / 2, pos.y + gridScale / 2);
				} glEnd ();
			}
			pos.y += gridScale;
		} while (pos.y < world->h);
		pos.x += gridScale;
	} while (pos.x < world->w);

	if (blackFlag) {
		glColor3f (1, 1, 1);
	}
	else {
		glColor3f (0, 0, 0);
	}
	for (size_t i = 0; i < heatingRobots.size (); i++) {
		glBegin (GL_LINE_LOOP); {
			for (double t = 0; t < 2 * M_PI; t += M_PI / 6) { //<-- Change this Value
				glVertex3f (heatingRobots [i]->pos.x + cos (t) * RADIUS, heatingRobots [i]->pos.y + sin (t) * RADIUS, 0.0);
			}
		} glEnd ();
		if (heatingRobots [i]->heatActuator->isSwitchedOn ()) {
		}
		else {
			glBegin (GL_LINE_LOOP); {
				glVertex2f (heatingRobots [i]->pos.x + RADIUS, heatingRobots [i]->pos.y + RADIUS);
				glVertex2f (heatingRobots [i]->pos.x - RADIUS, heatingRobots [i]->pos.y - RADIUS);
			} glEnd ();
			glBegin (GL_LINE_LOOP); {
				glVertex2f (heatingRobots [i]->pos.x - RADIUS, heatingRobots [i]->pos.y + RADIUS);
				glVertex2f (heatingRobots [i]->pos.x + RADIUS, heatingRobots [i]->pos.y - RADIUS);
			} glEnd ();
		}
	}

	char msg [1000];
	sprintf (msg, "time %.1f", currentTime);
	printText_Viewport (10, 10, msg);
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

void timerCallBack (int value)
{
	world->step (deltaTime);
	currentTime += deltaTime;
	// cerr << currentTime;
	// worldHeat->dumpState (cerr);

	glutPostRedisplay ();
	glutTimerFunc (simulationSpeed, timerCallBack, 0);
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
	case 'g':
		displayGradient = !displayGradient;
		break;
	case 'b':
		blackFlag = !blackFlag;
		break;
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
		heatingRobots [key - '1']->toogleHeat ();
		break;
	}
}

int main (int argc, char *argv[])
{
	readConfigFile ();

	if (argc == 4 && strcmp (argv [1], "dump-state") == 0) {
		ofstream ofs (argv [2]);
		int iterations = atoi (argv [3]);
		while (iterations > 0) {
			world->step (deltaTime);
			currentTime += deltaTime;
			ofs << currentTime;
			worldHeat->dumpState (ofs);
			iterations--;
		}
		ofs.close ();
	}
	else {


	// if (argc == 2 && strcmp (argv [1], "steady-state") == 0)  {
	// 	unsigned long iterations = 0;
	// 	do {
	// 		world->step (deltaTime);
	// 		iterations++;
	// 	} while (worldHeat->getTimeState () != WorldHeat::USING_CACHED_VALUES);
	// 	cout << "First steady state reached after " << iterations << " iterations\n"; 
	// 	heatingRobots [0]->toogleHeat ();
	// 	world->step (deltaTime);
	// 	iterations = 0;
	// 	do {
	// 		world->step (deltaTime);
	// 		iterations++;
	// 	} while (worldHeat->getTimeState () != WorldHeat::USING_CACHED_VALUES);
	// 	cout << "Second steady state reached after " << iterations << " iterations\n"; 
	// }

	   /* Initialize GLUT */

		glutInit( &argc, argv );

		/* window in the screen */

		glutInitWindowSize( 800, 800 );
		glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
		glutInitWindowPosition( 100, 100 );

		glutCreateWindow( "heat simulation" );

		/*  "callback function" */
		glutDisplayFunc ( displayCallBack );
		glutReshapeFunc (reshapeCallBack);
		glutKeyboardFunc (keyboardCallBack);
		glutTimerFunc (simulationSpeed, timerCallBack, 0);

		/* waiting events */
		glutMainLoop();
	}

	return 0;
}
