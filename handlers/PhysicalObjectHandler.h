/*! \file  PhysicalObjectHandler.h
    \brief Implementation of the generic object handler interface.
    
    Handles physical objects.

    TODO: Specfic robot handlers should probably inherit from this class,
          for generic object-handling capabilities.
*/

#ifndef ENKI_PHYSICAL_OBJECT_HANDLER_H
#define ENKI_PHYSICAL_OBJECT_HANDLER_H

#include <map>

#include "handlers/ObjectHandler.h"

namespace Enki
{

    class PhysicalObject;
    class WorldExt;

    //! Handling of PhysicalObjects
    /*!

     */
    class PhysicalObjectHandler : public ObjectHandler
    {
    public:
        PhysicalObjectHandler() { }
        virtual ~PhysicalObjectHandler() { }

        //! Casu factory method
        /*! Creates PhysicalObjects.

          Keeps a pointer to the created object, but does not
          delete it in the destructor.

          \return Returns the name of the created object.
        */
        virtual std::string createObject(const std::string& data, WorldExt* world);

        //! Handle incoming message
        /*! Handles casu actuator commands.

         */
        virtual int handleIncoming(const std::string& name,
                                   const std::string& device,
                                   const std::string& command,
                                   const std::string& data);

        //! Assemble outgoing messages.
        /*! Sends CASU sensor data messages.

         */
        virtual int sendOutgoing(zmq::socket_t& socket);

        virtual PhysicalObject* getObject(const std::string& name);

    private:
        typedef std::map<std::string, PhysicalObject*> ObjectMap;
        ObjectMap objects_;
    };
}

#endif
