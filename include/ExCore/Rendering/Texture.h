#pragma once
#include <cstdint>

namespace ExCore {
    enum class TextureFormat : uint8_t {
        Unknown,

        RGB24,
        RGBA32,
        Indexed,
    };

    constexpr int32_t getBitsPerPixel(const TextureFormat format) {
        switch (format) {
            default: return 0;

            case TextureFormat::Indexed: return 8;
            case TextureFormat::RGB24: return 24;
            case TextureFormat::RGBA32: return 32;
        }
    }

    constexpr const char* getTextureFormatName(const TextureFormat format) {
        switch (format) {
            default:						return "Unknown";

            case TextureFormat::Indexed:	return "Indexed";
  
            case TextureFormat::RGB24:		return "RGB24";
            case TextureFormat::RGBA32:		return "RGBA32";
        }
    }

    class Texture {
    public:
        Texture();
        ~Texture();

        TextureFormat getFormat() const { return _format; }

        bool create(const uint8_t* input, const TextureFormat format, const uint16_t width, const uint16_t height);

        uint16_t getWidth() const { return _width; }
        uint16_t getHeight() const { return _height; }

        uint32_t getTextureId() const { return _textureId; }
        uint32_t getPaletteId() const { return _paletteId; }

        uint32_t bind(uint32_t slot) const;
        uint32_t unBind(uint32_t slot) const;

        void release();
    private:
        uint32_t _textureId;
        uint32_t _paletteId;

        uint16_t _width;
        uint16_t _height;
        TextureFormat _format;

        void releaseTexture();
        void releasePalette();
    };
}