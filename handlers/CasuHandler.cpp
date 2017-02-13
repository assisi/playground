
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
	extern double env_temp;

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
            casus_[name] = new Casu(pos, yaw, world, env_temp);
            casus_[name]->pos = pos;
            casus_[name]->angle = yaw;
            // casus_[name]->peltier->setHeatDiffusivity (world, WorldHeat::THERMAL_DIFFUSIVITY_COPPER);

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
        else if (device == "Peltier")
        {
            if (command == "On")
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
        else if (device == "Speaker")
        {
            if (command == "On")
            {
                VibrationSetpoint freq_msg;
                assert (freq_msg.ParseFromString (data));
                casus_[name]->vibration_source->setFrequency (freq_msg.freq ());
                count++;
            }
        }
        else if (device == "Airflow")
        {
           if (command == "On")
           {
              Airflow airflow;
              assert (airflow.ParseFromString (data));
              BOOST_FOREACH(AirPump* p, casus_ [name]->air_pumps)
              {
                  p->setIntensity (airflow.intensity ());
              }
              count++;
           }
           else if (command == "Off")
           {
              BOOST_FOREACH(AirPump* p, casus_ [name]->air_pumps)
              {
                  p->setIntensity (0);
              }
              count++;
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
            
            ranges.SerializeToString(&data);
            zmq::send_multipart(socket, ca.first, "IR", "Ranges", data);

            /* Publish vibration readings */
            VibrationReadingArray vibrations;
            BOOST_FOREACH (VibrationSensor *vs, ca.second->vibration_sensors)
            {
                VibrationReading *vibrationReading = vibrations.add_reading ();
                const std::vector<double> &amplitudes = vs->getAmplitude ();
                const std::vector<double> &frequencies = vs->getFrequency ();
                BOOST_FOREACH (double a, vs->getAmplitude ())
                    vibrationReading->add_amplitude (a);
                BOOST_FOREACH (double f, vs->getFrequency ())
                    vibrationReading->add_freq (f);
					// TODO
					//  add vibration amplitude standard deviation
            }
            vibrations.SerializeToString (&data);
            zmq::send_multipart (socket, ca.first, "Fft", "Measurements", data);

            /* Publish temperature sensor readings. */
            TemperatureArray temperatures;
            BOOST_FOREACH(HeatSensor* h, ca.second->temp_sensors)
            {
                temperatures.add_temp(h->getMeasuredHeat());
            }
            /* Add fake readings for additional sensors and estimates
               that exist on real CASUs.
               TODO: This should also be calibrated with measurements.
            */
            double temp_avg = (ca.second->temp_sensors[0]->getMeasuredHeat() +
                               ca.second->temp_sensors[1]->getMeasuredHeat() +
                               ca.second->temp_sensors[2]->getMeasuredHeat() +
                               ca.second->temp_sensors[3]->getMeasuredHeat()) / 4.0;
            // Bottom PCB sensor
            temperatures.add_temp(temp_avg+1.0);
            // Metal ring around CASU
            temperatures.add_temp(temp_avg+0.5);
            // Wax around CASU
            temperatures.add_temp(temp_avg);
                                  
            temperatures.SerializeToString(&data);
            zmq::send_multipart(socket, ca.first, "Temp", "Temperatures", data);

            /* Publish actuator setpoints and states. */           
            
            /* Temperature setpoint */
            Temperature temp_ref;
            temp_ref.set_temp(ca.second->peltier->getHeat());
            temp_ref.SerializeToString(&data);
            if (ca.second->peltier->isSwitchedOn())
            {
                zmq::send_multipart(socket, ca.first, "Peltier", "On", data);
            }
            else
            {
                zmq::send_multipart(socket, ca.first, "Peltier", "Off", data);
            }                
            
            /* Vibration setpoint */
            VibrationSetpoint vib_ref;
            vib_ref.set_freq(ca.second->vibration_source->getFrequency());
            vib_ref.set_amplitude(ca.second->vibration_source->getMaximumAmplitude());
            vib_ref.SerializeToString(&data);
            //! TODO: WaveVibrationSource should implement an isSwitchedOn function!
            if (ca.second->vibration_source->getFrequency())
            {
                zmq::send_multipart(socket, ca.first, "Speaker", "On", data);
            }
            else
            {
                zmq::send_multipart(socket, ca.first, "Speaker", "Off", data);
            }

            /* Airflow setpoint */
            Airflow air_ref;
            air_ref.set_intensity(ca.second->air_pumps[0]->getIntensity());
            air_ref.SerializeToString(&data);
            //! TODO: AirPump should implement an isSwitchedOn function!
            if (ca.second->air_pumps[0]->getIntensity())
            {
                zmq::send_multipart(socket, ca.first, "Airflow", "On", data);
            }
            else
            {
                zmq::send_multipart(socket, ca.first, "Airflow", "Off", data);
            }

            /* Diagnostic LED setpoint */
            ColorStamped color_ref;
            Color col = ca.second->top_led->getColor();
            color_ref.mutable_color()->set_red(col.r());
            color_ref.mutable_color()->set_green(col.g());
            color_ref.mutable_color()->set_blue(col.b());
            color_ref.SerializeToString(&data);
            if (ca.second->top_led->isSwitchedOn())
            {
                zmq::send_multipart(socket, ca.first, "DiagnosticLed", "On", data);
            }
            else
            {
                zmq::send_multipart(socket, ca.first, "DiagnosticLed", "Off", data);
            }

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

// Local Variables: 
// indent-tabs-mode: nil
// End: 
