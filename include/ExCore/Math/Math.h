#pragma once
#include <cstdint>

static inline constexpr float PI = 3.14159265359f;
static inline constexpr float DEG_2_RAD = PI / 180.0f;
static inline constexpr float RAD_2_DEG = 180.0f / PI;

namespace Math {
    int32_t log2(uint64_t value);
    int32_t findFirstLSB(const uint64_t value);

    template<typename T>
    T sign(const T& value) {
        return T(value > 0 ? 1 : value < 0 ? -1 : 0);
    }

    template<typename T, size_t size>
    constexpr inline T nextDivByPowOf2(const T input) {
        return (input + (size - 1)) & ~(size - 1);
    }

    template<typename T>
    bool isPowerOf2(const T value) { return (value & (value - 1)) == 0; }

    template<> inline bool isPowerOf2<double>(const double value) { return isPowerOf2<int64_t>(int64_t(value)); }
    template<> inline bool isPowerOf2<float>(const float value) { return isPowerOf2<int64_t>(int64_t(value)); }

    template<typename T>
    int32_t potToIndex(const T value) {
        return value != T(0) && isPowerOf2<T>(value) ? log2(uint64_t(value)) : -1;
    }

    template<> inline int32_t potToIndex<double>(const double value) { return potToIndex<int64_t>(int64_t(value)); }
    template<> inline int32_t potToIndex<float>(const float value) { return potToIndex<int64_t>(int64_t(value)); }

}