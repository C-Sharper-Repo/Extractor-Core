#pragma once
#include <ExCore/Gui/IGuiDrawable.h>
#include <ExCore/Scene/Transform.h>
#include <imgui.h>
#include <glm.hpp>


template<>
bool ExCore::IGuiDrawable<glm::vec2>::onGui(const char* label, glm::vec2& object, const bool doInline) {
    if (doInline) {
        return ImGui::DragFloat2(label, &object.x, 0.0025f, 0, 0, "%.4f", ImGuiSliderFlags_NoRoundToFormat);
    }

    bool changed = false;
    ImGui::PushID(label);
    if (ImGui::CollapsingHeader(label)) {
        ImGui::Indent();
        changed |= ImGui::DragFloat("X", &object.x, 0.0025f, 0, 0, "%.4f", ImGuiSliderFlags_NoRoundToFormat);
        changed |= ImGui::DragFloat("Y", &object.y, 0.0025f, 0, 0, "%.4f", ImGuiSliderFlags_NoRoundToFormat);
        ImGui::Unindent();
    }
    ImGui::PopID();
    return changed;
}

template<>
bool ExCore::IGuiDrawable<glm::vec3>::onGui(const char* label, glm::vec3& object, const bool doInline) {
    if (doInline) {
        return ImGui::DragFloat3(label, &object.x, 0.0025f, 0, 0, "%.4f", ImGuiSliderFlags_NoRoundToFormat);
    }

    bool changed = false;
    ImGui::PushID(label);
    if (ImGui::CollapsingHeader(label)) {
        ImGui::Indent();
        changed |= ImGui::DragFloat("X", &object.x, 0.0025f, 0, 0, "%.4f", ImGuiSliderFlags_NoRoundToFormat);
        changed |= ImGui::DragFloat("Y", &object.y, 0.0025f, 0, 0, "%.4f", ImGuiSliderFlags_NoRoundToFormat);
        changed |= ImGui::DragFloat("Z", &object.z, 0.0025f, 0, 0, "%.4f", ImGuiSliderFlags_NoRoundToFormat);
        ImGui::Unindent();
    }
    ImGui::PopID();
    return changed;
}

template<>
bool ExCore::IGuiDrawable<glm::vec4>::onGui(const char* label, glm::vec4& object, const bool doInline) {
    if (doInline) {
        return ImGui::DragFloat4(label, &object.x, 0.0025f, 0, 0, "%.4f", ImGuiSliderFlags_NoRoundToFormat);
    }

    bool changed = false;
    ImGui::PushID(label);
    if (ImGui::CollapsingHeader(label)) {
        ImGui::Indent();
        changed |= ImGui::DragFloat("X", &object.x, 0.0025f, 0, 0, "%.4f", ImGuiSliderFlags_NoRoundToFormat);
        changed |= ImGui::DragFloat("Y", &object.y, 0.0025f, 0, 0, "%.4f", ImGuiSliderFlags_NoRoundToFormat);
        changed |= ImGui::DragFloat("Z", &object.z, 0.0025f, 0, 0, "%.4f", ImGuiSliderFlags_NoRoundToFormat);
        changed |= ImGui::DragFloat("W", &object.w, 0.0025f, 0, 0, "%.4f", ImGuiSliderFlags_NoRoundToFormat);
        ImGui::Unindent();
    }
    ImGui::PopID();
    return changed;
}


namespace ExCore::Gui {
    bool searchDialogCenter(const char* label, char path[513], const char* types = nullptr);
    bool searchDialogLeft(const char* label, char path[513], const char* types = nullptr);

    template<typename T>
    bool drawEnumList(const char* label, T& value) {
        int32_t valueI(int32_t(value) - EnumNames<T>::Start);
        int32_t cur = valueI;

        valueI = valueI < 0 ? 0 : valueI >= EnumNames<T>::Count ? EnumNames<T>::Count - 1 : valueI;
 
        ImGui::PushID(label);
        ImGui::Combo(label, &valueI, EnumNames<T>::getEnumNames(), EnumNames<T>::Count);
        ImGui::PopID();

        if (cur != valueI) {
            value = T(valueI + EnumNames<T>::Start);
            return true;
        }
        return false;
    }
    bool drawTransform(const char* label, Transform& tr, Space* space = nullptr);
    bool drawSplitter(const bool splitVertical, const float thickness, float* size0, float* size1, float minSize0, float minSize1, float splitterAxisSize = -1.0f);
    bool drawSplitter(const char* id, const bool splitVertical, const float thickness, float* size0, float* size1, float minSize0, float minSize1, float splitterAxisSize = -1.0f);
}