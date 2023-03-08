#pragma once
#include <ExCore/Scene/Component.h>

namespace ExCore {
    class Animator : public Component {
    public:

    private:

    };

    template<>
    static constexpr char* Component::getComponentName<Animator>() {
        return "Animator";
    }
}