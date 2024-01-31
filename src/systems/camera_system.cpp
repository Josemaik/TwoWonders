#include "camera_system.hpp"

void CameraSystem::update(EntityManager& em, ENGI::GameEngine& ge, float dt)
{
    auto& li = em.getSingleton<LevelInfo>();
    auto& playerEn = *em.getEntityByID(li.playerID);

    if (playerEn.hasTag<PlayerTag>())
    {
        auto& r = em.getComponent<RenderComponent>(playerEn);
        cameraPos = { r.position.x() + 33.f, r.position.y() + 43.f, r.position.z() + 32.f };
        cameraTar = r.position;

        auto& li = em.getSingleton<LevelInfo>();
        if (li.playerDetected)
        {
            if (!li.enemyPositions.empty())
            {
                float x{ 0.f }, y{ 0.f }, z{ 0.f };
                for (auto& e : li.enemyPositions)
                {
                    x += e.x();
                    y += e.y();
                    z += e.z();
                }
                x /= li.enemyPositions.size();
                y /= li.enemyPositions.size();
                z /= li.enemyPositions.size();

                // Calcular el punto que está a un tercio del camino entre la posición del jugador y la posición media de los enemigos
                vec3d enemyPos = { x, y, z };
                vec3d oneThirdPoint = r.position + (enemyPos - r.position) / 4.0;

                cameraTar = oneThirdPoint;
                cameraPos = { oneThirdPoint.x() + 33.f, oneThirdPoint.y() + 43.f, oneThirdPoint.z() + 33.f };
            }

            cameraPos -= vec3d{ 2.f, 7.f, 2.f };
        }

        float t = 0.1f; // Velocidad de la transición
        if (li.transition)
        {
            transitionTime += dt;
            if (transitionTime >= transitionLimit)
            {
                transitionTime = 0.f;
                li.transition = false;
            }
            else
            {
                t = 1.f;
            }
        }
        vec3d currentCameraPos = ge.getPositionCamera();
        vec3d currentCameraTarget = ge.getTargetCamera();
        vec3d newCameraPos = currentCameraPos + t * (cameraPos - currentCameraPos);
        vec3d newCameraTarget = currentCameraTarget + t * (cameraTar - currentCameraTarget);
        ge.setPositionCamera(newCameraPos);
        ge.setTargetCamera(newCameraTarget);
    }
}