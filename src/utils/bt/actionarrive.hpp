#pragma once
#include "node.hpp"
#include <utils/types.hpp>
struct BTActionArrive_t : BTNode_t{
    BTActionArrive_t(vec3f v) : where{v} {}

    // Referencia a EntityManager
    // Referencia a AIComponent de la entidad
    // Referencia a PhysicsComponent
    // Referencia a Entity


    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        std::cout << "Arriving (" << where.x() << "," << where.y() << "," << where.z() << ")\n";
        return BTNodeStatus_t::success;
    }
private:
    vec3f where {};
};