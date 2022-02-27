#ifndef __HXF_HTTP_CONNECTION_H__
#define __HXF_HTTP_CONNECTION_H__

#include "hxf/socket_stream.h"
#include "http.h"

namespace hxf {
namespace http {

class HttpConnection : public SocketStream {
public:
    typedef std::shared_ptr<HttpConnection> ptr;

    HttpConnection(Socket::ptr sock, bool owner = true);
    
    HttpResponse::ptr recvResponse();
    int sendRequest(HttpRequest::ptr req);
};

}
}

#endif
