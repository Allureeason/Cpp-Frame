#include <assert.h>


#include <iostream>
#include "hxf/util.h"
#include "hxf/log.h"
#include "hxf/macro.h"

//#define HXF_LOG_INFO(g_logger) std::cout


static hxf::Logger::ptr g_logger = HXF_LOG_NAME("root");


void test_assert() {
    HXF_LOG_INFO(g_logger) << hxf::BacktraceToString(10, 2, "-----");
    //HXF_ASSERT(false);
    HXF_ASSERT2(false, "test assert2");
}


int main(int argc, char** argv) {
    
    test_assert();

    return 0;
}
