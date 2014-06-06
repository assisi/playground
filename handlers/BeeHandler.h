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
        BeeHandler() { }
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
    };
}

#endif
