/* Test the ZMQ extensions.

 */

#include "WorldExt.h"
#include "handlers/PhysicalObjectHandler.h"
#include "handlers/CasuHandler.h"
#include "handlers/BeeHandler.h"

#include <unistd.h> // for usleep

using std::string;
using namespace Enki;

int main(int argc, char *argv[])
{
    double r = 120; // World radius (in cm?)
    string pub_address("tcp://*:5555"); 
    string sub_address("tcp://*:5556");
    WorldExt world(r, pub_address, sub_address);

    // Add handlers
    CasuHandler *ch = new CasuHandler();
    world.addHandler("Casu", ch);

    BeeHandler *bh = new BeeHandler();
    world.addHandler("Bee", bh);

    PhysicalObjectHandler *ph = new PhysicalObjectHandler();
    world.addHandler("Physical", ph);    
    
    while (1)
    {
        world.step(0.1, 10);
        usleep(10000);
    }

    return 0;
}
