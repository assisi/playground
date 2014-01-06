/*
    Enki - a fast 2D robot simulator
    Copyright (C) 1999-2013 Stephane Magnenat <stephane at magnenat dot net>
    Copyright (C) 2004-2005 Markus Waibel <markus dot waibel at epfl dot ch>
    Copyright (c) 2004-2005 Antoine Beyeler <abeyeler at ab-ware dot com>
    Copyright (C) 2005-2006 Laboratory of Intelligent Systems, EPFL, Lausanne
    Copyright (C) 2006-2008 Laboratory of Robotics Systems, EPFL, Lausanne
    See AUTHORS for details

    This program is free software; the authors of any publication 
    arising from research using this software are asked to add the 
    following reference:
    Enki - a fast 2D robot simulator
    http://home.gna.org/enki
    Stephane Magnenat <stephane at magnenat dot net>,
    Markus Waibel <markus dot waibel at epfl dot ch>
    Laboratory of Intelligent Systems, EPFL, Lausanne.

    You can redistribute this program and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/


#include <QApplication>

#include "WorldExt.h"
#include "AssisiPlayground.h"
#include "handlers/EPuckHandler.h"
#include "robots/Casu.h"

using namespace std;
using namespace Enki;

// http://qtnode.net/wiki?title=Qt_with_cmake
int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	
	// Create the world and the viewer
    double r = 120; // World radius (in cm?)
    string pub_address("tcp://127.0.0.1:5555"); 
    string sub_address("tcp://127.0.0.1:5556");
    WorldExt world(r, pub_address, sub_address);

    // Add a Casu to the world.
    Casu *casu = new Casu();
    world.addObject(casu);

    Casu *casu2 = new Casu();
    casu2->pos = Point(10,10);
    casu2->angle = 1.5;
    casu2->setCylindric(2,2,1000);
    casu2->setColor(Color(0.8,0,0,0.01));
    world.addObject(casu2);

    // Add handlers
    EPuckHandler *eh = new EPuckHandler();
    world.addHandler("EPuck", eh);

    EnkiPlayground viewer(&world);	
	viewer.show();
	
	return app.exec();
}

