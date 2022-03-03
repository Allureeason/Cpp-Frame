#include "hxf/hxf.h"
#include "hxf/http/http_connection.h"

static hxf::Logger::ptr g_logger = HXF_LOG_ROOT();

void test_pool() {
    hxf::http::HttpConnectionPool::ptr pool(new hxf::http::HttpConnectionPool(
                "www.sylar.top", "", 80, false, 10, 1000 * 30, 5));

    hxf::IOManager::GetThis()->addTimer(1000, [pool](){
            auto r = pool->doGet("/", 300);
            HXF_LOG_INFO(g_logger) << r->toString();
    }, true);
}

void run() {
    hxf::Address::ptr addr = hxf::Address::LookupAnyIPAddress("www.sylar.top:80");
    if(!addr) {
        HXF_LOG_INFO(g_logger) << "get addr error";
        return;
    }

    hxf::Socket::ptr sock = hxf::Socket::CreateTCP(addr);
    bool rt = sock->connect(addr);
    if(!rt) {
        HXF_LOG_INFO(g_logger) << "connect " << *addr << " failed";
        return;
    }

    hxf::http::HttpConnection::ptr conn(new hxf::http::HttpConnection(sock));
    hxf::http::HttpRequest::ptr req(new hxf::http::HttpRequest);
    req->setPath("/blog/");
    req->setHeader("host", "www.sylar.top");
    HXF_LOG_INFO(g_logger) << "req:" << std::endl
        << *req;

    conn->sendRequest(req);
    auto rsp = conn->recvResponse();

    if(!rsp) {
        HXF_LOG_INFO(g_logger) << "recv response error";
        return;
    }
    HXF_LOG_INFO(g_logger) << "rsp:" << std::endl
        << *rsp;

    std::ofstream ofs("rsp.dat");
    ofs << *rsp;

    HXF_LOG_INFO(g_logger) << "=========================";

    auto r = hxf::http::HttpConnection::DoGet("http://www.sylar.top/blog/", 300);
    HXF_LOG_INFO(g_logger) << "result=" << r->result
        << " error=" << r->error
        << " rsp=" << (r->response ? r->response->toString() : "");

    HXF_LOG_INFO(g_logger) << "=========================";
    test_pool();
}

int main(int argc, char** argv) {
    hxf::IOManager iom;
    iom.schedule(run);
    return 0;
}
