#include <ExCore/Rendering/Buffers/FrameBuffer.h>
#include <GL/glew.h>
#include <cassert>

namespace ExCore {
    FrameBuffer::FrameBuffer() : FrameBuffer(GL_RGBA8) { }
    FrameBuffer::FrameBuffer(const uint32_t colorFormat) : 
        _colorFormat(colorFormat), _specs(), _bufferId(0), _colorAttatchment(0) { }

    FrameBuffer::FrameBuffer(const FrameBufferSpecs& specs) : FrameBuffer(GL_RGBA8, specs) { }
    FrameBuffer::FrameBuffer(const uint32_t colorFormat, const FrameBufferSpecs& specs) : 
        _colorFormat(colorFormat), _specs(specs), _bufferId(0), _colorAttatchment(0) { }

    FrameBuffer::~FrameBuffer() {
        releaseBuffer();
        releaseColorAttatchment();
    }

    void FrameBuffer::invalidate(const FrameBufferSpecs& specs) {
        const bool rebuild = specs != _specs || !_bufferId;
        _specs = specs;
        invalidate(rebuild);
    }

    void FrameBuffer::invalidate(const bool rebuild) {
        if (!rebuild) { return; }

        const bool createdBuf = !_bufferId;
        const bool createdTex = !_colorAttatchment;

        if (createdBuf) {
            glCreateFramebuffers(1, &_bufferId);
        }

        if (createdTex || createdBuf) {
            glBindFramebuffer(GL_FRAMEBUFFER, _bufferId);
        }

        if (createdTex) {
            glCreateTextures(GL_TEXTURE_2D, 1, &_colorAttatchment);
        }

        glBindTexture(GL_TEXTURE_2D, _colorAttatchment);
        glTexImage2D(GL_TEXTURE_2D, 0, _colorFormat, _specs.width, _specs.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        if (createdTex || createdBuf) {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _colorAttatchment, 0);
        }

        if (createdTex ) {
            GLenum ret = glCheckFramebufferStatus(GL_FRAMEBUFFER);
            assert(ret == GL_FRAMEBUFFER_COMPLETE && "Framebuffer not complete!");
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::releaseBuffer() {
        if (_bufferId) {
            glDeleteFramebuffers(1, &_bufferId);
            _bufferId = 0;
        }
    }

    void FrameBuffer::releaseColorAttatchment() {
        if (_colorAttatchment) {
           glDeleteFramebuffers(1, &_colorAttatchment);
           _colorAttatchment = 0;
        }
    }

    uint32_t FrameBuffer::bindColorAttachment(const uint32_t slot) const {
        uint32_t bindP = slot;

        glActiveTexture(GL_TEXTURE0 + bindP++);
        glBindTexture(GL_TEXTURE_2D, _colorAttatchment);
        return slot;
    }

    uint32_t FrameBuffer::unbindColorAttachment(const uint32_t slot) const {
        uint32_t bindP = slot;
        glActiveTexture(GL_TEXTURE0 + bindP++);
        glBindTexture(GL_TEXTURE_2D, 0);
        return bindP;
    }

    bool FrameBuffer::bind() const {
        if (_bufferId) {
            glBindFramebuffer(GL_FRAMEBUFFER, _bufferId);
            return true;
        }
        return false;
    }

    void FrameBuffer::unbind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}