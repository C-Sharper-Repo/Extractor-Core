#include <ExCore/Scene/Components/Camera.h>
#include <ExCore/Scene/Object.h>

namespace ExCore {
    Camera::Camera(Object* obj) : Component(obj), ICamera(), _fBuffer() { 
        FrameBufferSpecs specs{};
        specs.width = 1920;
        specs.height = 1080;
        _fBuffer.invalidate(specs);
    }

    const Matrix4f& Camera::getViewMatrix(Matrix4f& view) const {
        view *= getWorldMatrix();
        return view;
    }
    const Matrix4f& Camera::getWorldMatrix() const {
        return _owner ? _owner->getTransform().getWorldMatrix() : Matrix4f::Identity;
    }
}