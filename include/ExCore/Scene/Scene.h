#pragma once
#include <ExCore/Rendering/ICamera.h>
#include <ExCore/Scene/Object.h>
#include <vector>
#include <set>
#include <ExCore/Scene/Time.h>
#include <ExCore/Util/HeapVector.h>
#include <memory>

namespace ExCore {
    class Scene {
    public:
        Scene();
        ~Scene();

        void start();
        void update();

        std::weak_ptr<Object> spawnObject();
        std::weak_ptr<Object> spawnObject(const char* name);
        std::weak_ptr<Object> spawnObject(const glm::vec2& position);
        std::weak_ptr<Object> spawnObject(const char* name, const glm::vec2& position);
        std::weak_ptr<Object> spawnObject(const glm::vec2& position, const float rotation);
        std::weak_ptr<Object> spawnObject(const char* name, const glm::vec2& position, const float rotation);
        std::weak_ptr<Object> spawnObject(const glm::vec2& position, const float rotation, const glm::vec2& scale);
        std::weak_ptr<Object> spawnObject(const char* name, const glm::vec2& position, const float rotation, const glm::vec2& scale);

        void setCamera(const std::weak_ptr<ICamera>& camera) { _camera = camera; }
        std::weak_ptr<ICamera> getCamera() { return _camera; };
        const std::weak_ptr<ICamera> getCamera() const { return _camera; }

        const Time& getTime() const { return _time; }
        const HeapVector<std::shared_ptr<Object>>& getObjects() const { return _objects; }

        std::weak_ptr<Object> findObject(const char* name);

        void clear();

    private:
        HeapVector<std::shared_ptr<Object>> _objects;
        std::set<Object*> _objectsToRemove;
     
        std::weak_ptr<ICamera> _camera;
        bool _deleting;

        Time _time;

    private:
        friend class Object;
        bool removeObject(Object* object);
    };
}