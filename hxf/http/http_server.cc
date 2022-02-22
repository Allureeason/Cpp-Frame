#include "http_server.h"
#include "hxf/log.h"
#include "http.h"
#include "http_parser.h"

namespace hxf {
namespace http {

static hxf::Logger::ptr g_logger = HXF_LOG_NAME("system");

HttpServer::HttpServer(bool keepalive
             ,IOManager* work
             ,IOManager* accept_work)
    :TcpServer(work, accept_work)
    ,m_isKeepalive(keepalive) {
    m_dispatch.reset(new ServletDispatch);
}

void HttpServer::handleClient(Socket::ptr client) {
    HXF_LOG_INFO(g_logger) << "handleClient " << *client;
    HttpSession::ptr session(new HttpSession(client));
    do {
        auto req = session->recvRequest();
        if(!req) {
            HXF_LOG_WARN(g_logger) << "recv http request fail, errno="
                << errno << " errstr=" << strerror(errno)
                << " client:" << *client << " keep_alive=" << m_isKeepalive;
            break;
        }
        HttpResponse::ptr rsp(new HttpResponse(req->getVersion()
                    ,req->isClose() || !m_isKeepalive));
        m_dispatch->handle(req, rsp, session);
        rsp->setHeader("Server", getName());
        session->sendResponse(rsp);
        if(!m_isKeepalive || req->isClose()) {
            break;
        }

    } while(true);
}

}
}
