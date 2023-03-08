#pragma once
#include <ExCore/Rendering/Buffers/FrameBuffer.h>
#include <glm.hpp>
#include <ExCore/Math/Matrix4f.h>

struct GLFWwindow;
namespace ExCore {
    class Window {
    public:
        static constexpr uint8_t FLAG_VIEW = 0x01;
        static constexpr uint8_t FLAG_SCISSOR = 0x02;

        Window();
        ~Window();

        static Window* getInstance();

        const Matrix4f& getWorldProjection() const { return _worldProjection; }
        const Matrix4f& getScreenProjection() const { return _screenProjection; }

        const FrameBuffer& getScreenBuffer() const { return _screenBuffer; }
        bool isMinimized() const { return _isMinimized; }

        int32_t getWidth()  const { return _size.x; }
        int32_t getHeight() const { return _size.y; }

        bool initialize(const char* title, uint32_t width = 1280, uint32_t height = 720);
        void close();

        void resetViewport(const uint8_t flags = FLAG_VIEW | FLAG_SCISSOR);
        void updateViewport(const glm::i32vec2& viewRect, const glm::i32vec2& viewSize, const uint8_t flags);

        bool update();
        bool finalizeFrame();

        GLFWwindow* getNativeWindow() { return _window; }
        const GLFWwindow* getNativeWindow() const { return _window; }

    private:
        static Window* Instance;

        GLFWwindow* _window;
        FrameBuffer _screenBuffer;

        Matrix4f _worldProjection;
        Matrix4f _screenProjection;
        glm::i32vec2 _size;
        glm::i32vec2 _minSize;
        bool _isMinimized;

        void onWindowResize(const int32_t width, const int32_t height);
        static void windowResizeCallback(GLFWwindow* window, const int32_t width, const int32_t height);
    };
}