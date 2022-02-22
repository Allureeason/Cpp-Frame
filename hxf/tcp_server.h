#ifndef __HXF_TCP_SERVER_H__
#define __HXF_TCP_SERVER_H__

#include <memory>
#include <vector>
#include <functional>
#include "noncopyable.h"
#include "iomanager.h"
#include "address.h"
#include "socket.h"

namespace hxf {

class TcpServer : public std::enable_shared_from_this<TcpServer>
                    , Noncopyable {
public:
    typedef std::shared_ptr<TcpServer> ptr;
    TcpServer(IOManager* worker = IOManager::GetThis(),
            IOManager* accept_worker = IOManager::GetThis());
    virtual ~TcpServer();

    virtual bool bind(hxf::Address::ptr addr);
    virtual bool bind(const std::vector<Address::ptr>& addrs,
            std::vector<Address::ptr>& fails);

    virtual bool start();
    virtual void stop();

    uint64_t getReadTimeout() const { return m_readTimeout;}
    std::string getName() const { return m_name;}

    void setReadTimeout(uint64_t v) { m_readTimeout = v;}
    void setName(const std::string& v) { m_name = v;}

    bool isStop() const { return m_isStop;}
protected:
    virtual void handleClient(Socket::ptr client);
    virtual void startAccept(Socket::ptr sock);

private:
    std::vector<Socket::ptr> m_socks;
    IOManager* m_worker;
    IOManager* m_acceptWorker;
    uint64_t m_readTimeout;
    std::string m_name;
    bool m_isStop;
};


}

#endif
