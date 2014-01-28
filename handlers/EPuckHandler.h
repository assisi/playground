/* \file  EPuckHandler.h
   \brief Communication handler for the EPuck robot.

 */

#ifndef ENKI_EPUCK_HANDLER_H
#define ENKI_EPUCK_HANDLER_H

#include <map>

#include "handlers/ObjectHandler.h"

namespace Enki
{
    class World;
    class EPuck;
    
    //! Handling of EPuck robots
    /*! 

     */
    class EPuckHandler : public ObjectHandler
    {
    public:
        EPuckHandler() { }
        virtual ~EPuckHandler() { }
        
        //! Robot factory method
        /*! Creates E-Pucks.

            Keeps a pointer to the created robot, but does not
            delete it in the destructor.

            \return Returns the name of the created robot.

         */
        virtual std::string createObject(const std::string& data,
                                         World* world);

        //! Handle incoming message
        /*! Handles E-Puck motion commands.

         */
        virtual int handleIncoming(const std::string& name,
                                   const std::string& device,
                                   const std::string& command,
                                   const std::string& data);

        //! Assemble outgoing messages
        /*! Sends E-Puck sensor data messages.
         */
        virtual int sendOutgoing(zmq::socket_t& socket);

        virtual PhysicalObject* getObject(const std::string& name);

    private:
        typedef std::map<std::string, EPuck*> EPuckMap;
        EPuckMap epucks_;
    };

}

#endif
