#include "attack_system.hpp"

void AttackSystem::update(EntityManager& em){

    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& ent, AttackComponent& att)
    {
        if(att.createAttack){
            vec3f vel = { 0 , 0 , 0 };
            if(ent.hasTag<PlayerTag>() && ent.hasComponent<InputComponent>()){
                auto& i = em.getComponent<InputComponent>(ent);
                if(i.last_key == i.down)
                    vel = { 0 , -1.0f , 0 };
                else if(i.last_key == i.up)
                    vel = { 0 , 1.0f , 0 };
                else if(i.last_key == i.right)
                    vel = { 1.0f , 0 , 0 };
                else if(i.last_key == i.left)
                    vel = { -1.0f , 0 , 0 };
            }

            auto& e{ em.newEntity() };
            em.addTag<HitPlayer>(e);
            auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = { 0.0f, 0.0f, 0.0f }, .scale = { 0.5f, 0.5f, 0.5f }, .color = BLACK });
            auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position = em.getComponent<PhysicsComponent>(ent).position, .velocity = vel });
            em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::ATTACK });
        
            att.createAttack =  false;
        }

        att.decreaseCountdown();
    });
}