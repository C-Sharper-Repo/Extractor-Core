#pragma once
#include <cstdint>
#include <ExCore/Math/Vertex.h>
#include <memory>

namespace ExCore {
    struct SpriteFrame {
        uint16_t x{0}, y{0};
        uint16_t width{0}, height{0};

        SpriteFrame() : 
            x(0), y(0), width(0), height(0) {}
        SpriteFrame(uint16_t x, uint16_t y, uint16_t width, uint16_t height) : 
            x(x), y(y), width(width), height(height) {}
    };

    class Texture;
    class Sprite {
    public:
        SpriteFrame rect;
        std::weak_ptr<Texture> texture;

        Sprite();
        Sprite(const SpriteFrame rect, const std::shared_ptr<Texture>& texture);

        const Vertex* getVertices() const { return _verts; }

    private:
        Vertex _verts[4];
    };
}