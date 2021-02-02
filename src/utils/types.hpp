#pragma once

#include <cstdint>

namespace Aery {
    using u16 = const std::uint16_t;
    using u32 = const std::uint32_t;
    using u64 = const std::uint64_t;
    using i16 = const std::int16_t;
    using i32 = const std::int32_t;
    using i64 = const std::int64_t;
    using f32 = const float;
    using f64 = const double;

    using mut_u16 = std::uint16_t;
    using mut_u32 = std::uint32_t;
    using mut_u64 = std::uint64_t;
    using mut_i16 = std::int16_t;
    using mut_i32 = std::int32_t;
    using mut_i64 = std::int64_t;
    using mut_f32 = float;
    using mut_f64 = double;
}