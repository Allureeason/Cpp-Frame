#include "hxf/tcp_server.h"
#include "hxf/iomanager.h"
#include "hxf/log.h"

hxf::Logger::ptr g_logger = HXF_LOG_ROOT();

void run() {
    auto addr = hxf::Address::LookupAny("0.0.0.0:8033");
    auto addr2 = hxf::UnixAddress::ptr(new hxf::UnixAddress("/tmp/unix_addr"));
    HXF_LOG_INFO(g_logger) << *addr << " - " << *addr2;
    std::vector<hxf::Address::ptr> addrs;
    addrs.push_back(addr);
    addrs.push_back(addr2);

    hxf::TcpServer::ptr tcp_server(new hxf::TcpServer);
    std::vector<hxf::Address::ptr> fails;
    while(!tcp_server->bind(addrs, fails)) {
        sleep(2);
    }
    tcp_server->start();
    
}
int main(int argc, char** argv) {
    hxf::IOManager iom(2);
    iom.schedule(run);
    return 0;
}
