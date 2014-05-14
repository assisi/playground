/*

 */
#include <cassert>

#include <iostream>

#include <boost/foreach.hpp>

#include <zmq.hpp>
#include "playground/zmq_helpers.hpp"

#include <PhysicalEngine.h>

#include "robots/e-puck/EPuck.h"
#include "handlers/EPuckHandler.h"

// Protobuf message headers
#include "base_msgs.pb.h"
#include "dev_msgs.pb.h"
#include "sim_msgs.pb.h"

using zmq::message_t;
using zmq::socket_t;
using std::string;
using std::cerr;
using std::endl;

using namespace AssisiMsg;

namespace Enki
{

// -----------------------------------------------------------------------------
    /* virtual */
    string EPuckHandler::createObject(const std::string& data,
                                      World* world)
    {
        string name = "";
        Spawn spawn_msg;
        assert(spawn_msg.ParseFromString(data));
        if (epucks_.count(spawn_msg.name()) < 1)
        {
            name = spawn_msg.name();
            Point pos(spawn_msg.pose().position().x(),
                      spawn_msg.pose().position().y());
            double yaw(spawn_msg.pose().orientation().z());
            epucks_[name] = new EPuck;
            epucks_[name]->pos = pos;
            epucks_[name]->angle = yaw;
            world->addObject(epucks_[name]);
        }
        else
        {
            cerr << "Robot " << spawn_msg.name() << " already exists!" << endl;
        }
        return name;
    }

// -----------------------------------------------------------------------------

    /* virtual */
    int EPuckHandler::handleIncoming(const std::string& name,
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
                epucks_[name]->leftSpeed = drive.vel_left();
                epucks_[name]->rightSpeed = drive.vel_right();
                count++;
            }
            else
            {
                cerr << "Unknown command " << command
                     << " for " << name << "/" << device << endl;
            }      
        }
        else
        {
            cerr << "Unknown device " << command << " for " << name << endl;
        }
        return count;
    }

// -----------------------------------------------------------------------------

    /* virtual */
    int EPuckHandler::sendOutgoing(socket_t& socket)
    {
        int count = 0;
        BOOST_FOREACH(const EPuckMap::value_type& ep, epucks_)
        {
            /* Publishing IR readings */
            
            // Send IR data (convert cm->m)
            RangeArray ranges;
            ranges.add_range(ep.second->infraredSensor0.getDist());
            ranges.add_range(ep.second->infraredSensor1.getDist());
            ranges.add_range(ep.second->infraredSensor2.getDist());
            ranges.add_range(ep.second->infraredSensor3.getDist());
            ranges.add_range(ep.second->infraredSensor4.getDist());
            ranges.add_range(ep.second->infraredSensor5.getDist());
            ranges.add_range(ep.second->infraredSensor6.getDist());
            ranges.add_range(ep.second->infraredSensor7.getDist());
            
            std::string data;
            ranges.SerializeToString(&data);
            zmq::send_multipart(socket, ep.first, "ir", "ranges", data);
            count++;

            /* Publish other stuff as necessary ... */
        }
        return count;
    }

// -----------------------------------------------------------------------------

    /* virtual */
    PhysicalObject* EPuckHandler::getObject(const std::string& name)
    {
        if (epucks_.count(name) > 0)
        {
            return epucks_[name];
        }
        else
        {
            return 0;
        }
    }

// -----------------------------------------------------------------------------

}


