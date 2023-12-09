#include "object_system.hpp"

void ObjectSystem::update(EntityManager& em, float deltaTime){
    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& ent, ObjectComponent& obj)
    {
        if(obj.decreaseLifeTime(deltaTime) && (obj.type != Object_type::Sword)) 
        {
            if(obj.type == Object_type::BombExplode)
                obj.effect();
            else
                em.destroyEntity(ent.getID());
        }

        // Recuperamos la entidad del player
        auto& li = em.getSingleton<LevelInfo>();
        auto* playerEnt = em.getEntityByID(li.playerID);

        // Si existe el player se aplica el efecto del objeto
        if(playerEnt && obj.active){
            switch (obj.type)
            {
                case Object_type::Life:
                    if(playerEnt->hasComponent<LifeComponent>())
                        em.getComponent<LifeComponent>(*playerEnt).increaseLife();
                    break;

                case Object_type::Sword:
                    if(!playerEnt->hasComponent<AttackComponent>())
                        em.addComponent<AttackComponent>(*playerEnt, AttackComponent{});
                    break;

                case Object_type::Bomb:
                    if(playerEnt->hasComponent<InformationComponent>())
                        em.getComponent<InformationComponent>(*playerEnt).addBomb();
                    break;

                case Object_type::Coin:
                    if(playerEnt->hasComponent<InformationComponent>())
                        em.getComponent<InformationComponent>(*playerEnt).addCoin();
                    break;

                case Object_type::BombExplode:
                    explodeBomb(em, ent);
                    break;

                default:
                    break;
            }
            em.destroyEntity(ent.getID());
        }
    });
}

void ObjectSystem::explodeBomb(EntityManager& em, Entity& ent){
    if(ent.hasComponent<RenderComponent>()){
        auto& ren = em.getComponent<RenderComponent>(ent);
        // Crear una entidad que quite vida a los enemigos
        auto& e{ em.newEntity() };
        em.addTag<HitPlayer>(e);
        auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = ren.position, .scale = { 3.0f, 1.0f, 3.0f }, .color = BLACK });
        auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position{ r.position }, .gravity = 0 });
        em.addComponent<LifeComponent>(e, LifeComponent{ .life = 5, .countdown = 0.0f });
        em.addComponent<ProjectileComponent>(e, ProjectileComponent{ .range = 0.2f});
        em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::ATK_PLAYER });

        // Crear una entidad que quite vida al player
        auto& e2{ em.newEntity() };
        em.addTag<HitPlayer>(e);
        auto& r2 = em.addComponent<RenderComponent>(e2, RenderComponent{ .position = ren.position, .scale = { 3.0f, 1.0f, 3.0f }, .color = BLACK });
        auto& p2 = em.addComponent<PhysicsComponent>(e2, PhysicsComponent{ .position{ r2.position }, .gravity = 0 });
        em.addComponent<LifeComponent>(e2, LifeComponent{ .life = 5 });
        em.addComponent<ProjectileComponent>(e2, ProjectileComponent{ .range = 0.2f});
        em.addComponent<ColliderComponent>(e2, ColliderComponent{ p2.position, r2.scale, BehaviorType::ATK_ENEMY });
    }
    
    // else
    //     em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::ATK_ENEMY });
}