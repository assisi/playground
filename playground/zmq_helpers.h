/*! \file  zmq_helpers.h
    \brief Convenience functions and classes for zmq.

 */

#ifndef ZMQ_HELPERS_H
#define ZMQ_HELPERS_H

namespace zmq
{
    //! Check if this is the last part of a multipart message.
    /*! Returns true if there are no further parts.
     */
    static bool last_part(socket_t& socket)
    {
        int64_t more;
        size_t more_size = sizeof(more);
        socket.getsockopt(ZMQ_RCVMORE, &more, &more_size);
        return  (more == 1) ? false : true;
    }

    //! Convert message data to string.
    static std::string msg_to_str(message_t& msg)
    {
        std::string data(static_cast<char*>(msg.data()), msg.size());
        return data;
    }

    //! Convert string to message data.
    static void str_to_msg(const std::string& str, message_t& msg)
    {
        msg.rebuild(str.size());
        memcpy(msg.data(), str.data(), str.size());
    }
                                  
}

#endif
