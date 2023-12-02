#include "attack_system.hpp"

void AttackSystem::update(EntityManager& em){

    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, AttackComponent& att)
    {
        if(att.createAttack){
            auto& e{ em.newEntity() };
            em.addTag<HitPlayer>(e);
            auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = { 0.0f, 0.0f, 0.0f }, .scale = { 0.5f, 0.5f, 0.5f }, .color = BLACK });
            auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position = { 0.0f, 0.0f, 0.0f }, .velocity = { .2f, .0f, .0f } });
            em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::ATTACK });
        
            att.createAttack =  false;
        }

        att.decreaseCountdown();
    });
}