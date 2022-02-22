#include "hxf/hxf.h"
#include "hxf/http/http.h"
#include <iostream>

void test_req() {
    hxf::http::HttpRequest::ptr req(new hxf::http::HttpRequest);
    req->setBody("hello hxf");
    req->setPath("/index.html");
    req->setHeader("host", "www.baidu.com");
    req->dump(std::cout) << std::endl;
}

void test_resp() {
    hxf::http::HttpResponse::ptr resp(new hxf::http::HttpResponse);
    resp->setHeader("x-x", "hxf");
    resp->setStatus((hxf::http::HttpStatus)403);
    resp->setBody("hello hxf");
    resp->dump(std::cout) << std::endl;
}

int main(int argc, char** argv) {
    //test_req();
    test_resp();
    return 0;
}
