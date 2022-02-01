#ifndef __HXF_MACRO_H__
#define __HXF_MACRO_H__

#include <string.h>
#include <assert.h>

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
