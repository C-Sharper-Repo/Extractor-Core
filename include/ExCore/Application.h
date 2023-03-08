#pragma once
#include <cstdint>
#include <string>
#include <ExCore/Rendering/Renderer.h>
#include <ExCore/Scene/Scene.h>
#include <ExCore/Scene/Transform.h>
#include <imgui.h>
#include <ExCore/Log.h>
#include <ExCore/Util/EnumUtils.h>

namespace ExCore {
    struct AppArgs {
        int32_t count = 0;
        char** args = nullptr;

        const char* operator[](int32_t index) const { return args[index]; }
    };

    struct AppSpecs {
        std::string name = "Extractor-Gui Project";
        AppArgs args;
    };

    class Application : public IGui {
    public:
        Application(const AppSpecs specs);
        virtual ~Application();

        bool initialize();

        void run();
        void close();

        void initFont(const wchar_t* charset, const char* fontpath);
        void initFont(const char* fontpath);

        void toggleFont(const bool state);
       
    protected:
        ImFont* _font;

        AppSpecs _specs;
        Renderer _renderer;
        Scene _scene;

        std::vector<ImGuiStyle> _styles;

        virtual void start() {}
        virtual void stop(){}

        virtual void doGui() = 0;
        virtual void setupStyles(ImGuiStyle& defStyle);

        void setStyle(const int32_t index);
    private:
        void setupStyles();
    };
    Application* createApplication(AppArgs args);
}