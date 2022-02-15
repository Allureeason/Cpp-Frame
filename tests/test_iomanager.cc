#include "hxf/log.h"
#include "hxf/iomanager.h"
#include "hxf/scheduler.h"

static hxf::Logger::ptr g_logger = HXF_LOG_ROOT();

void test_fiber() {
    HXF_LOG_INFO(g_logger) << "test fiber";
}

void test1() {
    hxf::IOManager iom(2, false, "hxf");
    iom.schedule(&test_fiber);
}

static hxf::Timer::ptr s_timer;

void test_timer() {
    hxf::IOManager iom(2, true, "test_timer");
    s_timer = iom.addTimer(500, [](){
        static int i = 0;
        HXF_LOG_INFO(g_logger) << "hello timer i=" << i;
        if(++i > 3) {
            s_timer->reset(3000, true);
        }
    }, true);
}


int main(int argc, char** argv) {

    //test1();
    test_timer();
    return 0;
}
