#include "camera_system.hpp"

void CameraSystem::update(EntityManager& em, ENGI::GameEngine& ge)
{
    auto& li = em.getSingleton<LevelInfo>();
    auto& playerEn = *em.getEntityByID(li.playerID);

    if (playerEn.hasTag<PlayerTag>())
    {
        vec3d pos{};
        auto& r = em.getComponent<RenderComponent>(playerEn);

        pos = { r.position.x() + 30.f, r.position.y() + 40.f, r.position.z() + 30.f };

        auto& li = em.getSingleton<LevelInfo>();
        if (li.playerDetected)
        {
            pos -= vec3d{ 2.f, 7.f, 2.f };
        }
        ge.setPositionCamera(pos);
        ge.setTargetCamera(r.position);
    }
}