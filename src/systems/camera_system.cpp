#include "camera_system.hpp"

void CameraSystem::update(EntityManager& em, GameEngine& ge, EventManager& evm)
{
    // Constantes de los distintos estados de la cámara
    static constexpr vec3d cameraPosSum = { -60.f, 66.f, -60.f };
    static constexpr vec3d cameraPosDetected = { -62.f, 76.f, -56.f };
    static constexpr vec3d cameraPosLocked = { -56.f, 80.f, -56.f };
    static constexpr float cameraFovyNormal = 100.f;
    static constexpr float cameraFovyDetected = 50.f;
    static constexpr float cameraFovyLocked = 47.5f;
    static constexpr float cameraFovyCinematic = 40.f;

    auto& li = em.getSingleton<LevelInfo>();
    if (li.isDead)
        return;

    // Velocidad de la transición
    float t = 0.1f;
    auto& playerEn = *em.getEntityByID(li.playerID);

    if (playerEn.hasTag<PlayerTag>())
    {
        auto& phy = em.getComponent<PhysicsComponent>(playerEn);
        if (li.viewPoint == vec3d::zero())
        {
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
        else
        {
            // Asignamos el punto de vista a la cámara para señalar ahí
            cameraPos = li.viewPoint + cameraPosSum;
            cameraTar = li.viewPoint;
            cameraFovy = cameraFovyCinematic;

            if (li.viewPointSound)
            {
                em.getSingleton<SoundSystem>().sonido_movimiento_camara();
                li.viewPointSound = false;
            }
            em.getSingleton<SoundSystem>().SFX_pasos_stop();

            if (!phy.notMove)
                phy.notMove = true;

            // La cinematica se desactiva cuando pasan 4 segundos
            viewPointTime += timeStep;
            if (viewPointTime >= viewPointLimit)
            {
                viewPointTime = 0.f;
                li.viewPoint = vec3d::zero();
                phy.notMove = false;

                if (!li.events.empty())
                {
                    for (auto& e : li.events)
                        evm.scheduleEvent(Event{ static_cast<EventCodes>(e) });

                    li.events.clear();
                }
            }

            t = 0.05f;
        }
    }

    if (li.transition)
    {
        transitionTime += timeStep;
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

    updateFrustum(em, ge, newCameraPos, newCameraTarget, newCameraFovy);
}

void CameraSystem::updateFrustum(EntityManager& em, GameEngine& ge, vec3d& cameraPos, vec3d& cameraTar, float cameraFovy)
{
    // Actualizamos el FrustumInfo de la cámara
    auto& frustum = em.getSingleton<FrustumInfo>();
    vec3f cameraUp = ge.getUpCamera().to_other<float>();
    float aspectRatio = ge.getAspectRat();

    // Calculate the bounds of the orthographic projection
    float halfHeight = static_cast<float>(std::tan(((cameraFovy) / 2.0f) * DEGTORAD) * cameraPos.distance(cameraTar));
    float halfWidth = aspectRatio * halfHeight;

    constexpr static float nearPlane = 0.1f;
    constexpr static float farPlane = 1000.0f;

    frustum.setFrustum(-halfWidth, halfWidth, -halfHeight, halfHeight, nearPlane, farPlane, cameraPos.to_other<float>(), cameraTar.to_other<float>(), cameraUp);

    using CMPs = MP::TypeList<>; // No components
    using TAGs = MP::TypeList<>; // No tags

    em.forEach<CMPs, TAGs>([&](Entity& e)
    {
        if (!e.hasTags(FrustOut{}) && e.hasComponent<ColliderComponent>())
        {
            auto& col = em.getComponent<ColliderComponent>(e);
            frustum.bboxIn(e.getID(), col.bbox);
        }
        else
            frustum.addToFrustum(e.getID());
    });
}