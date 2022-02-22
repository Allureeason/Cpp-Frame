#include "hxf/http/http_server.h"
#include "hxf/hxf.h"

void run() {
    hxf::Address::ptr addr = hxf::Address::LookupAnyIPAddress("0.0.0.0:8028");
    hxf::http::HttpServer::ptr http(new hxf::http::HttpServer);
    while(!http->bind(addr)) {
        sleep(2);
    }
    http->getDispatch()->addServlet("/hxf/xx",[](hxf::http::HttpRequest::ptr request,
                                hxf::http::HttpResponse::ptr response,
                                hxf::http::HttpSession::ptr session) {
        response->setBody(request->toString());
        return 0;
    });
    http->getDispatch()->addGlobServlet("/hxf/*",[](hxf::http::HttpRequest::ptr request,
                                hxf::http::HttpResponse::ptr response,
                                hxf::http::HttpSession::ptr session) {
        response->setBody("Glob pattern\r\n");
        //response->setBody(request->toString());
        return 0;
    });

    http->start();
}

int main(int argc, char** argv) {
    hxf::IOManager iom(2);
    iom.schedule(run);

    return 0;
}
