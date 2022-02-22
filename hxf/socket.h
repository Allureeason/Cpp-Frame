#ifndef __HXF_SOCKET_H__
#define __HXF_SOCKET_H__

#include <memory>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "address.h"
#include "noncopyable.h"

namespace hxf {

class Socket : public std::enable_shared_from_this<Socket>, Noncopyable {
public:
    typedef std::shared_ptr<Socket> ptr;
    typedef std::weak_ptr<Socket> weak_ptr;

    /**
     * @brief Socket类型
     */
    enum Type {
        /// TCP类型
        TCP = SOCK_STREAM,
        /// UDP类型
        UDP = SOCK_DGRAM
    };

    /**
     * @brief Socket协议簇
     */
    enum Family {
        /// IPv4 socket
        IPv4 = AF_INET,
        /// IPv6 socket
        IPv6 = AF_INET6,
        /// Unix socket
        UNIX = AF_UNIX,
    };

    /**
     * @brief 创建TCP Socket(满足地址类型)
     * @param[in] address 地址
     */
    static Socket::ptr CreateTCP(hxf::Address::ptr address);

    /**
     * @brief 创建UDP Socket(满足地址类型)
     * @param[in] address 地址
     */
    static Socket::ptr CreateUDP(hxf::Address::ptr address);

    /**
     * @brief 创建IPv4的TCP Socket
     */
    static Socket::ptr CreateTCPSocket();

    /**
     * @brief 创建IPv4的UDP Socket
     */
    static Socket::ptr CreateUDPSocket();

    /**
     * @brief 创建IPv6的TCP Socket
     */
    static Socket::ptr CreateTCPSocket6();

    /**
     * @brief 创建IPv6的UDP Socket
     */
    static Socket::ptr CreateUDPSocket6();

    /**
     * @brief 创建Unix的TCP Socket
     */
    static Socket::ptr CreateUnixTCPSocket();

    /**
     * @brief 创建Unix的UDP Socket
     */
    static Socket::ptr CreateUnixUDPSocket();
    
    Socket(int family, int type, int protocol = 0);
    virtual ~Socket();

    void setSendTimeout(uint64_t ms);
    int64_t getSendTimeout();

    void setRecvTimeout(uint64_t ms);
    int64_t RecvSendTimeout();

    bool getOption(int level, int option, void* result, socklen_t* len);
    template<class T>
    bool getOption(int level, int option, T& result) {
        socklen_t len = sizeof(T);
        return getOption(level, option, &result, &len);
    }

    void setOption(int level, int option, const void* result, socklen_t len);
    template<class T>
    void setOption(int level, int option, const T& result) {
        setOption(level, option, &result, sizeof(T));
    }

    virtual Socket::ptr accept();
    virtual bool bind(const Address::ptr addr);
    virtual bool connect(const Address::ptr addr, uint64_t timeout_ms = -1);
    virtual bool listen(int backlog = SOMAXCONN);
    virtual bool close();

    int send(const void* buffer, size_t length, int flags = 0);
    int send(const iovec* buffers, size_t length, int flags = 0);
    int sendTo(const void* buffer, size_t length, const Address::ptr to, int flags = 0);
    int sendTo(const iovec* buffers, size_t length, const Address::ptr to, int flags = 0);
    
    int recv(void* buffer, size_t length, int flags = 0);
    int recv(iovec* buffers, size_t length, int flags = 0);
    int recvFrom(void* buffer, size_t length, Address::ptr from, int flags = 0);
    int recvFrom(iovec* buffers, size_t length, Address::ptr from, int flags = 0);

    Address::ptr getRemoteAddress();
    Address::ptr getLocalAddress();

    int getSock() const { return m_sock;}
    int getFamily() const { return m_family;}
    int getType() const { return m_type;}
    int getProtocol() const { return m_protocol;}
    bool isConnected() const { return m_isConnected;}
    bool isValid() const;
    int getError();

    bool cancelRead();
    bool cancelWrite();
    bool cancelAccept();
    bool cancelAll();

    std::ostream& dump(std::ostream& os) const;
    std::string toString() const;
protected:
    void initSock();
    void newSock();
    bool init(int sock);
private:
    int m_sock;
    int m_family;
    int m_type;
    int m_protocol;
    bool m_isConnected;
    Address::ptr m_localAddress;
    Address::ptr m_remoteAddress;
};

/**
 * @brief 流式输出socket
 * @param[in, out] os 输出流
 * @param[in] sock Socket类
 */
std::ostream& operator<<(std::ostream& os, const Socket& sock);

}

#endif
