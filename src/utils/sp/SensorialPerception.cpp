#include "SensorialPerception.hpp"

void SensorialPerception::HearDetection(EntityContext_t &ectx)
{
    // Calculo la distancia del player al enemigo
    auto const distance = (ectx.phy.position - getplayerphy(ectx).position).lengthSQ();
    auto const radius_listen_1 = (ectx.ai->detect_radius * ectx.ai->detect_radius);
    auto const radius_listen_2 = radius_listen_1 / 4.0;

    if (!ectx.ai->playerdetected) {
        bool in_alert_radius{ false };
        ectx.ai->listen_steps = true;
        // Si estás dentro de su rango de alerta
        if (distance < radius_listen_1) {
            // Aquí se escuchan los pasos un poco
            in_alert_radius = true;
            // Compruebo si se escuchan los pasos del player
            if (getplayerphy(ectx).velocity.x() == 0.0 && getplayerphy(ectx).velocity.z() == 0.0) {
                // No se escuchan
                ectx.ai->listen_steps = false;
            }
            // Nivel de escucha de los pasos
            ectx.ai->increase_angle = 0.5f;
        }

        if (distance < radius_listen_2) {
            // Aquí se escuchan los pasos más fuerte
            in_alert_radius = true;
            // Compruebo si se escuchan los pasos del player
            if (getplayerphy(ectx).velocity.x() == 0.0 && getplayerphy(ectx).velocity.z() == 0.0) {
                // No se escuchan
                ectx.ai->listen_steps = false;
            }
            // Nivel de escucha de los pasos
            ectx.ai->increase_angle = 1.0f;
        }
        // Si no entro en ninguno de los radios de escucha no entra en estado de alerta
        if (!in_alert_radius) {
            ectx.ai->alert_state = false;
            // Además no me detecta
            ectx.ai->playerdetected = false;
        } else {
            ectx.ai->alert_state = true;
        }
        // Si se completa la barra de escucha, me detectan
        // Dibujar icono alerta encima de enemigo
        if (ectx.ai->endangle <= -360.0f) {
            ectx.ai->playerdetected = true;
            ectx.ai->alert_state = false;
            ectx.ai->endangle = 0.0f;
        }
    } else {
        ectx.ai->alert_state = false;
        ectx.ai->endangle = 0.0f;
    }
}

void SensorialPerception::SightDetection(EntityContext_t &ectx)
{
    // En nivel 3 no pueden detectarte por diferencia de altura en el nivel
    auto& li = ectx.em.getSingleton<LevelInfo>();
    if (li.mapID == 3 && (ectx.ent.hasTag<GolemTag>() || ectx.ent.hasTag<SlimeTag>() || ectx.ent.hasTag<GolemTag>())) {
        if (std::abs(getplayerphy(ectx).position.y() - ectx.phy.position.y()) > MIN_HIGHT_DETECTTION) {
            ectx.ai->playerdetected = false;
            ectx.ai->alert_state = false;
            return;
        }
    }

    // Calcula la dirección de visión del enemigo utilizando su orientación
    vec3d enemyDirection = vec3d(std::sin(ectx.phy.orientation), 0.0, std::cos(ectx.phy.orientation));
    // Vector que apunta desde el enemigo al jugador
    vec3d toPlayer = getplayerphy(ectx).position - ectx.phy.position;
    // Normalizamos vectores
    enemyDirection.normalize();
    toPlayer.normalize();
    // Producto escalar
    double dotProduct = enemyDirection.dotProduct(toPlayer);
    // Calcula el ángulo entre la dirección de visión del enemigo y la dirección hacia el jugador
    double angle = std::acos(dotProduct);
    // Convierte el ángulo de radianes a grados
    double angleDegrees = angle * 180 / K_PI;

    // Comprueba si el ángulo está dentro del rango de visión del enemigo
    if (angleDegrees < verticalFOV / 2.0 && std::abs(angleDegrees) < horizontalFOV / 2.0) {
        // Raycast
        vec3d intersection_wall{}, intersection_player{};
        // Realiza un raycast en la dirección en la que el enemigo está mirando para detectar obstáculos
        auto& phyplayer = getplayerphy(ectx);
        auto& plpos = phyplayer.position;
        vec3d dir = vec3d{plpos.x(), plpos.y(), plpos.z()} - ectx.phy.position;
        dir.normalize();
        RayCast ray = {vec3d{ectx.phy.position.x(), ectx.phy.position.y(), ectx.phy.position.z()}, dir};

        // Dibujado de raycast
        auto& bb = ectx.em.getSingleton<BlackBoard_t>();
        bb.position_origin = ray.origin;
        bb.direction = ray.direction;
        bb.launched = true;

        auto& bbox = getplayercollider(ectx).bbox;

        // Calculo punto de intersección con player
        bbox.intersectsRay(ray.origin, ray.direction, intersection_player);

        // Compruebo si la caja de colisión de un obstáculo ha colisionado con el rayo
        for (Entity& ent : ectx.em.getEntities()) {
            // Comprobamos entidades Pared y que tengan colisión component
            if (ent.hasComponent<ColliderComponent>()) {
                if (ent.hasTag<WallTag>() || ent.hasTag<LavaTag>()) {
                    auto& col = ectx.em.getComponent<ColliderComponent>(ent);
                    if (col.bbox.intersectsRay(ray.origin, ray.direction, intersection_wall)) {
                        if (intersection_wall.distance(ectx.phy.position) < MinRayDistance) {
                            // El primero que es encontrado, rompe el bucle
                            break;
                        }
                    }
                }
            }
        }
        // Comprobamos las distancias a los puntos de intersección
        if (intersection_wall.distance(ectx.phy.position) < intersection_player.distance(ectx.phy.position)) {
            // Hay un obstáculo delante
            ectx.ai->playerdetected = false;
        } else {
            if (intersection_player.distance(ectx.phy.position) > ectx.ai->detect_radius) {
                ectx.ai->playerdetected = false;
            } else {
                ectx.ai->alert_state = false;
                ectx.ai->playerdetected = true;
                auto& bb = ectx.em.getSingleton<BlackBoard_t>();
                for (auto it = bb.slimeData.begin(); it != bb.slimeData.end(); ++it) {
                    auto& slime = *ectx.em.getEntityByID(it->first);
                    if (it->first != ectx.ent.getID() && slime.hasComponent<AIComponent>()) {
                        auto const dis = ectx.phy.position.distance(it->second.position);
                        auto& aic = ectx.em.getComponent<AIComponent>(slime);
                        if (dis < (5.0 * 5.0) && !aic.playerdetected) {
                            aic.playerdetected = true;
                        }
                    }
                }
            }
        }
    }
}
