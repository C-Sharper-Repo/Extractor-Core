#pragma once
#include <ExCore/Rendering/Sprite.h>
#include <ExCore/Scene/Component.h>
#include <ExCore/Rendering/Renderable.h>
#include <memory>

namespace ExCore {
    class SpriteRenderer : public Component, public Renderable {
    public:
        SpriteRenderer(Object* object);

        void setSprite(std::shared_ptr<Sprite> sprite);
        std::shared_ptr<Sprite> getSprite();

        virtual bool canRender() const override;

        virtual uint32_t getVertexCount() const override;
        virtual const Vertex* getVertices() const override;

        virtual uint32_t getIndexCount() const override;
        virtual const uint32_t* getIndices() const override;

        virtual std::shared_ptr<Texture> getTexture() const override;
        virtual const Matrix4f& getWorldMatrix() const override;


    private:
        std::weak_ptr<Sprite> _sprite;
    };

    template<>
    static constexpr char* Component::getComponentName<SpriteRenderer>() {
        return "Sprite Renderer";
    }
}