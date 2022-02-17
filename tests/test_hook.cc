#include "hxf/hook.h"
#include "hxf/log.h"
#include "hxf/iomanager.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>


hxf::Logger::ptr g_logger = HXF_LOG_ROOT();

void test_sleep() {
    hxf::IOManager iom(1);
    iom.schedule([](){
        sleep(2);
        HXF_LOG_INFO(g_logger) << "sleep 2";
    });

    iom.schedule([](){
        sleep(3);
        HXF_LOG_INFO(g_logger) << "sleep 3";
    });
    HXF_LOG_INFO(g_logger) << "test_sleep";
}

void test_sock() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    inet_pton(AF_INET, "183.232.231.174", &addr.sin_addr.s_addr);

    HXF_LOG_INFO(g_logger) << "begin connect";
    int rt = connect(sock, (const sockaddr*)&addr, sizeof(addr));
    HXF_LOG_INFO(g_logger) << "sock=" << sock << " connect rt=" << rt << " errno=" << errno;

    if(rt) {
        return;
    }

    const char data[] = "GET / HTTP/1.0\r\n\r\n";
    rt = send(sock, data, sizeof(data), 0);
    HXF_LOG_INFO(g_logger) << "send rt=" << rt << " errno=" << errno;

    if(rt <= 0) {
        return;
    }

    std::string buff;
    buff.resize(4096);
    
    rt = recv(sock, &buff[0], buff.size(), 0);
    HXF_LOG_INFO(g_logger) << "recv rt=" << rt << " errno=" << errno;

    if(rt <= 0) {
        return;
    }

    buff.resize(rt);
    HXF_LOG_INFO(g_logger) << buff;
}

int main(int argc, char** argv) {
    //test_sleep();
    hxf::IOManager iom;

    iom.addTimer(1000, &test_sock, true);


    return 0;
}
