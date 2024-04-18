#pragma once
#include <vector>
#include <cstdint>
#include "../utils/vec3D.hpp"

struct Particle
{
    enum struct ParticleType { Pixel = 0, Texture };
    Particle(float lifeT = .6f) { lifeTime = lifeT; remainingLife = lifeTime; };

    vec3f position{ 0.f, 0.f, 0.f };
    vec3f initialPos{ 0.f, 0.f, 0.f };
    vec3f velocity{ 0.f, 0.f, 0.f };

    float lifeTime{ .6f };
    float remainingLife{ lifeTime };
    unsigned char r{}, g{}, b{}, a{};
    ParticleType type{ ParticleType::Pixel };
};

struct ParticleMakerComponent
{
    enum struct ParticleEffect { FIRE, FIREBALL, SPARKLES, LAVA, CHEST, LEVER, SPARKS, WATER, SMOKE, DUST, PURPLEM, MAX };

    bool active{ false };
    ParticleEffect effect{ ParticleEffect::CHEST };
    uint8_t maxParticles{ 0 };
    float spawnRate{ 0.f };
    float lifeTime{ 0.6f };
    float elapsed{ 0.f };
    bool multiply{ false };

    std::vector<Particle> particles{};
};