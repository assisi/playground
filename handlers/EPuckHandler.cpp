/*

 */
#include <iostream>

#include <boost/foreach.hpp>

#include <zmq.hpp>
#include "playground/zmq_helpers.h"

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

    /* virtual */
    string EPuckHandler::createObject(socket_t* sock, World* world)
    {
        string name("");
        message_t msg;     
        if (!last_part(*sock))
        {
            sock->recv(&msg);
            string msg_str(msg_to_str(msg));
            Spawn spawn_msg;
            if (spawn_msg.ParseFromString(msg_str))
            {
                name = spawn_msg.name();
                Point pos(spawn_msg.pose().position().x(),
                          spawn_msg.pose().position().y());
                double yaw(spawn_msg.pose().orientation().z());
                if (epucks_.count(name) < 1)
                {
                    epucks_[name] = new EPuck;
                    epucks_[name]->pos = pos;
                    epucks_[name]->angle = yaw;
                    world->addObject(epucks_[name]);
                }
                else
                {
                    cerr << "EPuck "<< name << " already exists." << endl;
                }
            }
            else
            {
                cerr << "Error deserializing spawn message!" << endl;
            }
        }
        else
        {
            cerr << "Missing message body from spawn message!" << std::endl;
        } 
        return name;
    }

// -----------------------------------------------------------------------------

    /* virtual */
    int EPuckHandler::handleIncoming(socket_t* sock, const string& name)
    {
        int count = 0;
        message_t msg;
        if (last_part(*sock))
        {
            cerr << "Missing command body for " << name << endl;
            return 0;
        }
        sock->recv(&msg);
        string device(msg_to_str(msg));
        if (device == "base")
        {
            if (last_part(*sock))
            {
                cerr << "Missing commad body for "
                     << name << "/" << device << endl;
                return 0;
            }
            sock->recv(&msg);
            string cmd(msg_to_str(msg));
            if (cmd != "vel")
            {
                cerr << "Unknown command for " << name << "/" << device << endl;
                return 0;
            }
            if (last_part(*sock))
            {
                cerr << "Missing commad body for "
                     << name << "/" << device << "/" << cmd << endl;
                return 0;
            }
            sock->recv(&msg);
            DiffDrive drive;
            if (drive.ParseFromString(msg_to_str(msg)))
            {
                epucks_[name]->leftSpeed = drive.vel_left();
                epucks_[name]->rightSpeed = drive.vel_right();
            }
            else
            {
                cerr << "Invalid argument type for " 
                     << name << "/" << device << "/" << cmd << endl;
            }
        }
        else
        {
            cerr << "Unknown device " << device << endl;
        }
        return count;
    }

    /* virtual */
    int EPuckHandler::sendOutgoing(socket_t* sock)
    {
        int count = 1;
        BOOST_FOREACH(const EPuckMap::value_type& ep, epucks_)
        {
            /* Publishing IR readings */

            // Send message envelope
            message_t msg;
            str_to_msg(ep.first, msg);
            sock->send(msg, ZMQ_SNDMORE);
            str_to_msg("ir", msg);
            sock->send(msg, ZMQ_SNDMORE);
            str_to_msg("ranges", msg);
            sock->send(msg, ZMQ_SNDMORE);
            
            // Send IR data (convert cm->m)
            RangeArray ranges;
            ranges.add_range(0.01*ep.second->infraredSensor0.getDist());
            ranges.add_range(0.01*ep.second->infraredSensor1.getDist());
            ranges.add_range(0.01*ep.second->infraredSensor2.getDist());
            ranges.add_range(0.01*ep.second->infraredSensor3.getDist());
            ranges.add_range(0.01*ep.second->infraredSensor4.getDist());
            ranges.add_range(0.01*ep.second->infraredSensor5.getDist());
            ranges.add_range(0.01*ep.second->infraredSensor6.getDist());
            ranges.add_range(0.01*ep.second->infraredSensor7.getDist());
            std::string data_str;
            ranges.SerializeToString(&data_str);
            str_to_msg(data_str, msg);
            sock->send(msg);
            count++;

            /* Publish other stuff as necessary ... */
        }
        return count;
    }
}
