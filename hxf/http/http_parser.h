#ifndef __HXF_HTTP_PARSER_H__
#define __HXF_HTTP_PARSER_H__

#include "http.h"
#include "http11_parser.h"
#include "httpclient_parser.h"

#include <memory>

namespace hxf {
namespace http {

class HttpRequestParser {
public:
    typedef std::shared_ptr<HttpRequestParser> ptr;
    HttpRequestParser();

    static uint64_t GetHttpRequestBufferSize();
    static uint64_t GetHttpRequestMaxBodySize();

    size_t execute(char* data, size_t len);
    int isFinished();
    bool hasError();
    uint64_t getContentLength();
    HttpRequest::ptr getData() const { return m_data;}
    const http_parser& getParser() const { return m_parser;}
    void setError(int v) { m_error = v;}
private:
    http_parser m_parser;
    HttpRequest::ptr m_data;
    int m_error;
};

class HttpResponseParser {
public:
    typedef std::shared_ptr<HttpResponseParser> ptr;
    HttpResponseParser();

    static uint64_t GetHttpResponseBufferSize();
    static uint64_t GetHttpResponseMaxBodySize();

    size_t execute(char* data, size_t len, bool chunck);
    int isFinished();
    bool hasError();
    uint64_t getContentLength();
    HttpResponse::ptr getData() const { return m_data;}
    const httpclient_parser& getParser() const { return m_parser;}
    void setError(int v) { m_error = v;}
private:
    httpclient_parser m_parser;
    HttpResponse::ptr m_data;
    int m_error;
};

}
}


#endif
