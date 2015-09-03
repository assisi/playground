/*! \file  BeeHandler.h
    \brief Communication handler for the Bees.
*/

#ifndef ENKI_BEE_HANDLER_H
#define ENKI_BEE_HANDLER_H

#include <map>

#include "handlers/ObjectHandler.h"

namespace Enki
{

    class Bee;
    class WorldExt;

    //! Handling of Bees
    /*!

     */
    class BeeHandler : public ObjectHandler
    {
    public:
        BeeHandler(double body_length, double body_width, double body_height,
                   double body_mass, double max_speed) :
        body_length_(body_length), body_width_(body_width), body_height_(body_height),
          body_mass_(body_mass), max_speed_(max_speed) { }
        virtual ~BeeHandler() { }

        //! Bee factory method
        /*! Creates Bees.

          Keeps a pointer to the created robot, but does not
          delete it in the destructor.

          \return Returns the name of the created robot.
        */
        virtual std::string createObject(const std::string& data, WorldExt* world);

        //! Handle incoming message
        /*! Handles bee actuator commands.

         */
        virtual int handleIncoming(const std::string& name,
                                   const std::string& device,
                                   const std::string& command,
                                   const std::string& data);

        //! Assemble outgoing messages.
        /*! Sends Bee sensor data messages.

         */
        virtual int sendOutgoing(zmq::socket_t& socket);

    virtual PhysicalObject* getObject(const std::string& name);

    private:
        typedef std::map<std::string, Bee*> BeeMap;
        BeeMap bees_;
        double body_length_;
        double body_width_;
        double body_height_;
        double body_mass_;
        double max_speed_;
    };
}

#endif
