#include <iostream>
#include <string>
#include "../hxf/log.h"
#include "../hxf/util.h"
#include "../hxf/singleton.h"

int main() {
    hxf::Logger::ptr logger(new hxf::Logger());

    hxf::LogAppender::ptr appender(new hxf::StdoutLogAppender);
    hxf::LogAppender::ptr fileAppender(new hxf::FileLogAppender("./log.txt")); 
//    std::string str = "hello hxf log";

//    hxf::LogEvent::ptr event(new hxf::LogEvent(__FILE__, __LINE__, 0, (uint32_t)hxf::GetThreadId(), hxf::GetFiber(), time(0)));

    logger->addAppender(appender);
    logger->addAppender(fileAppender);

//    logger->log(hxf::LogLevel::Level::DEBUG, event);

   //    HXF_LOG_DEBUG(logger) << "hello hxf log -- debug";
   //    HXF_LOG_INFO(logger) << "hello hxf log -- info";
   //    HXF_LOG_WARN(logger) << "hello hxf log -- warn";
   //    HXF_LOG_ERROR(logger) << "hello hxf log -- error";
   //    HXF_LOG_FATAL(logger) << "hello hxf log -- fatal";

    HXF_LOG_FMT_DEBUG(logger, "test hxf log %s", "DEBUG");
    HXF_LOG_FMT_INFO(logger, "test hxf log %s", "INFO");
    HXF_LOG_FMT_WARN(logger, "test hxf log %s", "WARN");
    HXF_LOG_FMT_ERROR(logger, "test hxf log %s", "ERROR");
    HXF_LOG_FMT_FATAL(logger, "test hxf log %s", "FATAL");

    auto l = hxf::LoggerMgr::GetInstance()->getLogger("xx");
    HXF_LOG_INFO(l) << "logger manager.";

    HXF_LOG_INFO(HXF_LOG_ROOT()) << "Get root to log";


    return 0;
}
