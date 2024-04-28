#pragma once
#include "entity.hpp"
#include "../utils/color.hpp"

#include <vector>
#include <glm/glm.hpp>

namespace DarkMoon {
    struct Light : Entity {
        Color color = D_WHITE;

        Light(Color c)
            : color(c) {};
    };

    struct PointLight : Light {
        glm::vec3 position{};
        float constant{ 1.0f };      // 1.0   // 0.9
        float linear{ 0.09f };       // 0.09  // 0.007
        float quadratic{ 0.032f };   // 0.032 // 0.00008

        PointLight(glm::vec3 pos, Color c)
            : Light(c), position(pos) {};
    };

    struct DirectionalLight : Light {
        glm::vec3 direction{};

        DirectionalLight(glm::vec3 dir, Color c)
            : Light(c), direction(dir) {};
    };
}
