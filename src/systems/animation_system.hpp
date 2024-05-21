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
    static inline constexpr std::array<float, 11> playerSpeeds = {
        3.0, 6.0, 3.0, 4.0, 1.0, 1.0, 2.0, 2.0, 2.0, 3.0, 2.0
    };

    static inline constexpr std::array<float, 4> snowmanSpeeds = {
        5.0, 2.0, 2.0, 1.0
    };

    static inline constexpr std::array<float, 4> nomadSpeeds = {
        3.0, 2.0, 2.0, 3.0
    };
};