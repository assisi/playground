/*

 */
#include <iostream>

#include <boost/foreach.hpp>

#include <zmq.hpp>
#include "playground/zmq_helpers.hpp"

#include "PhysicalEngine.h"
#include "handlers/PhysicalObjectHandler.h"

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
    string PhysicalObjectHandler::createObject(const std::string& data, 
                                     World* world)
    {
        string name = "";
        Spawn msg;     
        assert(msg.ParseFromString(data));
        if (objects_.count(msg.name()) < 1)
        {
            name = msg.name();
            Point pos(msg.pose().position().x(),
                      msg.pose().position().y());
            double yaw(msg.pose().orientation().z());
            objects_[name] = new PhysicalObject;
            objects_[name]->pos = pos;
            objects_[name]->angle = yaw;
            if (msg.has_color())
            {
                objects_[name]->setColor(Color(msg.color().red(),
                                               msg.color().green(),
                                               msg.color().blue()));
            }
            if (msg.type() == "Cylinder")
            {
                objects_[name]->setCylindric(msg.cylinder_shape().radius(),
                                             msg.cylinder_shape().height(),
                                             msg.cylinder_shape().mass());
                world->addObject(objects_[name]);
            }
            else if (msg.type() == "Polygon")
            {
                Polygone p;
                for (int i = 0; i < msg.polygon().vertices_size(); i++)
                {
                    p.push_back(Point(msg.polygon().vertices(i).x()),
                                Point(msg.polygon().vertices(i).y()));
                }
                PhysicalObject::Hull hull(PhysicalObject::Part(p, msg.polygon().height()));
                objects_[name]->setCustomHull(hull, msg.polygon().mass());
                world->addObject(objects_[name]);
            }
            else
            {
                cerr << "Unknown object type: " << msg.type() << endl;
                delete objects_[name];
                objects_.erase(name);
            }
        }
        else
        {
            cerr << "Object "<< msg.name() << " already exists." << endl;
        }
        return name;
    }

// -----------------------------------------------------------------------------

    /* virtual */
    int PhysicalObjectHandler::handleIncoming(const std::string& name,
                                    const std::string& device,
                                    const std::string& command,
                                    const std::string& data)
    {
        int count = 0;
        if (device == "Pos")
        {
            if (command == "Set")
            {
                PoseStamped msg;
                assert(msg.ParseFromString(data));
                devices_[name]->pos = Point(msg.pose().position().x(),
                                            msg.pose().position().y());
                devices_[name]->angle = msg.pose().orientation().z();
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
    int PhysicalObjectHandler::sendOutgoing(socket_t& socket)
    {
        int count = 0;
        BOOST_FOREACH(const ObjectMap::value_type& ca, objects_)
        {
            std::string data;
            /* Publishing IR readings */
            PoseStamped pose;
            pose.mutable_pose()->mutable_position()->set_x(ca.second->pos.x);
            pose.mutable_pose()->mutable_position()->set_y(ca.second->pos.y);
            pose.mutable_pose()->mutable_orientation()->set_z(ca.second->angle);
            pose.SerializeToString(&data);
            zmq::send_multipart(socket, ca.first, "Pos", "Get", data);

            /* Publish other stuff as necessary ... */

            count++;
        }
        return count;
    }
// -----------------------------------------------------------------------------

}
