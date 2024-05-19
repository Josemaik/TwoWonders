#pragma once

#include "../utils/types.hpp"
#include "../managers/game_engine.hpp"
#include <iostream>

struct AnimationSystem
{
    // Se van a buscar las entidad que tengan estos componentes y tags
    using SYSCMPs = MP::TypeList<RenderComponent, AnimationComponent>;
    using SYSTAGs = MP::TypeList<>;

    void update(EntityManager& em, GameEngine& engine);

private:
std::map<std::size_t, float> playerAnimMap = {
    {1, 2.0f},
    {2, 1.0f}
};
};

