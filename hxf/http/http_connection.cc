#include "http_connection.h"
#include "http_parser.h"
#include "hxf/log.h"

namespace hxf {
namespace http {

static hxf::Logger::ptr g_logger = HXF_LOG_NAME("system");

HttpConnection::HttpConnection(Socket::ptr sock, bool owner)
    :SocketStream(sock, owner) {
}

HttpResponse::ptr HttpConnection::recvResponse() {
    HttpResponseParser::ptr parser(new HttpResponseParser);
    uint64_t buffer_size = HttpResponseParser::GetHttpResponseBufferSize();
    std::shared_ptr<char> buffer(
        new char[buffer_size], [](char* ptr){
            delete[] ptr;
        });
    char* data = buffer.get();
    int offset = 0;
    do {
        int len = read(data + offset, buffer_size - offset);
        if(len <= 0) {
            close();
            return nullptr;
        }
        len += offset;
        size_t nparse = parser->execute(data, len, false);
        if(parser->hasError()) {
            HXF_LOG_INFO(g_logger) << "Parser response has error:"
                << parser->hasError();
            close();
            return nullptr;
        }
        offset = len - nparse;
        if(offset == (int)buffer_size) {
            close();
            return nullptr;
        }
        if(parser->isFinished()) {
            break;
        }
    } while(true);
    int length = parser->getContentLength();
    if(length > 0) {
        std::string body;
        body.resize(length);

        int len = 0;
        if(length >= offset) {
            memcpy(&body[0], data, offset);
            len = offset;
        } else {
            memcpy(&body[0], data, length);
        }
        length -= offset;
        if(length > 0) {
            if(readFixSize(&body[len], length) <= 0) {
                close();
                return nullptr;
            }
        }
        parser->getData()->setBody(body);
    }

    //parser->getData()->init();
    return parser->getData();
}

int HttpConnection::sendRequest(HttpRequest::ptr req) {
    std::stringstream ss;
    ss << *req;
    std::string data = ss.str();
    return writeFixSize(data.c_str(), data.size());
}

}
}
