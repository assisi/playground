/*

 */
#include <iostream>

#include <boost/foreach.hpp>

#include <zmq.hpp>
#include "playground/zmq_helpers.h"

#include "robots/Casu.h"
#include "handlers/CasuHandler.h"

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
    string CasuHandler::createObject(socket_t* sock, World* world)
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
                if (casus_.count(name) < 1)
                {
                    casus_[name] = new Casu;
                    casus_[name]->pos = pos;
                    casus_[name]->angle = yaw;
                    world->addObject(casus_[name]);
                }
                else
                {
                    cerr << "Casu "<< name << " already exists." << endl;
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
    int CasuHandler::handleIncoming(socket_t* sock, const string& name)
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
        if (device == "DiagnosticLed")
        {
            if (last_part(*sock))
            {
                cerr << "Missing commad body for "
                     << name << "/" << device << endl;
                return 0;
            }
            sock->recv(&msg);
            string cmd(msg_to_str(msg));
            if (cmd == "On")
            {

                if (last_part(*sock))
                {
                    cerr << "Missing commad body for "
                         << name << "/" << device << "/" << cmd << endl;
                    return 0;
                }
                sock->recv(&msg);
                ColorStamped color_msg;
                if (color_msg.ParseFromString(msg_to_str(msg)))
                {
                    casus_[name]->top_led.on( Enki::Color(color_msg.color().red(),
                                                          color_msg.color().green(),
                                                          color_msg.color().blue(),
                                                          color_msg.color().alpha() ) );
                }
                else
                {
                    cerr << "Invalid argument type for " 
                         << name << "/" << device << "/" << cmd << endl;
                }
            }
            else if (cmd == "Off")
            {
                if (last_part(*sock))
                {
                    cerr << "Missing commad body for "
                         << name << "/" << device << "/" << cmd << endl;
                    return 0;
                }
                sock->recv(&msg);
                /* Since the command is "Off", we don't care about
                   the message contents.
                */
                casus_[name]->top_led.off( );
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

    /* virtual */
    int CasuHandler::sendOutgoing(socket_t* sock)
    {
        int count = 1;
        BOOST_FOREACH(const CasuMap::value_type& ep, casus_)
        {
            /* Publishing IR readings */

            // Send message envelope
            /*
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
            */
            /* Publish other stuff as necessary ... */
        }
        return count;
    }
}
