#include <ExampleApp.h>
#include <imgui.h>
#include <ExCore/Util/HeapVector.h>
#include <iostream>
#include <ExCore/Scene/Components/SpriteRenderer.h>
#include <ExCore/Scene/Components/Camera.h>
#include <ExCore/IO/Image.h>
#include <ExCore/Gui/IGuiExtras.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <ExCore/IO/IOUtils.h>

#include <io.h>
#define F_OK 0
#define access _access

using namespace ExCore;
ExampleApp::ExampleApp(const AppSpecs& specs) : Application(specs) {}

char tempFile[513]{ 0 };
char tempFldr[513]{ 0 };

void ExampleApp::start() {
    auto obj = _scene.spawnObject("Fembu").lock();
    auto obj2 = _scene.spawnObject("Main Camera").lock();
    auto sprt = obj->addComponent<SpriteRenderer>().lock();
    auto cam = obj2->addComponent<Camera>().lock();

    cam->setClearColor(Color32(24, 24, 32, 255));
    obj2->getTransform().setScale({ 10, 10 }, Space::Local);

    _scene.setCamera(cam);

    ImageData data;
    static constexpr char testPath[] = "res/Fembu_Body.bmp";
    if (Bmp::decodeBmp(testPath, data)) {
        tex = std::make_shared<Texture>();
        sprite = std::make_shared<Sprite>(SpriteFrame(0, 0, data.width, data.height), tex);
        sprt->setSprite(sprite);
        tex.get()->create(data.pixels, data.format, data.width, data.height);
        EX_CORE_INFO("Created new texture and sprite! '{0}'", testPath);
    }
    data.clear(true);
}

Space space{ Space::Local };
std::weak_ptr<Object> fembu{};
bool opened{ false };
std::string path{};

void ExampleApp::doGui() {
    static int32_t CurMenu = 0;
    static bool settingsOpen = false;

    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) {
        window_flags |= ImGuiWindowFlags_NoBackground;
    }

    float headerY = ImGui::GetFrameHeight() * 2.2f;

    ImVec2 pos;
    ImVec2 size;

    //Header
    {
        pos = viewport->Pos;

        size = viewport->Size;
        size.x += 5;
        size.y = headerY;


        ImGui::SetNextWindowPos(pos);
        ImGui::SetNextWindowSize(size);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGui::Begin("##Header", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);
        //Header Menu Bar
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Open", "", false, true)) {
                    path = IO::openFile("Project JSON (*.json)\0*.json");
                    opened = access(path.c_str(), F_OK) == 0;

                    if (opened) {
                        EX_CORE_INFO("Opened file: '{0}'", path.c_str());
                    }
                    else if (path.length()) {
                        EX_CORE_ERROR("Couldn't open file: '{0}'", path.c_str());
                    }
                }
                if (ImGui::MenuItem("Close", "", false, opened)) { opened = false; }
                if (ImGui::MenuItem("Quit", "", false, true)) {
                    opened = false;
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Tools")) {
                if (ImGui::MenuItem("Settings", "", false, !settingsOpen)) {
                    settingsOpen = true;
                    /*auto fPath = IO::openFile("Fonts (*.ttf)\0*.ttf");

                    if (access(fPath.c_str(), F_OK) == 0) {
                        EX_CORE_INFO("Opened file: '{0}'", fPath.c_str());
                    }*/
                }
                ImGui::EndMenu();
            }
        }
        ImGui::EndMenuBar();

        ImGui::SetCursorPos({ 5, ImGui::GetFrameHeight() });
        if (ImGui::BeginChild("##MainView", { 0, ImGui::GetFrameHeight() })) {
            if (ImGui::BeginTabBar("##Main-Tab")) {
                if (ImGui::BeginTabItem("Setup##Main-Tab")) {
                    CurMenu = 0;
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Game##Main-Tab")) {
                    CurMenu = 1;
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
        }
        ImGui::EndChild();
        ImGui::End();
    }

    pos = viewport->Pos;
    pos.y += headerY;

    size = viewport->Size;
    size.y -= headerY;

    ImGui::SetNextWindowPos(pos);
    ImGui::SetNextWindowSize(size);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("##Main", nullptr, window_flags & ~ImGuiWindowFlags_MenuBar);
    ImGui::PopStyleVar();
    ImGui::PopStyleVar(2);

    //Setup dockspaces
    {
        ImGuiID dockspace_id = ImGui::GetID("Left");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        static bool first_time = true;
        if (first_time) {
            first_time = false;
            auto size = viewport->Size;

            size.y -= ImGui::GetCursorPosY();

            ImGui::DockBuilderRemoveNode(dockspace_id);
            ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
            ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

            auto dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.2f, nullptr, &dockspace_id);
            auto dock_id_right = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.2f, nullptr, &dockspace_id);
            auto dock_id_down = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.25f, nullptr, &dockspace_id);

            ImGui::DockBuilderDockWindow("Left", dock_id_left);
            ImGui::DockBuilderDockWindow("Right", dock_id_right);
            ImGui::DockBuilderDockWindow("Down", dock_id_down);
            ImGui::DockBuilderDockWindow("Preview", dockspace_id);

            ImGui::DockBuilderFinish(dockspace_id);
            ImGui::LoadIniSettingsFromDisk("imgui.ini");
        }
    }

    ImGui::End();

    static ImGuiWindowFlags_ viewFlags = ImGuiWindowFlags_(ImGuiWindowFlags_NoDecoration & ~(ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize));
    switch (CurMenu)
    {
        default:
            break;

        case 1:
            ImGui::Begin("Left", 0, viewFlags);
            ImGui::End();

            ImGui::Begin("Right", 0, viewFlags);
            ImGui::End();

            ImGui::Begin("Down", 0, viewFlags);
            ImGui::End();

            ImGui::Begin("Preview", 0, viewFlags);

            auto cam = _scene.getCamera().lock();
            if (cam) {
                const FrameBuffer& fb = cam->getFrameBuffer() ? *cam->getFrameBuffer() : Renderer::getInstance()->getScreenBuffer();
                ImGui::Image((void*)fb.getColorAttatchment(), ImGui::GetContentRegionAvail());
            }
            else {
                static constexpr char* NO_CAM = "No active camera!";
                auto avail = ImGui::GetContentRegionAvail();
                auto textS = ImGui::CalcTextSize(NO_CAM);
                ImGui::SetCursorPos({
                     avail.x * 0.5f - textS.x * 0.5f,
                     avail.y * 0.5f - textS.y * 0.5f
                    });
                ImGui::Text(NO_CAM);
            }

            ImGui::End();
            break;
    }

    if (settingsOpen) {
        ImGui::SetNextWindowSize({ 300, 900 }, ImGuiCond_FirstUseEver);
        if (ImGui::Begin("Settings##ExCore", &settingsOpen, ImGuiWindowFlags_NoDocking)) {
            static char temp[513]{ 0 };
            if (Gui::searchDialogLeft("Font path here...", temp, "Font (*.ttf)\0*.ttf")) {
                EX_CORE_TRACE("Set current font to '{0}'", temp);
            }
        }
        ImGui::End();
    }
}
