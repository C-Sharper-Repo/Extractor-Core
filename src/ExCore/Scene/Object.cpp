#include <ExCore/Scene/Object.h>
#include <ExCore/Scene/Scene.h>
#include <ExCore/Log.h>

namespace ExCore {
    Object::Object(const char* name, Scene* scene) :_flags(0), _scene(scene), _name(name), _transform(this) { }

    Object::~Object() {}

    void Object::start() {
        for (auto& comp : _components) {
            if (comp) {
                comp->start();
            }
        }
    }

    void Object::update(const Time& time) {
        for (auto& comp : _components) {
            if (comp) {
                comp->update(time);
            }
        }
    }

    bool Object::removeComponent(const std::weak_ptr<Component>& component) {
        int32_t index = indexOfComponent(component.lock().get());
        if (index > -1) {
            removeComponentInternal(index, true);
            return true;
        }
        return false;
    }

    void Object::destroy() {
        if (_flags & WILL_DELETE) { return; }

        _flags |= WILL_DELETE;
        _transform.destroyChildren();

        _transform.setParent(nullptr);
        if (_scene) {
            _scene->removeObject(this);
            return;
        }
        delete this;
    }

    void Object::actuallyDestroy(const bool doDelete) {
        for (auto& comp : _components) {
            comp.reset();
        }
        _components.clear();

        if (doDelete) {
            delete this;
        }
    }

    void Object::removeComponentInternal(int32_t index, bool deleteComp) {
        if (index > -1) {
            std::shared_ptr<Component> compR = _components[index];
            auto comp = compR.get();
            _components.erase(_components.begin() + index);

            if (deleteComp && compR) {
                compR.reset();
            }
        }
    }

    int32_t Object::indexOfComponent(Component* component) const {
        for (int32_t i = 0; i < _components.size(); i++) {
            if (_components[i].get() == component) { return i; }
        }
        return -1;
    }

    bool Object::removeComponentInternal(Component* component) {
        int32_t index = indexOfComponent(component);
        if (index > -1) {
            removeComponentInternal(index, false);
            return true;
        }
        return false;
    }

}