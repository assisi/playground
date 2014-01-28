/*

 */
#include <iostream>

#include <boost/foreach.hpp>

#include <zmq.hpp>
#include "playground/zmq_helpers.hpp"

#include "robots/Bee.h"
#include "handlers/BeeHandler.h"

// Protobuf message headers
#include "base_msgs.pb.h"
#include "dev_msgs.pb.h"
#include "sim_msgs.pb.h"

using namespace zmq;

using std::string;
using std::cerr;
using std::endl;

using namespace AssisiMsg;

namespace Enki
{

    /* virtual */
    string BeeHandler::createObject(const std::string& data, 
                                     World* world)
    {
        string name = "";
        Spawn spawn_msg;     
        assert(spawn_msg.ParseFromString(data));
        if (bees_.count(spawn_msg.name()) < 1)
        {
            name = spawn_msg.name();
            Point pos(spawn_msg.pose().position().x(),
                      spawn_msg.pose().position().y());
            double yaw(spawn_msg.pose().orientation().z());
            bees_[name] = new Bee;
            bees_[name]->pos = pos;
            bees_[name]->angle = yaw;
            world->addObject(bees_[name]);
        }
        else
        {
            cerr << "Bee "<< spawn_msg.name() << " already exists." << endl;
        }
        return name;
    }

// -----------------------------------------------------------------------------

    /* virtual */
    int BeeHandler::handleIncoming(const std::string& name,
                                    const std::string& device,
                                    const std::string& command,
                                    const std::string& data)
    {
        int count = 0;
        if (device == "base")
        {
            if (command == "vel")
            {
                DiffDrive drive;
                assert(drive.ParseFromString(data));
                bees_[name]->leftSpeed = drive.vel_left();
                bees_[name]->rightSpeed = drive.vel_right();
                count++;
            }
            else
            {
                cerr << "Unknown command for " << name << "/" << device << endl;
                return 0;
            }         
        }
        else
        {
            cerr << "Unknown device " << device << endl;
        }
        return count;
    }

// -----------------------------------------------------------------------------

    /* virtual */
    int BeeHandler::sendOutgoing(socket_t& socket)
    {
        int count = 0;
        BOOST_FOREACH(const BeeMap::value_type& ca, bees_)
        {
            std::string data;

            /* Publishing IR readings */
            RangeArray ranges;
            BOOST_FOREACH(IRSensor* ir, ca.second->range_sensors)
            {
                ranges.add_range(ir->getDist());                
            }
            ranges.SerializeToString(&data);
            send_multipart(socket, ca.first, "IR", "Ranges", data);
            count++;

            /* Publish velocities */
            DiffDrive drive;
            drive.set_vel_left(ca.second->leftEncoder);
            drive.set_vel_right(ca.second->rightEncoder);
            send_multipart(socket, ca.first, "Base", "Enc", data);
            count++;

            /* Publish light sensor data */
            ColorStamped light;
            light.mutable_color()->set_red(0);
            light.mutable_color()->set_green(0);
            light.mutable_color()->set_blue(ca.second->light_sensor_blue->getIntensity());
            
            light.SerializeToString(&data);
            send_multipart(socket, ca.first, "Light", "Readings", data);
            count++;

            /* Publish ground truth */
            PoseStamped pose;
            pose.mutable_pose()->mutable_position()->set_x(ca.second->pos.x);
            pose.mutable_pose()->mutable_position()->set_y(ca.second->pos.y);
            pose.mutable_pose()->mutable_orientation()->set_z(ca.second->angle);
            pose.SerializeToString(&data);
            send_multipart(socket, ca.first, "Base", "GroundTruth", data);
            
            /* Publish other stuff as necessary */
        }

        return count;
    }
// -----------------------------------------------------------------------------

}
