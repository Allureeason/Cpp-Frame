#include "hxf/scheduler.h"
#include "hxf/log.h"

static hxf::Logger::ptr g_logger = HXF_LOG_ROOT();

void test_fiber() {
    static int s_count = 5;

    sleep(1);
    if(--s_count) {
        hxf::Scheduler::GetThis()->schedule(&test_fiber, hxf::GetThreadId());
    }
    HXF_LOG_INFO(g_logger) << "test in fiber s_count=" << s_count;
}


int main(int argc, char** argv) {
    HXF_LOG_INFO(g_logger) << "main";
    hxf::Scheduler sc(3, false, "test");
    sc.start();
    sleep(2);

    HXF_LOG_INFO(g_logger) << "scheduler";
    sc.schedule(&test_fiber);

    sc.stop();

    HXF_LOG_INFO(g_logger) << "over";
    return 0;
}

