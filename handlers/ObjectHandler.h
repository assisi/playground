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
        virtual std::string createObject(const std::string& data,
                                         World* world) = 0;

        //! Handle incoming message
        /*! Override this method to handle incoming messages
            for your particular object.

            \arg sock The socket containing the message to be handled.
                      It's a 0MQ multipart message, with the 
                      first part(s) peeled off.

         */
        virtual int handleIncoming(const std::string& name,
                                   const std::string& device,
                                   const std::string& command,
                                   const std::string& data) = 0;

        //! Send outgoing messages.
        /*! Override this method to send outgoing messages 
            for your particular object.

            \arg sock Outgoing messages are written to socket sock.
            
         */
        virtual int sendOutgoing(zmq::socket_t& socket) = 0;

        //! Get object by name.
        /*! Returns pointer to the handled object "name", if it exists.
            Returns 0 otherwise.
         */
        virtual PhysicalObject* getObject(const std::string& name) = 0;
    };

}

#endif
