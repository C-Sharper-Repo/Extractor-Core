#pragma once
#include <cstdint>

namespace ExCore {
    struct FrameBufferSpecs {
        uint16_t width;
        uint16_t height;

        bool operator==(const FrameBufferSpecs& other) const {
            return width == other.width && height == other.height;
        }

        bool operator!=(const FrameBufferSpecs& other) const {
            return !(*this == other);
        }
    };

    class FrameBuffer {
    public:

        FrameBuffer();
        FrameBuffer(const uint32_t colorFormat);
        FrameBuffer(const FrameBufferSpecs& specs);
        FrameBuffer(const uint32_t colorFormat, const FrameBufferSpecs& specs);
        ~FrameBuffer();

        const uint32_t& getBufferId() const { return _bufferId; }
        const uint32_t getColorAttatchment() const { return _colorAttatchment; }

        const FrameBufferSpecs& getSpecs() const { return _specs; }

        void invalidate(const FrameBufferSpecs& specs);
        void releaseBuffer();

        void releaseColorAttatchment();
    
        uint32_t bindColorAttachment(const uint32_t slot) const;
        uint32_t unbindColorAttachment(const uint32_t slot) const;

        bool bind() const;
        void unbind() const;

    private:
        FrameBufferSpecs _specs;
        uint32_t _bufferId;

        uint32_t _colorFormat;
        uint32_t _colorAttatchment;

        void invalidate(const bool rebuild);
    };
}