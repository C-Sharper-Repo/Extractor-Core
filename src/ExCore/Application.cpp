#include <ExCore/Application.h>
#include <ExCore/IO/IOUtils.h>
#include <Windows.h>
#include <io.h>
#define F_OK 0
#define access _access

namespace ExCore {
    Application::Application(const AppSpecs specs) : _renderer(), _font(nullptr), _scene{} {}

    Application::~Application() {
        close();
    }

    bool Application::initialize() {
        if (_renderer.initialize(_specs.name.c_str())) {
            _scene.clear(); 
            initFont("res/fonts/MPLUS1-Regular.ttf");
            setupStyles();

            start();
            return true;
        }

        EX_CORE_ERROR("Failed to initialize renderer!");
        return false;
    }

    void Application::run() {
        while (_renderer.update()) {
            _scene.update();
            _renderer.render(_scene.getCamera(), this);
        }
    }    
    
    void Application::close() {
        stop();
        _scene.clear();
        _renderer.release();
    }

    void Application::initFont(const wchar_t* charset, const char* fontpath) {
        auto& io = ImGui::GetIO();
        io.Fonts->Clear();
        auto defFont = io.Fonts->AddFontDefault();

        EX_CORE_INFO("Initializing fonts");

        _font = nullptr;
        if (access(fontpath, F_OK) == 0) {
           _font = io.Fonts->AddFontFromFileTTF(fontpath, 16, 0, reinterpret_cast<const ImWchar*>(charset));
           EX_CORE_INFO("Added font: '{0}'", fontpath);
        }
        else if (fontpath) {
            EX_CORE_WARN("Couldn't read font '{0}'", fontpath);
        }
        io.Fonts->Build();
    }

    void Application::initFont(const char* fontpath) {
        static wchar_t DefaultCharset[]{
           //Same as Full Chinese chars in ImGui
           //Could possibly use a more specialized range
           0x0020, 0x00FF, // Basic Latin + Latin Supplement
           0x2000, 0x206F, // General Punctuation
           0x3000, 0x30FF, // CJK Symbols and Punctuations, Hiragana, Katakana
           0x31F0, 0x31FF, // Katakana Phonetic Extensions
           0xFF00, 0xFFEF, // Half-width characters
           0xFFFD, 0xFFFD, // Invalid
           0x4e00, 0x9FAF, // CJK Ideograms
           0,
        };
        initFont(DefaultCharset, fontpath);
    }

    void Application::toggleFont(const bool state) {
        if (!_font) { return; }
        if (state) {
            ImGui::PushFont(_font);
        }
        else { ImGui::PopFont(); }
    }

    void Application::setupStyles()  {
        EX_CORE_INFO("Setting up GUI styles");

        ImGuiStyle defStyle(ImGui::GetStyle());
        defStyle.Colors[ImGuiCol_Button] = { 0.155f, 0.155f, 0.155f, 1.0f };
        defStyle.Colors[ImGuiCol_WindowBg] = { 0.105f, 0.105f, 0.105f, 1.0f };
        defStyle.Colors[ImGuiCol_ChildBg] = { 0.125f, 0.125f, 0.125f, 1.0f };
        defStyle.Colors[ImGuiCol_Border] = { 0.065f, 0.065f, 0.065f, 1.0f };
        defStyle.Colors[ImGuiCol_FrameBg] = { 0.075f, 0.075f, 0.075f, 1.0f };
        defStyle.Colors[ImGuiCol_FrameBgHovered] = { 0.200f, 0.200f, 0.200f, 1.0f };
        defStyle.Colors[ImGuiCol_FrameBgActive] = { 0.180f, 0.180f, 0.180f, 1.0f };
        defStyle.Colors[ImGuiCol_CheckMark] = { 0.480f, 0.480f, 0.580f, 1.0f };
        defStyle.ChildBorderSize = 4.0f;
        _styles.push_back(defStyle);
        setupStyles(defStyle);
        setStyle(0);
    }

    void Application::setupStyles(ImGuiStyle& defStyle) {}

    void Application::setStyle(const int32_t index) {
        ImGui::GetStyle() = _styles[index];
    }
}
