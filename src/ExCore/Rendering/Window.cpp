#include <ExCore/Rendering/Window.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <ext.hpp>
#include <ExCore/Log.h>

namespace ExCore {
    Window* Window::Instance{ nullptr };

    Window::Window() : _window(nullptr), _size() , _minSize() { Instance = this; }
    Window::~Window() { Instance = Instance == this ? nullptr : Instance; close(); }

    Window* Window::getInstance() {
        return Instance;
    }

    bool Window::initialize(const char* title, uint32_t width, uint32_t height) {
        if (_window) { return true; }

        _minSize.x = width >> 2;
        _minSize.y = height >> 2;

        //Init GLFW window
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

        _window = glfwCreateWindow(width, height, title, NULL, NULL);
        if (!_window) {
            EX_CORE_ERROR("Error: Failed to initialize GLFW window!");
            return false;
        }

        glfwMakeContextCurrent(_window);

        //Init Glew
        if (glewInit() != GLEW_OK) {
            EX_CORE_ERROR("Error: Failed to initialize GLEW!");
            glfwTerminate();
            return false;
        }

        glfwSetWindowSizeLimits(_window, _minSize.x, _minSize.y, GLFW_DONT_CARE, GLFW_DONT_CARE);

        //Init window resize event update set screen size related values
        onWindowResize(width, height);

        glfwSetWindowSizeCallback(_window, windowResizeCallback);
        return true;
    }

    void Window::close() {
        if (_window == NULL) { return; }

        _screenBuffer.releaseBuffer();
        _screenBuffer.releaseColorAttatchment();

        glfwSetWindowSizeCallback(_window, NULL);
        glfwDestroyWindow(_window);
        _window = NULL;
    }

    void Window::resetViewport(const uint8_t flags) {
        if (flags & FLAG_VIEW) {
            glViewport(0, 0, _size.x, _size.y);
        }

        if (flags & FLAG_SCISSOR) {
            glScissor(0, 0, _size.x, _size.y);
        }
    }

    void Window::updateViewport(const glm::i32vec2& viewRect, const glm::i32vec2& viewSize, const uint8_t flags) {
        if (flags & FLAG_VIEW) {
            glViewport(viewRect.x, viewRect.y, viewSize.x, viewSize.y);
        }

        if (flags & FLAG_SCISSOR) {
            glScissor(viewRect.x, viewRect.y, viewSize.x, viewSize.y);
        }
    }

    bool Window::update() {
        return _window && !glfwWindowShouldClose(_window);
    }

    bool Window::finalizeFrame() {
        if (!_window) { return false; }
        glfwPollEvents();
        glfwSwapBuffers(_window);
        return true;
    }

    void Window::onWindowResize(const int32_t width, const int32_t height) {
        _size.x = width;
        _size.y = height;

        _isMinimized = width <= 0 || height <= 0;
        const float aspect = _size.x / float(_size.y);
        const float w = 0.5f * aspect;

        _worldProjection = Matrix4f::ortho(-w, w, -0.5f, 0.5f);

        _screenProjection = _worldProjection;
        _screenProjection.translate(-w, -0.5f);
        _screenProjection.scale(aspect, 1.0f);

        FrameBufferSpecs specs{};
        specs.width = width;
        specs.height = height;

        _screenBuffer.invalidate(specs);
    }

    void Window::windowResizeCallback(GLFWwindow* window, const int32_t width, const int32_t height) {
        if (Instance == nullptr) { return; }
        if (window == Instance->_window) {
            Instance->onWindowResize(width, height);
        }
    }
}