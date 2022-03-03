#include "hxf/http/http_server.h"
#include "hxf/log.h"

static hxf::Logger::ptr g_logger = HXF_LOG_ROOT();

void run() {
    g_logger->setLevel(hxf::LogLevel::INFO);
    hxf::Address::ptr addr = hxf::Address::LookupAnyIPAddress("0.0.0.0:8028");
    if(!addr) {
        HXF_LOG_INFO(g_logger) << "create addr fail: " << *addr;
        return;
    }

    hxf::http::HttpServer::ptr http_server(new hxf::http::HttpServer(true));
    while(!http_server->bind(addr)) {
        HXF_LOG_ERROR(g_logger) << "bind fail: " << *addr;
        sleep(1);
    }
    http_server->start();
}

int main(int argc, char** argv) {
    hxf::IOManager iom(1);
    iom.schedule(run);
    return 0;
}
