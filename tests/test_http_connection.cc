#include "hxf/hxf.h"
#include "hxf/http/http_connection.h"

static hxf::Logger::ptr g_logger = HXF_LOG_ROOT();

void run() {
    hxf::Address::ptr addr = hxf::Address::LookupAnyIPAddress("120.76.197.56:8028");
    if(!addr) {
        HXF_LOG_ERROR(g_logger) << "create addr failed.";
        return;
    }
    hxf::Socket::ptr sock = hxf::Socket::CreateTCP(addr);
    if(!sock->connect(addr)) {
        HXF_LOG_ERROR(g_logger) << "connect server fail";
        return;
    }
    hxf::http::HttpConnection::ptr conn(new hxf::http::HttpConnection(sock));
    hxf::http::HttpRequest::ptr req(new hxf::http::HttpRequest);
    req->setHeader("host", "www.hxf.com");
    req->setBody("hello other brother server");
    conn->sendRequest(req);
    
    hxf::http::HttpResponse::ptr rsp = conn->recvResponse();
    if(rsp) {
        HXF_LOG_INFO(g_logger) << "Get response:\n"
        << *rsp;
    }
}


int main(int argc, char** argv) {
    hxf::IOManager iom;
    iom.schedule(run);
    return 0;
}
