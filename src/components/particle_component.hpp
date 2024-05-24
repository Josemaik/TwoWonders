#pragma once
#include <vector>
#include <cstdint>
#include <variant>
#include "../utils/vec3D.hpp"
#include "../motor/src/darkmoon.hpp"

struct Particle
{
    enum struct ParticleType { Pixel = 0, Texture };
    Particle(float lifeT = .6f) { lifeTime = lifeT; remainingLife = lifeTime; };

    vec3f position{ 0.f, 0.f, 0.f };
    vec3f initialPos{ 0.f, 0.f, 0.f };
    vec3f velocity{ 0.f, 0.f, 0.f };

    float lifeTime{ .6f };
    float remainingLife{ lifeTime };
    std::variant<DarkMoon::Color, std::string> color{};
    ParticleType type{ ParticleType::Pixel };
};

struct ParticleMakerComponent
{
    enum struct ParticleEffect { FIRE, FIREBALL, FIRESPLASH, SPARKLES, LAVA, CHEST, WATERSPLASH, WATER, SMOKE, OBJECT, PURPLEM, PRISONDOOR, PLAYER, MISSIONOBJ, MAX };

    bool active{ false };
    ParticleEffect effect{ ParticleEffect::CHEST };
    uint8_t maxParticles{ 0 };
    float spawnRate{ 0.f };
    float lifeTime{ 0.6f };
    float elapsed{ 0.f };
    bool multiply{ false };

    std::vector<std::pair<DarkMoon::Node*, Particle>> particles{};
};