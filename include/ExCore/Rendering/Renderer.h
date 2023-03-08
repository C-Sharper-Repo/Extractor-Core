#pragma once
#include <ExCore/Rendering/Window.h>
#include <ExCore/Math/Vertex.h>
#include <ExCore/Math/Color32.h>
#include <ExCore/Rendering/Shader.h>
#include <ExCore/Rendering/Buffers/FrameBuffer.h>
#include <ExCore/Rendering/DynamicBatch.h>
#include <set>

namespace ExCore {
    class IGui {
    public:
        virtual void doGui() = 0;
    };

    class Sprite;
    class Renderable;
    class ICamera;
    class Renderer {
    public:
        static const uint32_t QUAD_INDICES[6];
        static const Vertex QUAD[4];

        Renderer();
        ~Renderer();

        bool initialize(const char* title, uint32_t width = 1280, uint32_t height = 720);
        void release();

        bool update();
        void render(const std::weak_ptr<ICamera>& camera, IGui* guiDraw);

        Window& getWindow() { return _window; }
        const Window& getWindow() const { return _window; }

        const FrameBuffer& getScreenBuffer() const { return _window.getScreenBuffer(); }
        static Renderer* getInstance() { return Instance; }

        static void addRenderable(Renderable* renderable);
        static void removeRenderable(Renderable* renderable);

    private:
        static Renderer* Instance;

        std::set<Renderable*> _renderables;
        std::vector<Renderable*> _activeRenderers;

        DynamicBatch _dynamicBatch;

        Window _window;
        bool _initialized;

        Shader _shaders[2];

        void clear(const Color32& color);
    };
}