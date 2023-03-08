#pragma once
#include <cstdint>

namespace ExCore {
    struct Color24;
    struct Color32 {
        static const Color32 White;
        static const Color32 Black;
        static const Color32 Gray;
        static const Color32 Clear;
        static const Color32 Red;
        static const Color32 Green;
        static const Color32 Blue;
        static const Color32 Magenta;
        static const Color32 Yellow;
        static const Color32 Cyan;

        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;

        Color32();
        Color32(const uint32_t rgba);
        Color32(const uint8_t gray);
        Color32(const uint8_t r, const uint8_t g, const uint8_t b);
        Color32(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a);
        Color32(const Color24& rgb);
        Color32(const Color24& rgb, const uint8_t alpha);

        operator uint32_t() const { return *reinterpret_cast<const uint32_t*>(this); }
        operator Color24() const;

        bool operator==(const Color32& other) const { 
            return static_cast<const uint32_t>(*this) == static_cast<const uint32_t>(other); 
        };
        bool operator!=(const Color32& other) const { 
            return static_cast<const uint32_t>(*this) != static_cast<const uint32_t>(other); 
        };

        bool operator<(const Color32& other) const { 
            return static_cast<const uint32_t>(*this) < static_cast<const uint32_t>(other); 
        };
        bool operator>(const Color32& other) const {
            return static_cast<const uint32_t>(*this) > static_cast<const uint32_t>(other);
        };

        bool operator==(const Color24& other) const;
        bool operator!=(const Color24& other) const;

        bool operator<(const Color24& other) const;
        bool operator>(const Color24& other) const;

        void flipRB();
    };
}