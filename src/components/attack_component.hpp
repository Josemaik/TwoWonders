#pragma once

#include "utils/types.hpp"

struct AttackComponent
{
    // uint8_t damage{};
    // uint8_t type{};    // TypeAttack --> 1=Melee | 2=Distance
    // int range{}, countdown{};

    //void attack(EntityManager& em){
    //    if(countdown < 0){
    //        createBullet(em);
    //        countdown = 30;
    //    }
    //}
//
    //private:
    //    void createBullet(EntityManager& em){
    //        auto& e{ em.newEntity() };
    //        em.addTag<HitPlayer>(e);
    //        auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = { 0.0f, 0.0f, 0.0f }, .scale = { 1.0f, 1.0f, 1.0f }, .color = BLACK });
    //        auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position = { 0.0f, 0.0f, 0.0f }, .velocity = { .1f, .0f, .0f } });
    //        em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::PLAYER });
    //    }
};