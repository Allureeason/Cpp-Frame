#ifndef __HXF_LOG_H__
#define __HXF_LOG_H__

#include "util.h"

#include <string>
#include <memory>
#include <fstream>
#include <sstream>
#include <stdint.h>
#include <vector>
#include <list>
#include <map>
#include "singleton.h"


// 数据流形式
#define HXF_LOG_LEVEL(logger, level) \
    if (logger->getLevel() <= level) \
        hxf::LogEventWrap(hxf::LogEvent::ptr(new hxf::LogEvent(logger, \
        level, __FILE__, __LINE__, 0, hxf::GetThreadId(),\
        hxf::GetFiber(), time(0)))).getSS()

#define HXF_LOG_DEBUG(logger) HXF_LOG_LEVEL(logger, hxf::LogLevel::DEBUG)
#define HXF_LOG_INFO(logger) HXF_LOG_LEVEL(logger, hxf::LogLevel::INFO)
#define HXF_LOG_WARN(logger) HXF_LOG_LEVEL(logger, hxf::LogLevel::WARN)
#define HXF_LOG_ERROR(logger) HXF_LOG_LEVEL(logger, hxf::LogLevel::ERROR)
#define HXF_LOG_FATAL(logger) HXF_LOG_LEVEL(logger, hxf::LogLevel::FATAL)

// format形式
#define HXF_LOG_FMT_LEVEL(logger, level, fmt, ...) \
    if (logger->getLevel() <= level) \
        hxf::LogEventWrap(hxf::LogEvent::ptr(new hxf::LogEvent(logger, \
        level, __FILE__, __LINE__, 0, hxf::GetThreadId(),\
        hxf::GetFiber(), time(0)))).getEvent()->format(fmt, __VA_ARGS__)

#define HXF_LOG_FMT_DEBUG(logger, fmt, ...) HXF_LOG_FMT_LEVEL(logger, hxf::LogLevel::DEBUG, fmt, __VA_ARGS__)
#define HXF_LOG_FMT_INFO(logger, fmt, ...) HXF_LOG_FMT_LEVEL(logger, hxf::LogLevel::INFO, fmt, __VA_ARGS__)
#define HXF_LOG_FMT_WARN(logger, fmt, ...) HXF_LOG_FMT_LEVEL(logger, hxf::LogLevel::WARN, fmt, __VA_ARGS__)
#define HXF_LOG_FMT_ERROR(logger, fmt, ...) HXF_LOG_FMT_LEVEL(logger, hxf::LogLevel::ERROR, fmt, __VA_ARGS__)
#define HXF_LOG_FMT_FATAL(logger, fmt, ...) HXF_LOG_FMT_LEVEL(logger, hxf::LogLevel::FATAL, fmt, __VA_ARGS__)

// 获取默认日志
#define HXF_LOG_ROOT() hxf::LoggerMgr::GetInstance()->getRoot()

namespace hxf {

// 日志等级
class LogLevel {
public:
    enum Level {
        UNKNOW = 0,
        DEBUG = 1,
        INFO = 2,
        WARN = 3,
        ERROR = 4,
        FATAL = 5
    };
    static const char* ToString(LogLevel::Level level);
};

class Logger;
// 日志事件
class LogEvent {
public:
    typedef std::shared_ptr<LogEvent> ptr;
    LogEvent(std::shared_ptr<Logger> m_logger, LogLevel::Level level,
            const char* file, uint32_t line, uint64_t elapse,
            uint32_t thread_id, uint32_t fiber_id, uint64_t time);
    const char* getFile() const { return m_file;}
    uint32_t getLine() const { return m_line;}
    uint64_t getElapse() const { return m_elapse;}
    uint32_t getThreadId() const { return m_threadId;}
    uint32_t getFiberId() const { return m_fiberId;}
    uint64_t getTime() const { return m_time;}
    std::string getContent() const { return m_ss.str();}
    std::stringstream& getSS() { return m_ss;}
    std::shared_ptr<Logger> getLogger() const { return m_logger;}
    LogLevel::Level getLevel() const { return m_level;}

    void format(const char* fmt, ...);
    void format(const char* fmt, va_list al);
private:
    const char* m_file = nullptr;  // 文件名
    uint32_t m_line = 0;           // 行号
    uint64_t m_elapse = 0;         // 程序启动到现在的毫秒数
    uint32_t m_threadId = 0;       // 线程号
    uint32_t m_fiberId = 0;        // 协程号
    uint64_t m_time = 0;           // 时间戳
    std::stringstream m_ss;        // 内容字符串流
    std::shared_ptr<Logger> m_logger;
    LogLevel::Level m_level;
};

class LogEventWrap {
public:
    LogEventWrap(LogEvent::ptr e);
    ~LogEventWrap();

    LogEvent::ptr getEvent() const { return m_event;}
    std::stringstream& getSS();
private:
    LogEvent::ptr m_event;
};

// 日志格式
class LogFormatter {
public:
    typedef std::shared_ptr<LogFormatter> ptr;
    LogFormatter(const std::string& pattern);
    std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);

    void init();

public:
    class FormatItem {
    public:
        typedef std::shared_ptr<FormatItem> ptr;
        virtual ~FormatItem(){}
        virtual void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0; 
    };

    bool isError() const { return m_error;}

    const std::string getPattern() const { return m_pattern;}
private:
    std::string m_pattern;
    std::vector<FormatItem::ptr> m_items;
    bool m_error;
};


// 日志输出地
class LogAppender {
public:
    typedef std::shared_ptr<LogAppender> ptr;
    virtual ~LogAppender(){}
    
    virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level, const LogEvent::ptr event) = 0;
    
    void setFormatter(LogFormatter::ptr val) { m_formatter = val; }
    LogFormatter::ptr getFormatter() const { return m_formatter; }
protected:
    LogLevel::Level m_level = LogLevel::Level::DEBUG;
    LogFormatter::ptr m_formatter;
};


// 日志器
class Logger : public std::enable_shared_from_this<Logger> {
public:
    typedef std::shared_ptr<Logger> ptr;
    Logger(const std::string& name = "root");    
    void log(LogLevel::Level level, const LogEvent::ptr event);

    void debug(const LogEvent::ptr event);
    void info(const LogEvent::ptr event);
    void wran(const LogEvent::ptr event);
    void error(const LogEvent::ptr event);
    void fatal(const LogEvent::ptr event);

    void addAppender(LogAppender::ptr appender);
    void delAppender(LogAppender::ptr appender);

    std::string getName() const { return m_name;}
    LogLevel::Level getLevel() const { return m_level;}
    LogFormatter::ptr getFormatter() const { return m_formatter;}
private:
    std::string m_name;
    LogLevel::Level m_level;
    std::list<LogAppender::ptr> m_appenders;
    LogFormatter::ptr m_formatter;
};

// 输出到控制台
class StdoutLogAppender : public LogAppender {
public:
    typedef std::shared_ptr<StdoutLogAppender> ptr;
    void log(std::shared_ptr<Logger> logger, LogLevel::Level level, const LogEvent::ptr event) override;
};

// 输出到文件
class FileLogAppender : public LogAppender {
public:
    typedef std::shared_ptr<FileLogAppender> ptr;
    FileLogAppender(const std::string& filename);
    void log(std::shared_ptr<Logger> logger, LogLevel::Level level, const LogEvent::ptr event) override;
    bool reopen();
private:
    std::string m_filename;
    std::ofstream m_ofstream;
};

class LoggerManager {
public:
    LoggerManager();
    Logger::ptr getLogger(const std::string& name);
    void init() {}
    Logger::ptr getRoot() const { return m_root;}
private:
    Logger::ptr m_root;
    std::map<std::string, Logger::ptr> m_loggers;
};
// 日志器管理类单例模式
typedef hxf::Singleton<LoggerManager> LoggerMgr; 

}

#endif
