#pragma once
#include <cstdint>
#include <ExCore/Rendering/Texture.h>
#include <ExCore/IO/FileStream.h>

namespace ExCore {

    struct ImageData {
        uint16_t width;
        uint16_t height;
        TextureFormat format;
        uint8_t* pixels;

        void clear(bool destroy) {
            if (destroy && pixels) {
                free(pixels);
            }
            pixels = nullptr;
            format = TextureFormat::Unknown;
            width = 0;
            height = 0;
        }
    };

    namespace Bmp {

        bool decodeBmp(const char* path, ImageData& data);
        bool decodeBmp(const Stream& stream, ImageData& data);

        bool encodeBmp(const Stream& stream, const ImageData& data);
    }

    namespace WebP {
        bool decodeWebP(const FileStream& stream, ImageData& data);
    }


}