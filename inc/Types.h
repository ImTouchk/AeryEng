#ifndef LUNAR_TYPES_H
#define LUNAR_TYPES_H

#include <cstdint>

using u32 = uint32_t;
using u64 = uint64_t;
using i32 = int32_t;
using i64 = int64_t;

template<typename _Ty>
class range {
private:
    _Ty last;
    _Ty iter;

public:
    range(_Ty start, _Ty end) : last(end), iter(start) {}
    range(const range& other) : last(other.last), iter(other.iter) {}

    const range& begin() const { return *this; }
    const range& end() const { return *this; }

    bool operator!=(const range&) const { return iter < last; }
    void operator++() { ++iter; }
    _Ty operator*() const { return iter; }
};

#endif