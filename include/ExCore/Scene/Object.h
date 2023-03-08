#pragma once
#include <string>
#include <ExCore/Scene/Transform.h>
#include <ExCore/Scene/Component.h>
#include <ExCore/Scene/Time.h>
#include <memory>
#include <ExCore/Gui/IGuiDrawable.h>
#include <ExCore/Gui/IGuiExtras.h>
#include <misc/cpp/imgui_stdlib.h>

namespace ExCore {
    class Scene;
    class Object : public IGuiMeta<Space> {
    public:
        Object(const char* name, Scene* scene);
        ~Object();

        const std::string& setName(const std::string& newName) { return _name = newName; }
        const std::string& setName(const char* newName) { return _name = newName; }
        const std::string& getName() const { return _name; }

        Transform& getTransform() { return _transform; }
        const Transform& getTransform() const { return _transform; }

        void start();
        void update(const Time& time);

        const Scene* getScene() const { return _scene; }
        Scene* getScene() { return _scene; }

        template<typename T>
        std::weak_ptr<T> addComponent();

        template<typename T>
        std::weak_ptr<T> getComponent();


        template<typename T>
        bool removeComponent();

        bool removeComponent(const std::weak_ptr<Component>& component);

        void destroy();

    private:
        static constexpr uint8_t WILL_DELETE   = 0x1;
        friend struct IGuiDrawable<Object>;

        uint8_t _flags;
        std::string _name;
        Scene* _scene;

        Transform _transform;
        std::vector<std::shared_ptr<Component>> _components;

        friend class Scene;
        void actuallyDestroy(const bool doDelete);
        void removeComponentInternal(int32_t index, bool deleteComp);

        template<typename T>
        int32_t indexOfComponent() const;
        int32_t indexOfComponent(Component* component) const;

        friend class Component;
        bool removeComponentInternal(Component* component);
    };

    template<>
    bool IGuiDrawable<Object>::onGui(const char* label, Object& object, const bool doInline) {
        ImGui::PushID(&object);
        bool changed = false;
        if (ImGui::CollapsingHeaderNoId(label ? label : object._name.c_str(), ImGuiTreeNodeFlags_NoTreePushOnOpen)) {
            ImGui::Indent();
         
            ImGui::InputText("Name", &object._name);
            ImGui::Text("Scene: 0x%08x", uint64_t(object._scene));

            Gui::drawTransform("Transform", object._transform, &object.getExtraMeta());

            ImGui::Unindent();
        }
        ImGui::PopID();
        return changed;
    }

    template<typename T>
    inline std::weak_ptr<T> Object::addComponent() {
        auto& comp = _components.emplace_back(std::make_shared<T>(this));
        return std::static_pointer_cast<T>(comp);
    }

    template<typename T>
    inline std::weak_ptr<T> Object::getComponent() {
        int32_t index = indexOfComponent<T>();
        return index > -1 ? std::weak_ptr<T>(std::static_pointer_cast<T>(_components[i])) : {};
    }

    template<typename T>
    inline bool Object::removeComponent() {
        int32_t index = indexOfComponent<T>();
        if (index > -1) {
            removeComponentInternal(index);
            return true;
        }
        return false;
    }

    template<typename T>
    inline int32_t Object::indexOfComponent() const {
        for (size_t i = 0; i < _components.size(); i++) {
            if (dynamic_cast<T*>(_components[i])) { return i; }
        }
        return -1;
    }
}