#pragma once
#include <ExCore/Math/Matrix4f.h>
#include <vector>
#include <glm.hpp>
#include <ExCore/Util/EnumUtils.h>

namespace ExCore {
    enum class Space : uint8_t {
        None,
        Local,
        World,
    };

    template<>
    struct EnumNames<Space> {
        static constexpr int32_t Start{ 1 };
        static constexpr int32_t Count{ 2 };

        static const char** getEnumNames() {
            static const char* names[] =
            {
                "Local",
                "World",
            };
            return names;
        }
    };

    class Object;
    class Transform {
    public:
        Transform(Object* object);

        bool setParent(Transform* parent);
        Transform* getParent() { return _parent; }
        const Transform* getParent() const { return _parent; }

        Object* getObject() { return _object; }
        const Object* getObject() const { return _object; }

        void setTRS(const glm::vec2& pos, const float rot, const glm::vec2& scale, const Space space = Space::World);

        void translate(const glm::vec2& translation, const Space space = Space::World);
        void rotate(const float rotation, const Space space = Space::World);
        void scale(const glm::vec2& scale, const Space space = Space::World);

        void setPosition(const glm::vec2& pos, const Space space = Space::World);
        void setRotation(const float rot, const Space space = Space::World);
        void setScale(const glm::vec2& scale, const Space space = Space::World);

        const glm::vec2& getPosition(const Space space = Space::World) const { return space == Space::World ? _wPos : _lPos; }
        float getRotation(const Space space = Space::World) const { return space == Space::World ? _wRot : _lRot; }
        const glm::vec2& getScale(const Space space = Space::World) const { return space == Space::World ? _wSca : _lSca; }

        const Matrix4f& getLocalMatrix() const { return _local; }
        const Matrix4f& getWorldMatrix() const { return _world; }

        Object* addChild(Object* tr);
        Transform* addChild(Transform* tr);
        bool removeChild(Transform* tr);
        bool removeChild(Object* tr);

        size_t getChildCount() const { return _children.size(); }

        Transform* getChildAt(const size_t index);
        const Transform* getChildAt(const size_t index) const;

        Transform* find(const char* name) const;

    private:
        Object* _object;
        Transform* _parent;

        Matrix4f _local;
        Matrix4f _world;

        glm::vec2 _wPos;
        float _wRot;
        glm::vec2 _wSca;

        glm::vec2 _lPos;
        float _lRot;
        glm::vec2 _lSca;

        std::vector<Transform*> _children;

        void update(const Space space);
        void updateChildren();

        Object* addChildInternal(Object* tr, const bool isDirect);
        bool removeChildInternal(Object* tr, const bool isDirect);

        friend class Object;
        void destroyChildren();
    };
}