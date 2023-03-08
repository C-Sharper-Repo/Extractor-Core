#include <ExCore/IO/Image.h>
#include <iostream>
#include <ExCore/Log.h>
#include <ExCore/Math/Color24.h>
#include <ExCore/Math/Color32.h>
#include <ExCore/Math/Math.h>
#include <ExCore/Util/DataUtilities.h>
#include <ExCore/IO/FileStream.h>

namespace ExCore {
    int32_t calcualtePadding(const int32_t width, const int32_t bpp) {
        const int32_t rem = (width * bpp) & 0x3;
        return rem ? 4 - rem : 0;
    }

    void flipRB(uint8_t* data, const int32_t width, const int32_t bpp) {
        const int32_t step = bpp >> 3;
        switch (bpp)
        {
            default: return;
            case 24:
            case 32:
                for (size_t i = 0, j = 0; i < width; i++, j += step) {
                    uint8_t temp = data[j];
                    data[j] = data[j + 2];
                    data[j + 2] = temp;
                }
                break;
        }
    }

    namespace WebP {
        enum TransformType : uint8_t {
            PREDICTOR_TRANSFORM      = 0,
            COLOR_TRANSFORM          = 1,
            SUBTRACT_GREEN_TRANSFORM = 2,
            COLOR_INDEXING_TRANSFORM = 3,
        };

        struct WebPSignature {
            uint32_t riff;
            uint32_t size;
            uint32_t webp;
        };

        struct WebPChunk {
            uint32_t type;
            uint32_t size;
        };

        //bool decodeWebP(const FileStream& stream, ImageData& data) {
        //    static constexpr uint32_t RIFF_HDR = 0x46464952U;
        //    static constexpr uint32_t WEBP_HDR = 0x50424557U;
        //    static constexpr uint32_t VPL8_HDR = 0x4C385056U;

        //    data = { 0, 0, TextureFormat::Unknown, nullptr };
        //    if (stream.size() < sizeof(WebPSignature)) { 
        //        std::cout << "[ExCore - WebP] Error: Stream too small!\n";
        //        return false;
        //    }

        //    WebPSignature sig{};
        //    stream.read(&sig, sizeof(WebPSignature));
        //    if (sig.riff != RIFF_HDR || sig.webp != WEBP_HDR) { 
        //        std::cout << "[ExCore - WebP] Error: Not webp data!\n";
        //        return false;
        //    }

        //    uint32_t intBuf{};
        //    WebPChunk chunk{};
        //    uint8_t version{ 0 };
        //    bool alphaUsed{ false };

        //    while (!stream.isEOF()) {
        //        stream.read(&chunk, sizeof(WebPChunk));

        //        size_t pos = stream.tell();
        //        switch (chunk.type)
        //        {
        //            default:
        //                break;
        //            case VPL8_HDR: {
        //                stream.seek(1, SEEK_CUR);
        //                stream.read(&intBuf, 4);

        //                data.width = (intBuf & 0x3FFF) + 1;
        //                intBuf >>= 14;

        //                data.height = (intBuf & 0x3FFF) + 1;
        //                intBuf >>= 14;

        //                alphaUsed = bool(intBuf & 0x1);
        //                version = (intBuf >> 1) & 0x7;

        //                if (version != 0) {
        //                    std::cout << "[ExCore - WebP] Error: Invalid VP8L version '" << int(version) << "'\n";
        //                    return false;
        //                }


        //                break;
        //            }
        //        }

        //        size_t toSeek = stream.tell() - pos;
        //        if (toSeek > 0) { stream.seek(toSeek, SEEK_CUR); }
        //    }
        //    return true;
        //}
    }

    namespace Bmp {

        bool decodeBmp(const char* path, ImageData& data) {
            FileStream stream(path, "rb");

            if (stream.isOpen()) {
                return decodeBmp(stream, data);
            }

            EX_CORE_ERROR("[Image-IO] (BMP) Error: Failed to open '{0}'!", path);
            return false;
        }

        bool decodeBmp(const Stream& stream, ImageData& data) {
#pragma pack(push, 1)
            struct BmpHeader {
                //File Header
                uint16_t signature;
                uint32_t length;
                char reserved[4];
                uint32_t dataOffset;

                //Info Header
                uint32_t headerSize;

                int32_t width;
                int32_t height;

                uint16_t planes;
                uint16_t bpp;

                uint32_t compression;
                uint32_t imageSize;

                uint32_t pPMX;
                uint32_t pPMY;

                int32_t colorsUsed;
                int32_t numOfColors;
            };
#pragma pack(pop, 1)

            data = {};

            size_t start = stream.tell();
            size_t dataSize = stream.size();
     
            if (dataSize < 54) {
                EX_CORE_ERROR("[Image-IO] (BMP) Error: Not enough data to read header!");
                return false;
            }

            BmpHeader header;
            stream.read(&header, sizeof(header));

            if (header.signature != 0x4D42U) { 
                EX_CORE_ERROR("[Image-IO] (BMP) Error: Invalid BMP signature!");
                return false;
            }
            if (header.length > dataSize) {
                EX_CORE_ERROR("[Image-IO] (BMP) Error: Not enough data in stream!");
                return false;
            }

            switch (header.bpp)
            {
                case 8:
                case 24:
                case 32:
                    break;
                default:
                    EX_CORE_ERROR("[Image-IO] (BMP) Error: Unsupported bitdepth ({0})!", header.bpp);
                    return false;
            }
            if (header.compression != 0 && header.compression != 0x3) { 
                EX_CORE_ERROR("[Image-IO] (BMP) Error: Unsupported compression mode ({0})!", header.compression);
                return false;
            }

            const bool flipX = header.width < 0;
            const bool flipY = header.height > 0;
            const uint32_t bytesPerPixel = header.bpp >> 3;

            if (header.width < 0) { header.width = -header.width; }
            if (header.height < 0) { header.height = -header.height; }

            const uint32_t reso = header.width * header.height;
       
            const int32_t padding = calcualtePadding(header.width, bytesPerPixel);

            const uint32_t rawScanSize = (header.width * bytesPerPixel);
            const uint32_t scanSize = rawScanSize + padding;
            char* scan = reinterpret_cast<char*>(alloca(scanSize));

            if (!scan) {
                EX_CORE_ERROR("[Image-IO] (BMP) Error: Failed to allocate scanline of size {0} bytes!", scanSize);
                return false;
            }

            const uint32_t paletteOff = header.bpp == 8 ? 256LL * 4 : 0;

            const uint32_t pixelDataSize = rawScanSize * header.height;
            const uint32_t outSize = pixelDataSize + paletteOff;

            data.pixels = reinterpret_cast<uint8_t*>(malloc(outSize));

            if (!data.pixels) {
                EX_CORE_ERROR("[Image-IO] (BMP) Error: Failed to allocate pixel buffer of size {0} bytes!", scanSize);
                _freea(scan);
                return false;
            }

            memset(data.pixels, 0, outSize);
            data.width = header.width;
            data.height = header.height;
        
            //Take note of position before any potential extra data used for 
            //bit depts other than 24 bits.
            size_t pos = stream.tell();

            const size_t end = size_t(pixelDataSize - rawScanSize) + paletteOff;
            //Seek to start of data and read data to buffer
            stream.seek(start + std::streamoff(header.dataOffset), std::ios::beg);
            for (size_t i = 0, sP = flipY ? 0 : paletteOff; i < header.height; i++, sP += rawScanSize) {
                stream.read(scan, scanSize);
                memcpy(data.pixels + (flipY ? (end - sP) : sP), scan, rawScanSize);
            }

            stream.seek(pos, std::ios::beg);
            switch (header.bpp)
            {
                case 8: {
                    //Read palette, swap Red & Blue channels, and then set alpha to 255
                    stream.read(data.pixels, header.numOfColors * 4);
                    Color32* cPtr = reinterpret_cast<Color32*>(data.pixels);

                    for (size_t i = 0; i < header.numOfColors; i++) {
                        auto& color = cPtr[i];
                        std::swap(color.b, color.r);
                        color.a = 0xFF;
                    }
                    data.format = TextureFormat::Indexed;
                    break;
                }
                case 24: {
                    //Swap Red & Blue channels
                    Color24* cPtr = reinterpret_cast<Color24*>(data.pixels);
                    for (size_t i = 0; i < reso; i++) {
                        auto& color = cPtr[i];
                        std::swap(color.b, color.r);
                    }
                    data.format = TextureFormat::RGB24;
                    break;
                }
                case 32: {
                    //Read color bit masks and calculate the bit offsets for them
                    uint32_t maskBuffer[4]{ 0 };
                    stream.read(reinterpret_cast<char*>(maskBuffer), 16);

                    const int32_t maskOffsets[4]{
                        Math::findFirstLSB(maskBuffer[0]),
                        Math::findFirstLSB(maskBuffer[1]),
                        Math::findFirstLSB(maskBuffer[2]),
                        Math::findFirstLSB(maskBuffer[3]),
                    };

                    //Apply color masks
                    uint32_t* iPtr = reinterpret_cast<uint32_t*>(data.pixels);
                    Color32* cPtr = reinterpret_cast<Color32*>(data.pixels);
                    for (size_t i = 0, j = 0; i < reso; i++, j++) {
                        auto& color = cPtr[i];
                        const uint32_t data = uint32_t(color);

                        color.r = (data & maskBuffer[0]) >> maskOffsets[0];
                        color.g = (data & maskBuffer[1]) >> maskOffsets[1];
                        color.b = (data & maskBuffer[2]) >> maskOffsets[2];
                        color.a = (data & maskBuffer[3]) >> maskOffsets[3];
                    }
                    data.format = TextureFormat::RGBA32;
                    break;
                }
            }
            return true;
        }

        bool encodeBmp(const Stream& stream, const ImageData& data) {
            static constexpr const char* BMP_SIG = "BM";
            static constexpr uint32_t HEADER_SIZE = 54;

            if (data.format == TextureFormat::RGBA32) {
                EX_CORE_ERROR("[Image-IO] (BMP) Error: Alpha isn't supported (yet)!");
                return false;
            }

            if (data.format == TextureFormat::Unknown) {
                EX_CORE_ERROR("[Image-IO] (BMP) Error: Unknown texture format!");
                return false;
            }

            uint32_t dataStart = HEADER_SIZE;

            int32_t padding = calcualtePadding(data.width, 3);
            int32_t rawScanSize = data.width * 3;
            int32_t scanSize = rawScanSize + padding;

            uint8_t* scan = reinterpret_cast<uint8_t*>(_malloca(scanSize));
            if (!scan) { 
                EX_CORE_ERROR("[Image-IO] (BMP) Error: Failed to allocate scanline buffer of size {0} bytes!", scanSize);
                return false; 
            }
            memset(scan, 0, scanSize);

            //File Header
            stream.write(BMP_SIG, 2);
            stream.writeValue<uint32_t>(dataStart + scanSize * data.height, 1);
            stream.writeZero(4);
            stream.writeValue(dataStart);

            //Info Header
            stream.writeValue(40);

            stream.write(&data.width, 4);
            stream.write(&data.height, 4);
            stream.writeValue<uint16_t>(1);

            stream.writeValue<uint16_t>(24);

            stream.writeValue<int32_t>(0x0, 1);
            stream.writeValue<int32_t>(scanSize * data.height, 1);
            stream.writeValue<uint32_t>(2835, 2);

            stream.writeZero(8);
            for (size_t y = 0, yP = size_t(data.height) * rawScanSize - size_t(rawScanSize); y < data.height; y++, yP -= rawScanSize) {
                memcpy(scan, data.pixels + yP, rawScanSize);
                flipRB(scan, data.width, 3);
                stream.write(scan, scanSize);
            }
            return true;
        }
    }
}

