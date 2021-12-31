#ifndef __HXF_UTIL_H__
#define __HXF_LOG_H__

#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <stdint.h>

namespace hxf {

pid_t GetThreadId();
uint32_t GetFiber();

}

#endif
