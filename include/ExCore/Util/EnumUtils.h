#pragma once

namespace ExCore {
    template<typename T>
    struct EnumNames {
        static constexpr int32_t Start{0};
        static constexpr int32_t Count{0};
        static constexpr int32_t End{Start + Count};

        static const char** getEnumNames() {
            return nullptr;
        }
    };
}