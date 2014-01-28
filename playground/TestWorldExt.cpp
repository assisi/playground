/* Test the ZMQ extensions.

 */

#include "WorldExt.h"
#include "handlers/EPuckHandler.h"
#include "handlers/CasuHandler.h"
#include "handlers/BeeHandler.h"

#include <unistd.h> // for usleep

using std::string;
using namespace Enki;

int main(int argc, char *argv[])
{
    double r = 120; // World radius (in cm?)
    string pub_address("tcp://127.0.0.1:5555"); 
    string sub_address("tcp://127.0.0.1:5556");
    WorldExt world(r, pub_address, sub_address);

    // Add handlers

    EPuckHandler *eh = new EPuckHandler();
    world.addHandler("EPuck", eh);
    World* wp = &world;

    CasuHandler *ch = new CasuHandler();
    world.addHandler("Casu", ch);

    BeeHandler *bh = new BeeHandler();
    world.addHandler("Bee", bh);
    
    while (1)
    {
        wp->step(10, 20);
        usleep(1000000);
    }

    return 0;
}
