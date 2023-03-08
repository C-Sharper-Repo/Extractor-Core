#define IMGUI_DEFINE_MATH_OPERATORS
#include <ExCore/Gui/IGuiExtras.h>
#include <ExCore/IO/IOUtils.h>
#include <ExCore/Scene/Object.h>
#include <imgui_internal.h>

namespace ExCore::Gui {

    bool searchDialogCenter(const char* label, char path[513], const char* types) {
        ImGui::BeginGroup();
        ImGui::PushID(label);
        float sqSize = ImGui::GetFrameHeight();
        auto size = ImGui::GetWindowSize();
        float inputW = std::max(size.x - sqSize - 5, 120.0f);
        float totW = inputW + sqSize;

        ImGui::SetCursorPos({ (size.x * 0.5f) - totW * 0.5f , (size.y * 0.5f) - sqSize * 0.5f });
        bool press = ImGui::RadioButton("##Search", true);
        ImGui::SameLine();

        ImGui::PushItemWidth(inputW);
        bool changed = ImGui::InputTextWithHint("##Path", label, path, 512);
        ImGui::PopItemWidth();

        if (press) {
            //Check if we're a folder
            if (types == nullptr) {
                auto str = IO::openFolder("Search folder");
                if (str.length() > 0) {
                    memset(path, 0, 513);
                    memcpy(path, str.c_str(), std::min<size_t>(512, str.length()));
                    changed = true;
                }
            }
            else {
                auto str = IO::openFile(types);
                if (str.length() > 0) {
                    memset(path, 0, 513);
                    memcpy(path, str.c_str(), std::min<size_t>(512, str.length()));
                    changed = true;
                }
            }
        }
        ImGui::PopID();
        ImGui::EndGroup();
        return changed;

        return false;
    }

    bool searchDialogLeft(const char* label, char path[513], const char* types) {
        ImGui::BeginGroup();
        ImGui::PushID(label);
        float sqSize = ImGui::GetFrameHeight();
        float width = ImGui::GetWindowSize().x;
        bool press = ImGui::RadioButton("##Search", true);
        ImGui::SameLine();

        ImGui::PushItemWidth(std::max(width - sqSize - 5, 120.0f));
        bool changed = ImGui::InputTextWithHint("##Path", label, path, 512);
        ImGui::PopItemWidth();

        if (press) {
            //Check if we're a folder
            if (types == nullptr) {
                auto str = IO::openFolder("Search folder");
                if (str.length() > 0) {
                    memset(path, 0, 513);
                    memcpy(path, str.c_str(), std::min<size_t>(512, str.length()));
                    changed = true;
                }
            }
            else {
                auto str = IO::openFile(types);
                if (str.length() > 0) {
                    memset(path, 0, 513);
                    memcpy(path, str.c_str(), std::min<size_t>(512, str.length()));
                    changed = true;
                }
            }
        }
        ImGui::PopID();
        ImGui::EndGroup();
        return changed;
    }

    bool drawTransform(const char* label, Transform& tr, Space* space) {
        ImGui::PushID(label);

        bool changed = false;
        bool chGen = false;

        if (ImGui::CollapsingHeader(label)) {
            auto parent = tr.getParent();
            ImGui::LabelText("Parent", "%s", parent ? parent->getObject()->getName().c_str() : "No Parent");
  
            chGen = space && drawEnumList("Space", *space);
            Space spc = space ? *space : Space::World;

            glm::vec2 pos = tr.getPosition(spc);
            float rot = tr.getRotation(spc);
            glm::vec2 sca = tr.getScale(spc);

            changed |= ImGui::DragFloat2("Position", &pos.x, 0.001f, 0, 0, "%.4f", ImGuiSliderFlags_NoRoundToFormat);
            changed |= ImGui::DragFloat("Rotation", &rot, 0.001f, 0, 0, "%.4f", ImGuiSliderFlags_NoRoundToFormat);
            changed |= Gui::drawGui("Scale", sca);

            if (changed) {
                tr.setTRS(pos, rot, sca, spc);
            }
        }

        ImGui::PopID();
        return chGen || changed;
    }

    bool drawSplitter(const bool splitVertical, const float thickness, float* size0, float* size1, float minSize0, float minSize1, float splitterAxisSize) {
        return drawSplitter("##Splitter", thickness, size0, size1, minSize0, minSize1, splitterAxisSize);
    }

    bool drawSplitter(const char* idIn, const bool splitVertical, const float thickness, float* size0, float* size1, float minSize0, float minSize1, float splitterAxisSize) {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = g.CurrentWindow;
        ImGuiID id = window->GetID(idIn);
        ImRect bb;
        bb.Min = window->DC.CursorPos + (splitVertical ? ImVec2(*size0, 0.0f) : ImVec2(0.0f, *size0));
        bb.Max = bb.Min + ImGui::CalcItemSize(splitVertical ? ImVec2(thickness, splitterAxisSize) : ImVec2(splitterAxisSize, thickness), 0.0f, 0.0f);
        return ImGui::SplitterBehavior(bb, id,  splitVertical ? ImGuiAxis_X : ImGuiAxis_Y, size0, size1, minSize0, minSize1, 0.0f);

    }
}