#include <QApplication>
#include <QImage>

#include "WorldExt.h"
#include "AssisiPlayground.h"
#include "handlers/PhysicalObjectHandler.h"
#include "handlers/EPuckHandler.h"
#include "handlers/CasuHandler.h"
#include "robots/Casu.h"

#include <iostream>

using namespace std;
using namespace Enki;

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	
	// Create the world and the viewer
    double r = 40; // World radius (in cm?)
    string pub_address("tcp://127.0.0.1:5555"); 
    string sub_address("tcp://127.0.0.1:5556");
    
    //QImage texture("playground/world.png");
    QImage texture(QString(":/textures/ground_grayscale.png"));
    texture = texture.convertToFormat(QImage::Format_ARGB32);    texture.invertPixels(QImage::InvertRgba);
    WorldExt world(r, pub_address, sub_address,
                   Color::gray, texture.width(),
                   texture.height(), (uint32_t*) texture.bits() );

    // Add handlers
    EPuckHandler *eh = new EPuckHandler();
    world.addHandler("EPuck", eh);

    CasuHandler *ch = new CasuHandler();
    world.addHandler("Casu", ch);

    PhysicalObjectHandler *ph = new PhysicalObjectHandler();
    world.addHandler("Physical", ph);

    EnkiPlayground viewer(&world);	
	viewer.show();
	
	return app.exec();
    
}

