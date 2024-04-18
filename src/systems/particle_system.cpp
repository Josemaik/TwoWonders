#include "particle_system.hpp"

void ParticleSystem::update(EntityManager& em)
{
    // Definimos el mapa de efectos de partículas
    static std::map<Effects, std::pair<vec3f, std::vector<Color>>> particleEffects = {
    { Effects::CHEST, { { 10.0f, 0.1f, 5.0f }, { {255, 215, 0, 255} } } },
    { Effects::SPARKS, { { 5.0f, 0.1f, 5.0f }, { {255, 215, 0, 255} } } },
    { Effects::FIRE, { { 2.0f, 0.01f, 2.0f }, { {255, 0, 0, 255}, {156, 50, 52, 255} } } },
    { Effects::SPARKLES, { { 1.0f, 0.01f, 1.0f }, { {255, 0, 0, 255}, {156, 50, 52, 255} } } },
    { Effects::SMOKE, { { 3.0f, 0.01f, 3.0f }, { {56, 50, 52, 255}, {130, 129, 129, 255 } } } },
    { Effects::PURPLEM, { { 1.0f, 0.0001f, 1.0f }, { {128, 0, 0, 255} } } },
    { Effects::LAVA, { { 5.0f, 0.01f, 15.0f }, {{255, 0, 0, 255}, {156, 50, 52, 255}, { 130, 129, 129, 255 }} } },
    { Effects::WATER, { { 2.0f, -0.5f, 2.0f }, { {0, 0, 255, 255} } } },
    { Effects::FIREBALL, { { 2.0f, 0.5f, 2.0f }, { {255, 0, 0, 255}, {130, 129, 129, 255} } } },
    { Effects::OBJECT, { { 1.0f, 0.2f, 1.0f }, {  {255, 215, 0, 255}, { 255, 119, 0, 255 } } } }
    };

    // La parte del motor gráfico será poder colocar puntos de luz desde donde se generen las partículas sjsjsj

    auto& frti = em.getSingleton<FrustumInfo>();
    em.forEach<SYSCMPs, SYSTAGs>([&](Entity&, PhysicsComponent& phy, ColliderComponent& col, ParticleMakerComponent& pmc)
    {
        if (frti.bboxInFrustum(col.boundingBox) == FrustumInfo::Position::OUTSIDE)
            return;

        // Si el componente tiene activo su efecto de partículas
        if (pmc.active)
        {
            // Revisamos que se creen las partículas
            pmc.elapsed += timeStep;
            if (pmc.elapsed >= pmc.spawnRate)
            {
                pmc.elapsed = 0.f;
                if (pmc.particles.size() < pmc.maxParticles)
                {
                    Particle p(pmc.lifeTime);
                    // Asignamos la posición
                    float randomFloatX{};
                    float randomFloatZ{};
                    float rangeX{};
                    float rangeZ{};

                    if (std::abs(std::sin(phy.orientation)) != 1)
                    {
                        rangeX = particleEffects[pmc.effect].first.x();
                        rangeZ = particleEffects[pmc.effect].first.z();
                    }
                    else
                    {
                        rangeX = particleEffects[pmc.effect].first.z();
                        rangeZ = particleEffects[pmc.effect].first.x();
                    }

                    randomFloatX = static_cast<float>(phy.position.x()) - rangeX / 2 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / rangeX);
                    randomFloatZ = static_cast<float>(phy.position.z()) - rangeZ / 2 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / rangeZ);

                    p.position = { randomFloatX, static_cast<float>(phy.position.y()), randomFloatZ };
                    p.initialPos = p.position;

                    // Asignamos la velocidad
                    randomFloatX = 0.1f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / (0.2f - 0.1f));
                    randomFloatZ = 0.1f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / (0.2f - 0.1f));
                    p.velocity = { randomFloatX, particleEffects[pmc.effect].first.y(), randomFloatZ };

                    // Asignamos el color
                    std::vector<Color>& colors = particleEffects[pmc.effect].second;
                    Color& color = colors[std::rand() % colors.size()];
                    p.r = color.r;
                    p.g = color.g;
                    p.b = color.b;
                    p.a = color.a;

                    // La metemos en el vector de partículas
                    pmc.particles.push_back(p);
                }
            }

            // Actualizamos las partículas
            for (auto& p : pmc.particles)
            {
                p.remainingLife -= timeStep;
                if (p.remainingLife <= 0.f)
                {
                    if (pmc.effect != Effects::WATER && pmc.effect != Effects::FIREBALL)
                        p.position = p.initialPos;
                    else
                        p.position = phy.position.to_other<float>();
                    p.remainingLife = p.lifeTime;
                }
                else
                {
                    if (pmc.effect == Effects::WATER)
                        std::cout << "Pos: " << p.position << "\n";
                    // Movemos las partículas acorde con el efecto del creador de partículas
                    if (pmc.multiply)
                        p.position += p.velocity * 1.2f;
                    else
                        p.position += p.velocity;
                }
            }
        }
    });
}