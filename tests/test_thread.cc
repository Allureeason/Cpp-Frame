#include "../hxf/log.h"
#include "../hxf/config.h"
#include "../hxf/thread.h"
#include "../hxf/util.h"

#include <vector>

static hxf::Logger::ptr g_logger = HXF_LOG_ROOT();

int count = 0;
hxf::RWMutex s_rwmutex;
hxf::Mutex s_mutex;
void fun1() {
    HXF_LOG_INFO(g_logger) << "name=" << hxf::Thread::GetName()
                           << " this.name: " << hxf::Thread::GetThis()->getName()
                           << " id: " << hxf::GetThreadId()
                           << " this.id" << hxf::Thread::GetThis()->getId();
    for(int i = 0; i < 100000; ++i) {
        //hxf::RWMutex::WriteLock lock(s_rwmutex);
        hxf::Mutex::Lock lock(s_mutex);
        ++count;
    }
}

void fun2() {
    while(true) {
        HXF_LOG_INFO(g_logger) << "*********************************";    
    }
}

void fun3() {
    while(true) {
        HXF_LOG_INFO(g_logger) << "=================================";
    }
}


int main(int argc, char** argv) {

    std::vector<hxf::Thread::ptr> thrs;

    HXF_LOG_INFO(g_logger) << "thread create begin.";
    for(int i = 0; i < 5; i++) {
        hxf::Thread::ptr thr1(new hxf::Thread(fun2, "name" + std::to_string(i)));
        hxf::Thread::ptr thr2(new hxf::Thread(fun3, "name" + std::to_string(i)));
        thrs.push_back(thr1);
        thrs.push_back(thr2);
    }

    for(auto& i : thrs) {
        i->join();
    }
    HXF_LOG_INFO(g_logger) << "thread create end.";
    HXF_LOG_INFO(g_logger) << "count = " << count;


    return 0;
}
