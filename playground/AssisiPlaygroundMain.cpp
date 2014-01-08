#include <QApplication>

#include "WorldExt.h"
#include "AssisiPlayground.h"
#include "handlers/EPuckHandler.h"
#include "handlers/CasuHandler.h"
#include "robots/Casu.h"

using namespace std;
using namespace Enki;

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	
	// Create the world and the viewer
    double r = 120; // World radius (in cm?)
    string pub_address("tcp://127.0.0.1:5555"); 
    string sub_address("tcp://127.0.0.1:5556");
    WorldExt world(r, pub_address, sub_address);

    // Add handlers
    EPuckHandler *eh = new EPuckHandler();
    world.addHandler("EPuck", eh);

    CasuHandler *ch = new CasuHandler();
    world.addHandler("Casu", ch);

    EnkiPlayground viewer(&world);	
	viewer.show();
	
	return app.exec();
}

