#pragma once

#include "../utils/types.hpp"
#include "../managers/game_engine.hpp"
#include <iostream>

struct AnimationSystem
{
    // Se van a buscar las entidad que tengan estos componentes y tags
    using SYSCMPs = MP::TypeList<AnimationComponent,RenderComponent>;
    using SYSTAGs = MP::TypeList<>;

    void update(EntityManager& em,GameEngine& engine);

};

