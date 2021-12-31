#include "util.h"

namespace hxf {


pid_t GetThreadId() {
    return syscall(SYS_gettid);
}

uint32_t GetFiber() {
    return 0;
}

}
