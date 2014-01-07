/*! \file  CasuHandler.h
    \brief Communication handler for the Casu.
*/

#ifndef ENKI_CASU_HANDLER_H
#define ENKI_CASU_HANDLER_H

#include <map>

#include "handlers/ObjectHandler.h"

namespace Enki
{

    class Casu;
    class World;

    //! Handling of Casus
    /*!

     */
    class CasuHandler : public ObjectHandler
    {
    public:
        CasuHandler() { }
        virtual ~CasuHandler() { }

        //! Casu factory method
        /*! Creates Casus.

          Keeps a pointer to the created robot, but does not
          delete it in the destructor.

          \return Returns the name of the created robot.
        */
        virtual std::string createObject(zmq::socket_t* sock, World* world);

        //! Handle incoming message
        /*! Handles casu actuator commands.

         */
        virtual int handleIncoming(zmq::socket_t* sock, const std::string& name);

        //! Assemble outgoing messages.
        /*! Sends CASU sensor data messages.

         */
        virtual int sendOutgoing(zmq::socket_t* sock);

    private:
        typedef std::map<std::string, Casu*> CasuMap;
        CasuMap casus_;
    };
}

#endif
