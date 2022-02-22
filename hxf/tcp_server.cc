#include "tcp_server.h"
#include "log.h"
#include "config.h"

namespace hxf {

static hxf::Logger::ptr g_logger = HXF_LOG_NAME("system");

static hxf::ConfigVar<uint64_t>::ptr g_tcp_read_timeout =
    hxf::Config::Lookup("tcp.read_timeout", (uint64_t)(60 * 1000 * 2), "tcp read timeout");

TcpServer::TcpServer(IOManager* worker, IOManager* accept_worker)
    :m_worker(worker)
    ,m_acceptWorker(accept_worker)
    ,m_readTimeout(g_tcp_read_timeout->getValue())
    ,m_name("hxf_server/1.0")
    ,m_isStop(true) {
}

TcpServer::~TcpServer() {
    for(auto &sock : m_socks) {
        sock->close();
    }
    m_socks.clear();
}

bool TcpServer::bind(hxf::Address::ptr addr) {
    std::vector<Address::ptr> addrs;
    std::vector<Address::ptr> fails;
    addrs.emplace_back(addr);
    return bind(addrs, fails);
}

bool TcpServer::bind(const std::vector<Address::ptr>& addrs,
        std::vector<Address::ptr>& fails) {
    for(auto &addr : addrs) {
        Socket::ptr sock = Socket::CreateTCP(addr);
        if(!sock->bind(addr)) {
            HXF_LOG_ERROR(g_logger) << "bind fail errno="
                << errno << " errstr=" << strerror(errno);
            fails.push_back(addr);
            continue;
        }
        if(!sock->listen()) {
            HXF_LOG_ERROR(g_logger) << "listen fail errno="
                << errno << " errstr=" << strerror(errno);
            fails.push_back(addr);
            continue;
        }
        m_socks.push_back(sock);
    }
    
    if(!fails.empty()) {
        m_socks.clear();
        return false;
    }

    for(auto &i : m_socks) {
        HXF_LOG_INFO(g_logger) << "server bind success: " << *i;
    }
    return true;
}

void TcpServer::startAccept(Socket::ptr sock) {
    while(!m_isStop) {
        Socket::ptr client = sock->accept();
        if(client) {
            m_worker->schedule(std::bind(&TcpServer::handleClient,
                        shared_from_this(), client));
        } else {
            HXF_LOG_ERROR(g_logger) << "accept fail errno="
                << errno << " errstr=" << strerror(errno);
        }
    }
}

bool TcpServer::start() {
    if(!m_isStop) {
        return true;
    }
    m_isStop = false;
    for(auto &sock : m_socks) {
        m_acceptWorker->schedule(std::bind(&TcpServer::startAccept,
                    shared_from_this(), sock));
    }
    return true;
}

void TcpServer::stop() {
    m_isStop = true;
    auto self = shared_from_this();
    m_acceptWorker->schedule([this, self](){
        for(auto &sock : m_socks) {
            sock->cancelAll();
            sock->close();
        }
        m_socks.clear();
    });
}

void TcpServer::handleClient(Socket::ptr client) {
    HXF_LOG_INFO(g_logger) << "handleClient: " << *client;
}



}
