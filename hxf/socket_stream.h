#ifndef __HXF_SOCKET_STREAM_H__
#define __HXF_SOCKET_STREAM_H__

#include <memory>
#include "stream.h"
#include "socket.h"

namespace hxf {

class SocketStream : public Stream {
public:
    SocketStream(Socket::ptr sock, bool owner = true);
    virtual ~SocketStream();

    virtual int read(void* buffer, size_t length) override;
    virtual int read(ByteArray::ptr ba, size_t length) override;
    virtual int write(const void* buffer, size_t length) override;
    virtual int write(ByteArray::ptr ba, size_t length) override;
    virtual void close() override;
    
    Socket::ptr getSocket() const { return m_socket;}
    bool isConnected() const;

    Address::ptr getRemoteAddress();
    Address::ptr getLocalAddress();
    std::string getRemoteAddressString();
    std::string getLocalAddressString();

protected:
    Socket::ptr m_socket;
    bool m_owner;
};

}


#endif
