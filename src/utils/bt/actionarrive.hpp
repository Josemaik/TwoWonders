#pragma once
#include "node.hpp"
#include <utils/types.hpp>
struct BTActionArrive_t : BTNode_t{
    BTActionArrive_t(vec3f v) : where{v} {}
    void run() noexcept final { // final es como override sin dejar sobreescribir
        std::cout << "Arriving (" << where.x() << "," << where.y() << "," << where.z() << ")\n";
    }
private:
    vec3f where {};
};