module;
#include <cstdint>
export module Lunar:Numbers;

template<typename _Ty>
struct base_num {
    using val = base_num<_Ty>;
    using ref = val&;

    base_num(ref Other) {
        m_Value = Other.m_Value;
    }

    base_num(_Ty Value) {
        m_Value = Value;
    }

    base_num() {
        m_Value = 0;
    }

    ref operator=(const ref Other) {
        m_Value = Other.m_Value;
        return *this;
    }

    ref operator=(const _Ty Value) {
        m_Value = Value;
        return *this;
    }

    template<typename _Ty2>
    val operator+(const base_num<_Ty2>& Other) {
        val Result = val(m_Value + static_cast<_Ty>(Other.m_Value));
        return Result;
    }

#define OVERLOAD_OPERATOR(op) \
    template<typename _Ty2> \
    val operator op(const _Ty2 Value) { \
        val Result = val( m_Value op static_cast<_Ty>(Value) ); \
        return Result; \
    } \
    val operator op(const ref Other) { \
        val Result = val( m_Value op Other.m_Value ); \
        return Result; \
    } \
    val operator op(const _Ty Value) { \
        val Result = val( m_Value op Value ); \
        return Result; \
    }

    OVERLOAD_OPERATOR(+) OVERLOAD_OPERATOR(-)
    OVERLOAD_OPERATOR(*) OVERLOAD_OPERATOR(/ )
    OVERLOAD_OPERATOR(&) OVERLOAD_OPERATOR(^)
    OVERLOAD_OPERATOR(%) OVERLOAD_OPERATOR(| )

    ref operator--() {
        m_Value--;
        return *this;
    }

    val operator--(int) {
        val Result = val(m_Value--);
        return Result;
    }

    ref operator++() {
        m_Value++;
        return *this;
    }

    val operator++(int) {
        val Result = val(m_Value++);
        return Result;
    }

    template<typename _Ty2>
    base_num(base_num<_Ty>& Other) {
        m_Value = static_cast<_Ty>(Other.m_Value);
    }

    template<typename _Ty2>
    operator _Ty2() const {
        return _Ty2(m_Value);
    }

    template<typename _Ty2>
    ref operator=(const base_num<_Ty2>& Other) {
        m_Value = Other.m_Value;
        return *this;
    }

    template<typename _Ty2>
    ref operator=(const _Ty2 Value) {
        m_Value = static_cast<_Ty>(Value);
        return *this;
    }

    _Ty& value() {
        return m_Value;
    }
private:
    _Ty m_Value;
};

export namespace Lunar {
    using u32 = base_num<uint32_t>;
    using u64 = base_num<uint64_t>;

    using i32 = base_num<int32_t>;
    using i64 = base_num<int64_t>;

    using f32 = base_num<float>;
    using f64 = base_num<double>;
}
