#include <ExCore/Scene/Object.h>
#include <ExCore/Scene/Component.h>
#include <ExCore/Scene/Scene.h>
#include <ExCore/Log.h>

namespace ExCore {
    Component::Component(Object* owner) : _flags(), _owner(owner) {}

    Component::~Component() {}

    void Component::start() {}
    void Component::update(const Time& time) { }

    void Component::destroy() {
        if (!(_flags & WILL_DELETE)) {
            _flags |= WILL_DELETE;
            if (_owner) {
                _owner->removeComponentInternal(this);
                return;
            }
            delete this;
        }
    }
}