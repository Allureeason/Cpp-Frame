#ifndef __HXF_UTIL_H__
#define __HXF_UTIL_H__

#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <stdint.h>

#include <vector>
#include <sstream>
#include <string>

namespace hxf {

pid_t GetThreadId();
uint32_t GetFiber();

void Backtrace(std::vector<std::string>& bt, int size = 64, int skip = 2);
std::string BacktraceToString(int size = 64, int skip = 2, const std::string& prefix = "");

}

#endif
