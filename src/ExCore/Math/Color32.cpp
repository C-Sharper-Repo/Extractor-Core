#include <ExCore/Math/Color32.h>
#include <ExCore/Math/Color24.h>

namespace ExCore {
    const Color32 Color32::White(0xFF, 0xFF, 0xFF, 0xFF);
    const Color32 Color32::Black(0, 0, 0, 0xFF);
    const Color32 Color32::Gray(0x80, 0x80, 0x80, 0xFF);
    const Color32 Color32::Clear(0, 0, 0, 0);

    const Color32 Color32::Red(0xFF, 0, 0, 0xFF);
    const Color32 Color32::Green(0, 0xFF, 0, 0xFF);
    const Color32 Color32::Blue(0, 0, 0xFF, 0xFF);
    const Color32 Color32::Magenta(0xFF, 0, 0xFF, 0xFF);
    const Color32 Color32::Yellow(0xFF, 0xFF, 0, 0xFF);
    const Color32 Color32::Cyan(0, 0xFF, 0xFF, 0xFF);

    Color32::Color32() : r(0), g(0), b(0), a(0) { }
    Color32::Color32(const uint32_t rgba) : r(rgba & 0xFF), b((rgba >> 8) & 0xFF), g((rgba >> 16) & 0xFF), a((rgba >> 24) & 0xFF) { }
    Color32::Color32(const uint8_t gray) : r(gray), g(gray), b(gray), a(0xFF) { }
    Color32::Color32(const uint8_t r, const uint8_t g, const uint8_t b) : r(r), g(g), b(b), a(0xFF) { }
    Color32::Color32(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) : r(r), g(g), b(b), a(a) { }

    Color32::Color32(const Color24& rgb) : r(rgb.r), g(rgb.g), b(rgb.b), a(0xFF) { }
    Color32::Color32(const Color24& rgb, const uint8_t alpha) : r(rgb.r), g(rgb.g), b(rgb.b), a(alpha) { }

    Color32::operator Color24() const { return Color24(r, g, b); }

    bool Color32::operator==(const Color24& other) const {
        const uint32_t rgb = static_cast<uint32_t>(other);
        return rgb == (static_cast<uint32_t>(*this) & 0xFFFFFF);
    }
    bool Color32::operator!=(const Color24& other) const { return !(*this != other); }

    bool Color32::operator>(const Color24& other) const {
        const uint32_t rgb = static_cast<uint32_t>(other);
        return rgb > (static_cast<uint32_t>(*this) & 0xFFFFFF);
    }
    bool Color32::operator<(const Color24& other) const { 
        const uint32_t rgb = static_cast<uint32_t>(other);
        return rgb < (static_cast<uint32_t>(*this) & 0xFFFFFF);
    }

    void Color32::flipRB() {
        uint8_t temp = r;
        r = b;
        b = temp;
    }
}