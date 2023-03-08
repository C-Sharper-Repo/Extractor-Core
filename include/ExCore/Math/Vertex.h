#pragma once
#include <glm.hpp>
#include <ExCore/Math/Color32.h>

namespace ExCore {
    struct Vertex {
        glm::vec2 position;
        Color32 color;
        glm::vec2 uv;
    };
}