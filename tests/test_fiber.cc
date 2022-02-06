#include "hxf/fiber.h"
#include "hxf/log.h"

#include <vector>


hxf::Logger::ptr g_logger = HXF_LOG_ROOT();

void run_in_fiber() {
    HXF_LOG_INFO(g_logger) << "run_in_fiber begin";
    hxf::Fiber::YieldToHold();
    HXF_LOG_INFO(g_logger) << "run_in_fiber end";
    hxf::Fiber::YieldToHold();
}


void test_fiber() {
    HXF_LOG_INFO(g_logger) << "main begin -1";
    {
        hxf::Fiber::GetThis();
        HXF_LOG_INFO(g_logger) << "main begin";
        hxf::Fiber::ptr fiber(new hxf::Fiber(run_in_fiber));
        fiber->swapIn();
        HXF_LOG_INFO(g_logger) << "main after swapIn";
        fiber->swapIn();
        HXF_LOG_INFO(g_logger) << "main after end";
        fiber->swapIn();
    }
    HXF_LOG_INFO(g_logger) << "main after end2";
}


int main(int argc, char** argv) {
    hxf::Thread::SetName("main");
    {
        hxf::Fiber::ptr fiber(new hxf::Fiber(run_in_fiber));
    }
    std::vector<hxf::Thread::ptr> thrs;
    for(int i = 0; i < 3; i++) {
        thrs.push_back(hxf::Thread::ptr(
                    new hxf::Thread(&test_fiber, "name_" + std::to_string(i))));
    }

    for(auto i : thrs) {
        i->join();
    }

    return 0;
}
