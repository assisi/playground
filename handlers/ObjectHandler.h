/*! \file  ObjectHandler.h
    \brief Classes defining the interface for handling messages to/from Enki robots.

 */

#ifndef ENKI_OBJECT_HANDLER_H
#define ENKI_OBJECT_HANDLER_H


namespace zmq
{
    class socket_t;
}

namespace Enki
{
    class World;
    class PhysicalObject;
    
    //! Abstract base class, defines the message-handling interface for Enki
    /*! Users should implement their own message handling according for each robot type.

     */
    class ObjectHandler
    {
    public:
        ObjectHandler() { }
        virtual ~ObjectHandler() { }
        
        //! Robot factory method
        /*! Override this method to create the appropriate
            object type.

         */
        virtual std::string createObject(zmq::socket_t* sock, World* world) = 0;

        //! Handle incoming message
        /*! Override this method to handle incoming messages
            for your particular object.

            \arg sock The socket containing the message to be handled.
                      It's a 0MQ multipart message, with the 
                      first part(s) peeled off.

         */
        virtual int handleIncoming(zmq::socket_t* sock, const std::string& name) = 0;

        //! Assemble outgoing messages
        /*! Override this method to create outgoing messages 
            for your particular object.

            \arg sock Outgoing messages are written to socket sock.
            
         */
        virtual int sendOutgoing(zmq::socket_t* sock) = 0;
    };

}

#endif
