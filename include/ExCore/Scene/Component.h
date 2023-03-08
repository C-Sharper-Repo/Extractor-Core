#pragma once
#include <ExCore/Scene/Time.h>

namespace ExCore {
    class Object;
    class Component {
    public:
        Component(Object* owner);
        virtual ~Component();

        virtual void start();
        virtual void update(const Time& time);
    
        void destroy();

        template<typename T>
        static constexpr char* getComponentName() {
            return "Component";
        }

    protected:
        Object* _owner;

    private:
        static constexpr uint8_t WILL_DELETE = 0x1;
        uint8_t _flags;
    };
}