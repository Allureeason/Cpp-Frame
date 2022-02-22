#ifndef __HXF_HTTP_SERVER_H__
#define __HXF_HTTP_SERVER_H__

#include "hxf/tcp_server.h"
#include "http_session.h"
#include "servlet.h"

namespace hxf {
namespace http {

class HttpServer : public TcpServer {
public:
    typedef std::shared_ptr<HttpServer> ptr;
    HttpServer(bool keepalive = false
             ,IOManager* work = hxf::IOManager::GetThis()
             ,IOManager* accept_work = hxf::IOManager::GetThis());

    ServletDispatch::ptr getDispatch() const { return m_dispatch;}
    void setDispatch(ServletDispatch::ptr v) { m_dispatch = v;}

protected:
    virtual void handleClient(Socket::ptr client) override;
private:
    bool m_isKeepalive;
    ServletDispatch::ptr m_dispatch;
};

}
}

#endif
