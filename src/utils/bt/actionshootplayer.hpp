#pragma once
#include "node.hpp"
#include <utils/types.hpp>

struct BTActionShootPlayer : BTNode_t{
    //Funcion para saber si el player ha sido detectado o no
    [[nodiscard]] bool isPlayerDetected(EntityManager& EM, PhysicsComponent const& p, AIComponent const& ai) const noexcept {
        auto& li = EM.getSingleton<LevelInfo>();
        auto* playerEn = EM.getEntityByID(li.playerID);
        if (not playerEn) return false;
        auto& plphy = EM.getComponent<PhysicsComponent>(*playerEn);
        auto const distance = (p.position - plphy.position).lengthSQ();
        return  distance < (ai.detect_radius * ai.detect_radius);
    }
    // Obtener la distancia del enemigo con respecto al player
    [[nodiscard]] vec3f getPlayerDistance(EntityManager& EM, PhysicsComponent const& p, AIComponent& ai) const noexcept {
        auto& li = EM.getSingleton<LevelInfo>();
        auto* playerEn = EM.getEntityByID(li.playerID);
        if (not playerEn) { ai.playerdetected = false; return vec3f{}; };
        auto& plphy = EM.getComponent<PhysicsComponent>(*playerEn);
        auto const distance = plphy.position - p.position;
        return  distance;
    }
    vec3f getRandomPosinRange(float xmin, float xmax, float zmin, float zmax) {
        //Semilla para generar numeros aleatorios
        std::random_device rd;
        std::mt19937 gen(rd());
        // creo rangos
        std::uniform_real_distribution<float> rangoX(xmin, xmax);
        std::uniform_real_distribution<float> rangoZ(zmin, zmax);
        // obtengo x y z aleatoria
        float x = rangoX(gen);
        float z;
        do {
            z = rangoZ(gen);
        } while (z >= -18.0f && z <= -13.0f);

        //devuelvo vector
        return vec3f{ x,0.0f,z };
    }
    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        // if (!ectx.ai.shoot) {
            if (ectx.ai.elapsed_change_position >= ectx.ai.countdown_change_position) {
                //before change position go visible
                auto& rend = ectx.em.getComponent<RenderComponent>(ectx.ent);
                rend.visible = true;
                //Set random position
                vec3f randomPos = getRandomPosinRange(ectx.ai.Xmin, ectx.ai.Xmax, ectx.ai.Zmin, ectx.ai.Zmax);
                ectx.phy.position.setX(randomPos.x());
                ectx.phy.position.setZ(randomPos.z());
                //Attack
                // ectx.ai.shoot = true;
                ectx.ai.elapsed_change_position = 0;
                return BTNodeStatus_t::success;
            }
            ectx.ai.dec_countdown_change_pos(ectx.deltatime);
            return BTNodeStatus_t::running;
    }
};