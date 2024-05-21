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
        3.0f, 6.0f, 3.0f, 4.0f, 2.5f, 1.0f, 2.0f, 2.0f, 2.0f, 3.0f, 2.0f
    };

    static inline constexpr std::array<float, 4> snowmanSpeeds = {
        5.0f, 2.0f, 2.0f, 1.0f
    };

    static inline constexpr std::array<float, 4> nomadSpeeds = {
        3.0f, 2.0f, 2.0f, 3.0f
    };

    static inline constexpr std::array<float, 4> catSpeeds = {
        2.0f, 4.0f, 2.0f, 3.0f
    };
};