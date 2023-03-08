#include <GL/glew.h>
#include <ExCore/Rendering/Texture.h>

namespace ExCore {
    uint32_t textureFormatToGLFormat(const TextureFormat& fmt, const bool isMain = true) {
        switch (fmt)
        {
            default:                    return isMain ? GL_RGBA8 : GL_RGBA;
            case TextureFormat::RGB24:  return isMain ? GL_RGB8 : GL_RGB;
            case TextureFormat::Indexed: return isMain ? GL_R8 : GL_RED;
        }
    }

    uint32_t getGLPixelAlignment(const TextureFormat& fmt) {
        switch (fmt)
        {
            default:                    return 4;

            case TextureFormat::Indexed:
            case TextureFormat::RGB24:  return 1;
        }
    }


    Texture::Texture() : _width(0), _height(0), _textureId(0), _paletteId(0), _format(TextureFormat::Unknown) { }
    Texture::~Texture() {
        release();
    }

    bool Texture::create(const uint8_t* input, const TextureFormat format, const uint16_t width, const uint16_t height) {
        const bool isIndexed = format == TextureFormat::Indexed;
        const uint8_t* pixels = input + (isIndexed ? 256 * 4 : 0);

        if (!_textureId) {
            glGenTextures(1, &_textureId);
        }

        _format = format;
        _width = width;
        _height = height;

        glBindTexture(GL_TEXTURE_2D, _textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glPixelStorei(GL_UNPACK_ALIGNMENT, getGLPixelAlignment(format));
        glTexImage2D(GL_TEXTURE_2D, 0, textureFormatToGLFormat(format, true), width, height, 0, textureFormatToGLFormat(format, false), GL_UNSIGNED_BYTE, pixels);

        if (isIndexed) {
            if (!_paletteId) {
                glGenTextures(1, &_paletteId);
            }

            glBindTexture(GL_TEXTURE_1D, _paletteId);
            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
            glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, input);
        }
        else {
            releasePalette();
        }
        return true;
    }

    uint32_t Texture::bind(uint32_t slot) const {
        glActiveTexture(GL_TEXTURE0 + slot++);
        glBindTexture(GL_TEXTURE_2D, _textureId);

        if (_format == TextureFormat::Indexed) {
            glActiveTexture(GL_TEXTURE0 + slot++);
            glBindTexture(GL_TEXTURE_1D, _paletteId);;
        }
        return slot;
    }

    uint32_t Texture::unBind(uint32_t slot) const {
        glActiveTexture(GL_TEXTURE0 + slot++);
        glBindTexture(GL_TEXTURE_2D, 0);

        if (_format == TextureFormat::Indexed) {
            glActiveTexture(GL_TEXTURE0 + slot++);
            glBindTexture(GL_TEXTURE_1D, 0);;
        }
        return slot;
    }

    void Texture::release() {
        releaseTexture();
        releasePalette();
    }

    void Texture::releaseTexture() {
        if (_textureId) {
            glDeleteTextures(1, &_textureId);
            _textureId = 0;
        }
    }

    void Texture::releasePalette() {
        if (_paletteId) {
            glDeleteTextures(1, &_paletteId);
            _paletteId = 0;
        }
    }


}