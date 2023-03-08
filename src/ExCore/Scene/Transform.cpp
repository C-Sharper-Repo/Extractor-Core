#include <ExCore/Scene/Transform.h>
#include <ExCore/Scene/Object.h>
#include <glm/gtc/matrix_transform.hpp>

namespace ExCore {
    Transform::Transform(Object* object) :
        _object(object), _parent(nullptr), _children{}, _world(), _local(), _lPos(), _lRot(), _lSca(1), _wPos(), _wRot(), _wSca(1) { }

    bool Transform::setParent(Transform* parent) {
        if (parent == &_object->getTransform()) { return false; }
        if (_parent == parent) { return true; }

        if (_parent) {
            _parent->removeChildInternal(_object, false);
        }

        _parent = parent;
        if (_parent) {
            _parent->_children.emplace_back(this);
        }
        update(Space::Local);
        return true;
    }

    void Transform::setTRS(const glm::vec2& pos, const float rot, const glm::vec2& scale, const Space space) {
        switch (space) {
            case Space::Local:
                _lPos = pos;
                _lRot = rot;
                _lSca = scale;
                _local.setTRS(pos, rot, scale);
                update(Space::World);
                break;
            case Space::World:
                _wPos = pos;
                _wRot = rot;
                _wSca = scale;
                _world.setTRS(pos, rot, scale);
                update(Space::Local);
                break;
        }
    }

    void Transform::translate(const glm::vec2& translation, const Space space) {
        switch (space) {
            case Space::Local:
                _lPos += translation;
                _local.translate(translation);
                update(Space::World);
                break;
            case Space::World:
                _wPos += translation;
                _world.translate(translation);
                update(Space::Local);
                break;
        }
    }

    void Transform::rotate(const float rotation, const Space space) {
        switch (space) {
            case Space::Local:
                _lRot += rotation;
                _local.rotate(rotation);
                update(Space::World);
                break;
            case Space::World:
                _wRot += rotation;
                _world.rotate(rotation);
                update(Space::Local);
                break;
        }
    }

    void Transform::scale(const glm::vec2& scale, const Space space) {
        switch (space) {
            case Space::Local:
                _lSca *= scale;
                _local.scale(scale);
                update(Space::World);
                break;
            case Space::World:
                _wSca *= scale;
                _world.scale(scale);
                update(Space::Local);
                break;
        }
    }

    void Transform::setPosition(const glm::vec2& pos, const Space space) {
        switch (space) {
            case Space::Local:
                _lPos = pos;
                _local.setTRS(_lPos, _lRot, _lSca);
                update(Space::World);
                break;
            case Space::World:
                _wPos = pos;
                _world.setTRS(_wPos, _wRot, _wSca);
                update(Space::Local);
                break;
        }
    }

    void Transform::setRotation(const float rot, const Space space) {
        switch (space) {
            case Space::Local:
                _lRot = rot;
                _local.setTRS(_lPos, _lRot, _lSca);
                update(Space::World);
                break;
            case Space::World:
                _wRot = rot;
                _world.setTRS(_wPos, _wRot, _wSca);
                update(Space::Local);
                break;
        }
    }

    void Transform::setScale(const glm::vec2& scale, const Space space) {
        switch (space) {
            case Space::Local:
                _lSca = scale;
                _local.setTRS(_lPos, _lRot, _lSca);
                update(Space::World);
                break;
            case Space::World:
                _wSca = scale;
                _world.setTRS(_wPos, _wRot, _wSca);
                update(Space::Local);
                break;
        }
    }

    Object* Transform::addChild(Object* tr) { return addChildInternal(tr, true); }
    Transform* Transform::addChild(Transform* tr) { 
        Object* trO = addChildInternal(tr ? tr->_object : nullptr, true);
        return trO ? &trO->getTransform() : nullptr;
    }

    bool Transform::removeChild(Transform* tr) { return removeChildInternal(tr ? tr->_object : nullptr, true); }
    bool Transform::removeChild(Object* tr) { return removeChildInternal(tr, true); }

    Transform* Transform::getChildAt(const size_t index) {
        assert(index >= 0 && index < _children.size() && "Index out of range!");
        return _children[index];
    }

    const Transform* Transform::getChildAt(const size_t index) const {
        assert(index >= 0 && index < _children.size() && "Index out of range!");
        return _children[index];
    }

    Transform* Transform::find(const char* name) const {
        for (const Transform* child : _children) {
            if (child->_object && strcmp(name, child->_object->getName().c_str())) { return const_cast<Transform*>(child); }
        }
        return nullptr;
    }

    void Transform::update(const Space space) {
        if (space == Space::Local) {
            if (_parent) {
                Matrix4f inv = _world.getInverse();
                _lPos = inv.transformPoint(_wPos);
                _lSca = { _wSca.x / _parent->_wSca.x, _wSca.y / _parent->_wSca.y };
                _lRot = _parent->_wRot - _wRot;

                _local.setTRS(_lPos, _lRot, _lSca);
            }
            else {
                _lPos = _wPos;
                _lRot = _wRot;
                _lSca = _wSca;
                _local = _world;
            }
            updateChildren();
        } else if (space == Space::World) {
            if (_parent) {
                _wPos = _parent->_world.transformPoint(_wPos);
                _wSca = { _parent->_wSca.x * _lSca.x, _parent->_wSca.y * _lSca.y };
                _wRot = _parent->_wRot - _lRot;

                _world.setTRS(_wPos, _wRot, _wSca);
            }
            else {
                _wPos = _lPos;
                _wRot = _lRot;
                _wSca = _lSca;
                _world = _local;
            }
            updateChildren();
        }
    }

    void Transform::updateChildren() {
        for (Transform* child : _children) {
            child->update(Space::World);
        }
    }

    Object* Transform::addChildInternal(Object* tr, const bool isDirect) {
        Transform* trPtr = tr ? &tr->getTransform() : nullptr;

        if (trPtr && tr != _object && trPtr->_parent != &_object->getTransform()) {

            if (trPtr->_parent) {
                trPtr->_parent->removeChild(&tr->getTransform());
            }
            trPtr->_parent = &_object->getTransform();
            _children.emplace_back(&tr->getTransform());

            if (isDirect) {
                trPtr->update(Space::Local);
            }
        }
        return tr;
    }

    bool Transform::removeChildInternal(Object* tr, const bool isDirect) {
        if (tr && tr == _object) {
            auto find = std::find(_children.begin(), _children.end(), &tr->getTransform());
            if (find != _children.end()) {
                _children.erase(find);
                if (isDirect) {
                    tr->getTransform()._parent = nullptr;
                    tr->getTransform().update(Space::World);
                }
                return true;
            }
        }
        return false;
    }

    void Transform::destroyChildren() {
        for (size_t i = 0; i < _children.size(); i++) {
            _children[i]->_object->destroy();
        }
        _children.clear();
    }
}