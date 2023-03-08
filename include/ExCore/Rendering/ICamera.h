#pragma once
#include <ExCore/Math/Color32.h>
#include <ExCore/Math/Rect.h>
#include <ExCore/Math/Matrix4f.h>
#include <ExCore/Rendering/Buffers/FrameBuffer.h>

namespace ExCore {
    class ICamera {
    public:
        ICamera() : _clearColor(Color32::Clear), _viewRect(0, 0, 1, 1), _screenRect(0, 0, 1, 1) {}

        virtual const Matrix4f& getViewMatrix(Matrix4f& view) const = 0;
        virtual const Matrix4f& getWorldMatrix() const = 0;

        const Rect<float>& getViewRect() const { return _viewRect; }
        Rect<float>& getViewRect() { return _viewRect; }

        const Rect<float>& getScreenRect() const { return _screenRect; }
        Rect<float>& getScreenRect() { return _screenRect; }

        Color32 setClearColor(const Color32 color) { return _clearColor = color; }
        Color32 getClearColor() const { return _clearColor; }

        virtual const FrameBuffer* getFrameBuffer() const = 0;

    protected:
        Color32 _clearColor;
        Rect<float> _viewRect;
        Rect<float> _screenRect;
    };
}