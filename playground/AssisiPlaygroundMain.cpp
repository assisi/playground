#include <QApplication>
#include <QImage>

#include <boost/program_options.hpp>

#include "WorldExt.h"
#include "AssisiPlayground.h"

#include "handlers/PhysicalObjectHandler.h"
#include "handlers/EPuckHandler.h"
#include "handlers/CasuHandler.h"
#include "handlers/BeeHandler.h"


#include <iostream>

using namespace std;
using namespace Enki;

namespace po = boost::program_options;

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	
    // Parse command line options
    po::options_description desc("Recognized options");
    int r;
    desc.add_options()
        ("help", "produce help message")
        ("r", po::value<int>(&r)->default_value(40), "playground radius, in cm");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help"))
    {
        cout << desc << endl;
        return 1;
    }

    
	// Create the world and the viewer
    //double r = 40; // World radius (in cm?)
    //string pub_address("tcp://127.0.0.1:5555"); 
    //string sub_address("tcp://127.0.0.1:5556");
    string pub_address("tcp://*:5555"); 
    string sub_address("tcp://*:5556");

    //QImage texture("playground/world.png");
    QImage texture(QString(":/textures/ground_grayscale.png"));
    texture = texture.convertToFormat(QImage::Format_ARGB32);    texture.invertPixels(QImage::InvertRgba);
    WorldExt world(r, pub_address, sub_address,
                   Color::gray, texture.width(),
                   texture.height(), (uint32_t*) texture.bits() );

    CasuHandler *ch = new CasuHandler();
    world.addHandler("Casu", ch);

    PhysicalObjectHandler *ph = new PhysicalObjectHandler();
    world.addHandler("Physical", ph);

    BeeHandler *bh = new BeeHandler();
    world.addHandler("Bee", bh);

    AssisiPlayground viewer(&world);	
	viewer.show();
	
	return app.exec();
    
}

