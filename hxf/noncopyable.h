#ifndef __HXF_NONCOPYABLE_H__
#define __HXF_NONCOPYABLE_H__

namespace hxf {

class Noncopyable {
public:
    Noncopyable() = default;
    ~Noncopyable() = default;

    Noncopyable(const Noncopyable&) = delete;
    Noncopyable& operator=(const Noncopyable&) = delete;
};

}

#endif
