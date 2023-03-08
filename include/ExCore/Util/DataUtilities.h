#pragma once 

namespace ExCore::Data {
    template<typename T>
    T read(const void* data) {
        return data ? *reinterpret_cast<const T*>(data) : {};
    }

    template<typename T>
    T& read(const void* data, T& value) {
        return value = read<T>(data);
    }
}