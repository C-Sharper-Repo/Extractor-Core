#include <ExCore/Rendering/Sprite.h>
#include <ExCore/Rendering/Texture.h>

namespace ExCore {
    static glm::vec2 pixToUVCoord(const glm::i32vec2& coord, const glm::i32vec2& size) {
        return { size.x ? coord.x / float(size.x) : 0, size.y ? coord.y / float(size.y) : 0 };
    }

    Sprite::Sprite() : texture(), rect() { }
    Sprite::Sprite(const SpriteFrame rect, const std::shared_ptr<Texture>& tex)
        : texture(tex), rect(rect) {
        auto texPtr = texture.lock().get();
        glm::i32vec2 reso = texPtr ? glm::i32vec2(texPtr->getWidth(), texPtr->getHeight()) : glm::i32vec2(0, 0);
        _verts[0] = { {rect.width * -0.5f, rect.height * -0.5f}, {0xFF, 0xFF, 0xFF, 0xFF}, pixToUVCoord({rect.x, rect.y}, reso) };
        _verts[1] = { {rect.width *  0.5f, rect.height * -0.5f}, {0xFF, 0xFF, 0xFF, 0xFF}, pixToUVCoord({rect.x + rect.width, rect.y}, reso) };
        _verts[2] = { {rect.width *  0.5f, rect.height *  0.5f}, {0xFF, 0xFF, 0xFF, 0xFF}, pixToUVCoord({rect.x + rect.width, rect.y + rect.height}, reso) };
        _verts[3] = { {rect.width * -0.5f, rect.height *  0.5f}, {0xFF, 0xFF, 0xFF, 0xFF}, pixToUVCoord({rect.x,              rect.y + rect.height}, reso) };
    }
}