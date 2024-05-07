#pragma once

#include <glm/glm.hpp>

struct BoneInfo
        {
            //id of index in finalbonematrices
            int id{};
            //offset matrix transform vertex from model space to bone space
            glm::mat4 offset{};
};
