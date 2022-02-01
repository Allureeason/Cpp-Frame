#include "hxf/util.h"
#include "hxf/log.h"


hxf::Logger::ptr g_logger = HXF_LOG_NAME("root");

int main(int argc, char** argv) {

    HXF_LOG_INFO(g_logger) << "test global log";

    return 0;
}
