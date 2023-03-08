#pragma once
#include <ExCore/Scene/Component.h>
#include <ExCore/Rendering/ICamera.h>

namespace ExCore {
    class Camera : public Component, public ICamera {
    public:
        Camera(Object* obj);

        virtual const Matrix4f& getViewMatrix(Matrix4f& view) const override;
        virtual const Matrix4f& getWorldMatrix() const override;

        const FrameBuffer* getFrameBuffer() const { return &_fBuffer; };

    private:
        FrameBuffer _fBuffer;
    };

    template<>
    static constexpr char* Component::getComponentName<Camera>() {
        return "Camera";
    }
}