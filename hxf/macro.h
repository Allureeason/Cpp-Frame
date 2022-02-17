#ifndef __HXF_MACRO_H__
#define __HXF_MACRO_H__

#include <string.h>
#include <assert.h>

#if defined __GNUC__ || defined __llvm__
/// LIKCLY 宏的封装, 告诉编译器优化,条件大概率成立
#   define HXF_LIKELY(x)       __builtin_expect(!!(x), 1)
/// LIKCLY 宏的封装, 告诉编译器优化,条件大概率不成立
#   define HXF_UNLIKELY(x)     __builtin_expect(!!(x), 0)
#else
#   define HXF_LIKELY(x)      (x)
#   define HXF_UNLIKELY(x)      (x)
#endif

/*
 * 定义常用的宏
 *
 * */

#define HXF_ASSERT(x) \
    if(!(x)) { \
        HXF_LOG_ERROR(HXF_LOG_ROOT()) << "ASSERTION:" << #x \
        << "\nbacktrace:\n" \
        << hxf::BacktraceToString(100, 2, "       "); \
        assert(x); \
    }

#define HXF_ASSERT2(x, w) \
    if(!(x)) { \
        HXF_LOG_ERROR(HXF_LOG_ROOT()) << "ASSERTION:" << #x \
        << "\n" << w \
        << "\nbacktrace:\n" \
        << hxf::BacktraceToString(100, 2, "       "); \
        assert(x); \
    }




#endif
