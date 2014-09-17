
/*

 */
#include <iostream>

#include <boost/foreach.hpp>

#include <zmq.hpp>
#include "playground/zmq_helpers.hpp"

#include "playground/WorldExt.h"
#include "robots/Casu.h"
#include "handlers/CasuHandler.h"
#include "interactions/LightConstants.h"

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
    string CasuHandler::createObject(const std::string& data, 
                                     WorldExt* world)
    {
        string name = "";
        Spawn spawn_msg;     
        assert(spawn_msg.ParseFromString(data));
        if (casus_.count(spawn_msg.name()) < 1)
        {
            name = spawn_msg.name();
            Point pos(spawn_msg.pose().position().x(),
                      spawn_msg.pose().position().y());
            double yaw(spawn_msg.pose().orientation().z());
            casus_[name] = new Casu(world);
            casus_[name]->pos = pos;
            casus_[name]->angle = yaw;
            world->addObject(casus_[name]);
        }
        else
        {
            cerr << "Casu "<< spawn_msg.name() << " already exists." << endl;
        }
        return name;
    }

// -----------------------------------------------------------------------------

    /* virtual */
    int CasuHandler::handleIncoming(const std::string& name,
                                    const std::string& device,
                                    const std::string& command,
                                    const std::string& data)
    {
        int count = 0;
        if (device == "DiagnosticLed")
        {
            if (command == "On")
            {
                ColorStamped color_msg;
                assert(color_msg.ParseFromString(data));
                casus_[name]->top_led->on( Enki::Color(color_msg.color().red(),
                                                      color_msg.color().green(),
                                                      color_msg.color().blue(),
                                                      color_msg.color().alpha() ) );
                count++;
            }
            else if (command == "Off")
            {
                casus_[name]->top_led->off( );
                count++;
            }
            else
            {
                cerr << "Unknown command for " << name << "/" << device << endl;
            }         
        }
        else if (device == "Light")
        {
            if (command == "On")
            {
                ColorStamped color_msg;
                assert(color_msg.ParseFromString(data));
                casus_[name]->light_source_blue->on( color_msg.color().blue());
                count++;
             }
            else if (command == "Off")
            {
                casus_[name]->light_source_blue->off( );
                count++;
            }
            else
            {
                cerr << "Unknown command " << command << " for " << name << "/" << device << endl;
            }
        }
        else if (device == "Peltier")
        {
            if (command == "temp")
            {
                Temperature temp_msg;
                assert(temp_msg.ParseFromString(data));
                casus_[name]->peltier->setHeat(temp_msg.temp());
                casus_[name]->peltier->setSwitchedOn(true);
                count++;
            }
            else if (command == "Off")
            {
                casus_[name]->peltier->setSwitchedOn(false);
                count++;
            }
        }
        else if (device == "VibeMotor")
        {
            if (command == "on")
            {
                /*
                Vibration freq_msg;
                assert (freq_msg.ParseFromString (data));
                casus_[name]->vibration_source->setFrequency (freq_msg.freq ());
                count++;
                */
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
    int CasuHandler::sendOutgoing(socket_t& socket)
    {
        int count = 0;
        BOOST_FOREACH(const CasuMap::value_type& ca, casus_)
        {
            std::string data;
            
            /* Publishing IR readings */
            RangeArray ranges;
            BOOST_FOREACH(IRSensor* ir, ca.second->range_sensors)
            {
                ranges.add_range(ir->getDist());                
                ranges.add_raw_value(ir->getValue());
            }
            // Add an additional (fake) reading for the top sensor which isn't modeled
            // TODO: Do this in a smarter way, e.g., if more than half of the
            //       sensors are detecting objects, assume this one is also detecting
            //       and object. Right now, the top sensor is never detecting anything.
            ranges.add_range(10000);
            ranges.add_raw_value(0);
            
            ranges.SerializeToString(&data);
            zmq::send_multipart(socket, ca.first, "IR", "Ranges", data);

            /* Publish vibration readings */
            VibrationArray vibrations;
            BOOST_FOREACH (VibrationSensor *vs, ca.second->vibration_sensors)
            {
                /*
               const std::vector<double> &amplitudes = vs->getAmplitude ();
               const std::vector<double> &frequencies = vs->getFrequency ();
               BOOST_FOREACH (double a, vs->getAmplitude ())
                  vibrations.add_amplitude (a);
               BOOST_FOREACH (double f, vs->getFrequency ())
                  vibrations.add_freq (f);
                */
            }
            vibrations.SerializeToString (&data);
            zmq::send_multipart (socket, ca.first, "Acc", "Measurements", data);

            /* Publish temperature sensor readings. */
            TemperatureArray temperatures;
            BOOST_FOREACH(HeatSensor* h, ca.second->temp_sensors)
            {
                temperatures.add_temp(h->getMeasuredHeat());
            }
            temperatures.SerializeToString(&data);
            zmq::send_multipart(socket, ca.first, "Temp", "Temperatures", data);

            /* Publish other stuff as necessary ... */

            count++;
        }
        return count;
    }
// -----------------------------------------------------------------------------

    /* virtual */
    PhysicalObject* CasuHandler::getObject(const std::string& name)
    {
        if (casus_.count(name) > 0)
        {
            return casus_[name];
        }
        else
        {
            return 0;
        }
    }

// -----------------------------------------------------------------------------

}
