/*

 */
#include <iostream>

#include <boost/foreach.hpp>

#include <zmq.hpp>
#include "playground/zmq_helpers.hpp"

#include "playground/WorldExt.h"
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
using std::cout;

using namespace AssisiMsg;

namespace Enki
{

    /* virtual */
    string BeeHandler::createObject(const std::string& data, 
                                     WorldExt* world)
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
        if (device == "Base")
        {
            if (command == "Vel")
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
        else if (device == "Color")
        {
            if (command == "Set")
            {
                ColorStamped color_msg;
                assert(color_msg.ParseFromString(data));
                /*
                bees_[name]->setColor(Color(color_msg.color().red(),
                                            color_msg.color().green(),
                                            color_msg.color().blue()));
                */
                bees_[name]->setColor(color_msg.color().red(),
                                      color_msg.color().green(),
                                      color_msg.color().blue());
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

            /* Publishing Object Sensor readings */
            ObjectArray objects;
            BOOST_FOREACH(ObjectSensor* obj, ca.second->object_sensors)
            {
                objects.add_range(obj->getDist());                
                objects.add_type(obj->getType());
            }
            objects.SerializeToString(&data);
            send_multipart(socket, ca.first, "Object", "Ranges", data);
            count++;

            /* Publish velocity setpoints */
            DiffDrive drive;
            drive.set_vel_left(ca.second->leftSpeed);
            drive.set_vel_right(ca.second->rightSpeed);
            drive.SerializeToString(&data);
            send_multipart(socket, ca.first, "Base", "VelRef", data);
            count++;

            /* Publish velocities */
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

            /* Publish temperature sensor data */
            TemperatureArray temps;
            temps.add_temp(ca.second->heat_sensor->getMeasuredHeat());
            temps.SerializeToString(&data);
            send_multipart(socket, ca.first, "Temp", "Temperatures", data);

            /* Publish Diagnostic color "actuator" set value */
            ColorStamped color;
            color.mutable_color()->set_red(ca.second->color_r_);
            color.mutable_color()->set_green(ca.second->color_g_);
            color.mutable_color()->set_blue(ca.second->color_b_);
            color.SerializeToString(&data);
            send_multipart(socket, ca.first, "Color", "ColorVal", data);

            /* Publish air flow sensor */
            AirflowReading airflowReading;
				airflowReading.set_intensity (ca.second->air_flow_sensor->intensity.norm ());
				airflowReading.set_direction (ca.second->air_flow_sensor->intensity.angle ());
				airflowReading.SerializeToString (&data);
				send_multipart (socket, ca.first, "Airflow", "Reading", data);

            /* Publish other stuff as necessary */
        }

        return count;
    }
// -----------------------------------------------------------------------------

    /* virtual */
    PhysicalObject* BeeHandler::getObject(const std::string& name)
    {
        if (bees_.count(name) > 0)
        {
            return bees_[name];
        }
        else
        {
            return 0;
        }
    }

// -----------------------------------------------------------------------------

}
