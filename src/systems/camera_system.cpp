#include "camera_system.hpp"

void CameraSystem::update(EntityManager& em, ENGI::GameEngine& ge, float dt)
{
    auto& li = em.getSingleton<LevelInfo>();
    auto& playerEn = *em.getEntityByID(li.playerID);

    if (playerEn.hasTag<PlayerTag>())
    {
        auto& phy = em.getComponent<RenderComponent>(playerEn);
        cameraPos = { phy.position.x() + 66.f, phy.position.y() + 70.f, phy.position.z() + 66.f };
        cameraTar = phy.position;
        cameraFovy = 70.f;
        li.cameraChange = false;
        if (li.cameraChange)
            cameraPos = { phy.position.x() - 66.f, phy.position.y() + 70.f, phy.position.z() + 66.f };

        if (li.playerDetected && li.lockedEnemy == li.max)
        {
            if (!li.enemyPositions.empty())
            {
                double x{ 0.0 }, y{ 0.0 }, z{ 0.0 };
                for (auto& e : li.enemyPositions)
                {
                    x += e.x();
                    y += e.y();
                    z += e.z();
                }
                x /= static_cast<double>(li.enemyPositions.size());
                y /= static_cast<double>(li.enemyPositions.size());
                z /= static_cast<double>(li.enemyPositions.size());

                // Calcular el punto que está a un tercio del camino entre la posición del jugador y la posición media de los enemigos
                vec3d enemyPos = { x, y, z };
                vec3d oneFourthPoint = phy.position + (enemyPos - phy.position) / 4.0;

                cameraTar = oneFourthPoint;
                cameraPos = { oneFourthPoint.x() + 66.f, oneFourthPoint.y() + 70.f, oneFourthPoint.z() + 66.f };

                if (li.cameraChange)
                    cameraPos = { oneFourthPoint.x() - 66.f, oneFourthPoint.y() + 70.f, oneFourthPoint.z() + 66.f };
            }


            cameraPos += vec3d{ 2.f, 10.f, 2.f };
            cameraFovy = 60.f;
        }
        else if (li.lockedEnemy != li.max)
        {
            auto& enemy = *em.getEntityByID(li.lockedEnemy);
            auto& lockedEnemyPos = em.getComponent<PhysicsComponent>(enemy).position;
            vec3d oneFourthPoint = phy.position + (lockedEnemyPos - phy.position) / 4.0;

            cameraTar = oneFourthPoint;
            cameraPos = { oneFourthPoint.x() + 56.f, oneFourthPoint.y() + 90.f, oneFourthPoint.z() + 56.f };

            if (li.cameraChange)
                cameraPos = { oneFourthPoint.x() - 56.f, oneFourthPoint.y() + 90.f, oneFourthPoint.z() + 56.f };

            cameraFovy = 55.f;
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

        // Get current camera position, target and fovy
        vec3d currentCameraPos = ge.getPositionCamera();
        vec3d currentCameraTarget = ge.getTargetCamera();
        float currentCameraFovy = ge.getFovyCamera();

        // Interpolate between current and new camera position, target and fovy
        vec3d newCameraPos = currentCameraPos + t * (cameraPos - currentCameraPos);
        vec3d newCameraTarget = currentCameraTarget + t * (cameraTar - currentCameraTarget);
        float newCameraFovy = currentCameraFovy + t * (cameraFovy - currentCameraFovy);

        // Set new camera position, target and fovy
        ge.setPositionCamera(newCameraPos);
        ge.setTargetCamera(newCameraTarget);
        ge.setFovyCamera(newCameraFovy);
    }
}