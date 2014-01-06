/* Demo app implementing a simple wander behavior with an e-puck.

   Spawning and control is implemented through the ZMQ message-based
   interface.
 */

#include <iostream>

#include "zmq.hpp"
#include "zmq_helpers.h"

#include "dev_msgs.pb.h"
#include "sim_msgs.pb.h"

#include <unistd.h> // for usleep

using namespace std;
using namespace zmq;
using namespace AssisiMsg;

void send_speed_cmd(string name, socket_t& sock, double leftVel, double rightVel)
{
    message_t msg;
    str_to_msg(name, msg);
    sock.send(msg, ZMQ_SNDMORE);
    str_to_msg("base", msg);
    sock.send(msg, ZMQ_SNDMORE);
    str_to_msg("vel", msg);
    sock.send(msg, ZMQ_SNDMORE);
    DiffDrive drive;
    drive.set_vel_left(leftVel);
    drive.set_vel_right(rightVel);
    string drive_str;
    drive.SerializeToString(&drive_str);
    str_to_msg(drive_str, msg);
    sock.send(msg);
}

int main(int argc, char* argv[])
{
    context_t context(1);
    socket_t pub(context, ZMQ_PUB);
    socket_t sub(context, ZMQ_SUB);

    string pub_addr("tcp://127.0.0.1:5556");
    string sub_addr("tcp://127.0.0.1:5555");
    string name("robby");

    pub.bind(pub_addr.c_str());
    sub.connect(sub_addr.c_str());
    sub.setsockopt(ZMQ_SUBSCRIBE, name.c_str(), name.size());
    usleep(1000000);

    // Spawn robot
    cout << "Spawnning " << name << endl;
    message_t msg;
    str_to_msg("sim", msg);
    pub.send(msg, ZMQ_SNDMORE);
    str_to_msg("spawn", msg);
    pub.send(msg, ZMQ_SNDMORE);
    str_to_msg("EPuck", msg);
    pub.send(msg, ZMQ_SNDMORE);
    Spawn spawn_msg;
    spawn_msg.set_name(name);
    spawn_msg.set_type("EPuck");
    spawn_msg.mutable_pose()->mutable_position()->set_x(0.0);
    spawn_msg.mutable_pose()->mutable_position()->set_y(0.0);
    spawn_msg.mutable_pose()->mutable_orientation()->set_z(0.0);
    string msg_str;
    spawn_msg.SerializeToString(&msg_str);
    str_to_msg(msg_str, msg);
    pub.send(msg);
    
    double vel_l = 5;
    double vel_r = 5;
     
    //  Read sensor updates
    //  Set wheel velocities to avoid obstacles
    while (1)
    {
        message_t msg;
        sub.recv(&msg);  // Receiving robot name
        string msg_str(msg_to_str(msg));
        sub.recv(&msg);  // Receiving sensor type
        msg_str = msg_to_str(msg);
        if (msg_str != "ir")
        {
            cout << "Unexpected sensor type, aborting!" << endl;
            return 1;
        }
        sub.recv(&msg); // Receiving reading type
        // We're skipping the sensor type check!
        sub.recv(&msg); // Receiving the actual message
        msg_str = msg_to_str(msg);
        RangeArray ranges;
        if (!ranges.ParseFromString(msg_to_str(msg)))
        {
            cout << "Can't parse range message, aborting!" << endl;
            return 1;
        }

        /* The control algorithm 
           Don't try to control a real robot this way :)
           TODO: Improve this a bit to get better behavior
                 in a round arena
        */
     
        // Check right sensors
        for (int j = 0; j < 4; j++)
        {
            if (ranges.range(j) < 0.02)
            {
                vel_l = 2.5;
                break;
            }
        }

        // Check left sensors
        for (int j = 4; j < 8; j++)
        {
            if (ranges.range(j) < 0.02)
            {
                vel_r = 2.5;
                break;
            }
        }

        // Check front sensors
        if ((ranges.range(0) < 0.02) || (ranges.range(7) < 0.02))
        {
            vel_l = - vel_l;
            vel_r = - vel_r;
        }
        
        // Resume forward motion when it's safe
        if ((vel_l < 0) && (vel_r < 0) 
            && (ranges.range(0) > 0.04)
            && (ranges.range(7) > 0.04))
        {
            vel_l = - vel_l;
            vel_r = - vel_r;
        }
        send_speed_cmd(name, pub, vel_l, vel_r);
    }

    return 0;
}
