#include "camera_system.hpp"

void CameraSystem::update(EntityManager& em, ENGI::GameEngine& ge, float dt)
{
    // Constantes de los distintos estados de la cámara
    static constexpr vec3d cameraPosSum = { 66.f, 70.f, 66.f };
    static constexpr vec3d cameraPosDetected = { 68.f, 80.f, 62.f };
    static constexpr vec3d cameraPosLocked = { 56.f, 90.f, 56.f };
    static constexpr float cameraFovyNormal = 70.f;
    static constexpr float cameraFovyDetected = 60.f;
    static constexpr float cameraFovyLocked = 55.f;
    static constexpr float cameraFovyCinematic = 50.f;

    auto& li = em.getSingleton<LevelInfo>();

    // Velocidad de la transición
    float t = 0.1f;

    if (li.viewPoint == vec3d::zero())
    {
        auto& playerEn = *em.getEntityByID(li.playerID);

        if (playerEn.hasTag<PlayerTag>())
        {
            auto& phy = em.getComponent<RenderComponent>(playerEn);
            cameraPos = phy.position + cameraPosSum;
            cameraTar = phy.position;
            cameraFovy = cameraFovyNormal;

            if (li.playerDetected && li.lockedEnemy == li.max)
            {
                if (!li.enemyPositions.empty())
                {
                    // Calcular la posición media de los enemigos
                    double x{}, y{}, z{};
                    for (auto& e : li.enemyPositions)
                    {
                        x += e.x();
                        y += e.y();
                        z += e.z();
                    }
                    x /= static_cast<double>(li.enemyPositions.size());
                    y /= static_cast<double>(li.enemyPositions.size());
                    z /= static_cast<double>(li.enemyPositions.size());

                    // Calcular el punto que está a un cuarto del camino entre la posición del jugador y la posición media de los enemigos
                    vec3d enemyPos = { x, y, z };
                    vec3d oneFourthPoint = phy.position + (enemyPos - phy.position) / 4.0;

                    cameraTar = oneFourthPoint;
                    cameraPos = oneFourthPoint + cameraPosDetected;
                    cameraFovy = cameraFovyDetected;
                }

            }
            else if (li.lockedEnemy != li.max)
            {
                auto& enemy = *em.getEntityByID(li.lockedEnemy);
                auto& lockedEnemyPos = em.getComponent<PhysicsComponent>(enemy).position;
                vec3d oneFourthPoint = phy.position + (lockedEnemyPos - phy.position) / 4.0;

                cameraTar = oneFourthPoint;
                cameraPos = oneFourthPoint + cameraPosLocked;

                cameraFovy = cameraFovyLocked;
            }
        }
    }
    else
    {
        // Asignamos el punto de vista a la cámara para señalar ahí
        cameraPos = li.viewPoint + cameraPosSum;
        cameraTar = li.viewPoint;
        cameraFovy = cameraFovyCinematic;

        // La cinematica se desactiva cuando pasan 4 segundos
        viewPointTime += dt;
        if (viewPointTime >= viewPointLimit)
        {
            viewPointTime = 0.f;
            li.viewPoint = vec3d::zero();
        }

        t = 0.05f;
    }

    if (li.transition)
    {
        transitionTime += dt;
        if (transitionTime >= transitionLimit)
        {
            transitionTime = 0.f;
            li.transition = false;
        }
        else
            t = 1.f;
    }

    // Pillamos la posición actual de la cámara, el punto de vista y el fovy
    vec3d currentCameraPos = ge.getPositionCamera();
    vec3d currentCameraTarget = ge.getTargetCamera();
    float currentCameraFovy = ge.getFovyCamera();

    // Interpolamos la posición, el punto de vista y el fovy
    vec3d newCameraPos = currentCameraPos + t * (cameraPos - currentCameraPos);
    vec3d newCameraTarget = currentCameraTarget + t * (cameraTar - currentCameraTarget);
    float newCameraFovy = currentCameraFovy + t * (cameraFovy - currentCameraFovy);

    // Asignamos la nueva posición, el nuevo punto de vista y el nuevo fovy a la cámara
    ge.setPositionCamera(newCameraPos);
    ge.setTargetCamera(newCameraTarget);
    ge.setFovyCamera(newCameraFovy);
}