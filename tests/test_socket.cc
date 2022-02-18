#include "hxf/socket.h"
#include "hxf/hxf.h"
#include "hxf/iomanager.h"

static hxf::Logger::ptr g_looger = HXF_LOG_ROOT();

void test_socket() {
    //std::vector<hxf::Address::ptr> addrs;
    //hxf::Address::Lookup(addrs, "www.baidu.com", AF_INET);
    //hxf::IPAddress::ptr addr;
    //for(auto& i : addrs) {
    //    HXF_LOG_INFO(g_looger) << i->toString();
    //    addr = std::dynamic_pointer_cast<hxf::IPAddress>(i);
    //    if(addr) {
    //        break;
    //    }
    //}
    hxf::IPAddress::ptr addr = hxf::Address::LookupAnyIPAddress("www.baidu.com");
    if(addr) {
        HXF_LOG_INFO(g_looger) << "get address: " << addr->toString();
    } else {
        HXF_LOG_ERROR(g_looger) << "get address fail";
        return;
    }

    hxf::Socket::ptr sock = hxf::Socket::CreateTCP(addr);
    addr->setPort(80);
    HXF_LOG_INFO(g_looger) << "addr=" << addr->toString();
    if(!sock->connect(addr)) {
        HXF_LOG_ERROR(g_looger) << "connect " << addr->toString() << " fail";
        return;
    } else {
        HXF_LOG_INFO(g_looger) << "connect " << addr->toString() << " connected";
    }

    const char buff[] = "GET / HTTP/1.0\r\n\r\n";
    int rt = sock->send(buff, sizeof(buff));
    if(rt <= 0) {
        HXF_LOG_INFO(g_looger) << "send fail rt=" << rt;
        return;
    }

    std::string buffs;
    buffs.resize(4096);
    rt = sock->recv(&buffs[0], buffs.size());

    if(rt <= 0) {
        HXF_LOG_INFO(g_looger) << "recv fail rt=" << rt;
        return;
    }

    buffs.resize(rt);
    HXF_LOG_INFO(g_looger) << buffs;
}

void test2() {
    hxf::IPAddress::ptr addr = hxf::Address::LookupAnyIPAddress("www.baidu.com:80");
    if(addr) {
        HXF_LOG_INFO(g_looger) << "get address: " << addr->toString();
    } else {
        HXF_LOG_ERROR(g_looger) << "get address fail";
        return;
    }

    hxf::Socket::ptr sock = hxf::Socket::CreateTCP(addr);
    if(!sock->connect(addr)) {
        HXF_LOG_ERROR(g_looger) << "connect " << addr->toString() << " fail";
        return;
    } else {
        HXF_LOG_INFO(g_looger) << "connect " << addr->toString() << " connected";
    }

    uint64_t ts = hxf::GetCurrentUS();
    for(size_t i = 0; i < 10000000000ul; ++i) {
        if(int err = sock->getError()) {
            HXF_LOG_INFO(g_looger) << "err=" << err << " errstr=" << strerror(err);
            break;
        }

        //struct tcp_info tcp_info;
        //if(!sock->getOption(IPPROTO_TCP, TCP_INFO, tcp_info)) {
        //    HXF_LOG_INFO(g_looger) << "err";
        //    break;
        //}
        //if(tcp_info.tcpi_state != TCP_ESTABLISHED) {
        //    HXF_LOG_INFO(g_looger)
        //            << " state=" << (int)tcp_info.tcpi_state;
        //    break;
        //}
        static int batch = 10000000;
        if(i && (i % batch) == 0) {
            uint64_t ts2 = hxf::GetCurrentUS();
            HXF_LOG_INFO(g_looger) << "i=" << i << " used: " << ((ts2 - ts) * 1.0 / batch) << " us";
            ts = ts2;
        }
    }
}

int main(int argc, char** argv) {
    hxf::IOManager iom;
    iom.schedule(&test_socket);
    //iom.schedule(&test2);
    return 0;
}

