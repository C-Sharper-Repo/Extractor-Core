#include <ExCore/Scene/Scene.h>

namespace ExCore {
    template<typename T, typename U>
    static inline bool smartPtrEquals(const T& a, const U& b) {
        return a.get() == b;
    }

    Scene::Scene() : _deleting(false), _time(), _camera() { }
    Scene::~Scene() {
        clear();
    }

    void Scene::start() {
        _time.reset();
    }

    void Scene::update() {
        for (auto obj : _objects) {
            obj->update(_time);
        }

        _time.tick();
        if (_objectsToRemove.size() > 0) {
            int32_t* indices = reinterpret_cast<int32_t*>(_malloca(_objectsToRemove.size() * sizeof(int32_t)));

            if (indices) {
                int c = 0;
                for (auto obj : _objectsToRemove) {
                    indices[c++] = _objects.indexOf(obj, smartPtrEquals);
                    obj->actuallyDestroy(true);
                }
                _objects.removeAt(indices, int32_t(_objectsToRemove.size()), true);
                _objectsToRemove.clear();
                _freea(indices);
            }
        }
    }

    std::weak_ptr<Object> Scene::spawnObject() {
        return spawnObject({ 0, 0 }, 0.0f, {1, 1});
    }   
    
    std::weak_ptr<Object> Scene::spawnObject(const char* name) {
        return spawnObject(name, { 0, 0 }, 0.0f, { 1, 1 });
    }

    std::weak_ptr<Object> Scene::spawnObject(const glm::vec2& position) {
        return spawnObject(position, 0.0f, { 1, 1 });
    }

    std::weak_ptr<Object> Scene::spawnObject(const char* name, const glm::vec2& position) {
        return spawnObject(name, position, 0.0f, { 1, 1 });
    }

    std::weak_ptr<Object> Scene::spawnObject(const glm::vec2& position, const float rotation) {
        return spawnObject(position, rotation, { 1, 1 });
    }

    std::weak_ptr<Object> Scene::spawnObject(const char* name, const glm::vec2& position, const float rotation) {
        return spawnObject(name, position, rotation, { 1, 1 });
    }

    std::weak_ptr<Object> Scene::spawnObject(const glm::vec2& position, const float rotation, const glm::vec2& scale) {
        return spawnObject("New Object", position, rotation, scale);
    }

    std::weak_ptr<Object> Scene::spawnObject(const char* name, const glm::vec2& position, const float rotation, const glm::vec2& scale) {
        auto spawned = std::make_shared<Object>(name, this);
        _objects.push(spawned);
        spawned->getTransform().setTRS(position, rotation, scale);
        return spawned;
    }

    bool Scene::removeObject(Object* object) {
        return object && _objectsToRemove.insert(object).second;
    }

    std::weak_ptr<Object> Scene::findObject(const char* name) {
        for (auto& obj : _objects) {
            if (obj->getName() == name) {
                return obj;
            }
        }
        return std::weak_ptr<Object>();
    }

    void Scene::clear()  {
        _deleting = true;

        for (int32_t i = 0; i < _objects.size(); i++) {
            _objects[i]->actuallyDestroy(false);
            _objects[i].reset();
        }

        _objectsToRemove.clear();

        _objects.clear();
        _deleting = false;
    }
}