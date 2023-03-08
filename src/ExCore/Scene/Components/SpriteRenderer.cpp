#include <ExCore/Scene/Components/SpriteRenderer.h>
#include <ExCore/Scene/Object.h>
#include <ExCore/Rendering/Renderer.h>

namespace ExCore {
    SpriteRenderer::SpriteRenderer(Object* object) : Component(object) { }

    void SpriteRenderer::setSprite(std::shared_ptr<Sprite> sprite) {
        _sprite = sprite;
    }

    std::shared_ptr<Sprite> SpriteRenderer::getSprite()  {
        return _sprite.lock();
    }

    bool SpriteRenderer::canRender() const {
        return !_sprite.expired();
    }

    uint32_t SpriteRenderer::getVertexCount() const {
        return _sprite.expired() ? 0 : 4;
    }

    const Vertex* SpriteRenderer::getVertices() const {
        return _sprite.expired() ? nullptr : _sprite.lock().get()->getVertices();
    }

    uint32_t SpriteRenderer::getIndexCount() const {
        return 6;
    }

    const uint32_t* SpriteRenderer::getIndices() const {
        return Renderer::QUAD_INDICES;
    }

    std::shared_ptr<Texture> SpriteRenderer::getTexture() const {
        return _sprite.expired() ? std::shared_ptr<Texture>() : _sprite.lock().get()->texture.lock();
    }

    const Matrix4f& SpriteRenderer::getWorldMatrix() const {
        return _owner ? _owner->getTransform().getWorldMatrix() : Matrix4f::Identity;
    }

}