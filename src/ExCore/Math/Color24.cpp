#include <ExCore/Math/Color24.h>
#include <ExCore/Math/Color32.h>

namespace ExCore {
    const Color24 Color24::White(0xFF, 0xFF, 0xFF);
    const Color24 Color24::Black(0, 0, 0);
    const Color24 Color24::Gray(0x80, 0x80, 0x80);
                       
    const Color24 Color24::Red(0xFF, 0, 0);
    const Color24 Color24::Green(0, 0xFF, 0);
    const Color24 Color24::Blue(0, 0, 0xFF);
    const Color24 Color24::Magenta(0xFF, 0, 0xFF);
    const Color24 Color24::Yellow(0xFF, 0xFF, 0);
    const Color24 Color24::Cyan(0, 0xFF, 0xFF);

    Color24::Color24() : r(0), g(0), b(0) {}
    Color24::Color24(const uint8_t gray) : r(gray), g(gray), b(gray) { }
    Color24::Color24(const uint8_t r, const uint8_t g, const uint8_t b) : r(r), g(g), b(b) { }
    Color24::Color24(const Color32& rgba) : r(rgba.r), g(rgba.g), b(rgba.b) { }

    bool Color24::operator==(const Color32& other) const {
        const uint32_t rgb = static_cast<uint32_t>(*this);
        return rgb == (static_cast<uint32_t>(other) & 0xFFFFFF);
    }
    bool Color24::operator!=(const Color32& other) const { return !(*this == other); }

    bool Color24::operator<(const Color32& other) const {
        const uint32_t rgb = static_cast<uint32_t>(*this);
        return rgb < (static_cast<uint32_t>(other) & 0xFFFFFF);
    }

    bool Color24::operator>(const Color32& other) const {
        const uint32_t rgb = static_cast<uint32_t>(*this);
        return rgb > (static_cast<uint32_t>(other) & 0xFFFFFF);
    }

    void Color24::flipRB() {
        uint8_t temp = r;
        r = b;
        b = temp;
    }
}