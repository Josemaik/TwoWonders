#include "object_system.hpp"

void ObjectSystem::update(EntityManager& em, float deltaTime){
    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& ent, ObjectComponent& obj)
    {
        if(obj.decreaseLifeTime(deltaTime)) 
            em.destroyEntity(ent.getID());

        // Recuperamos la entidad del player
        auto& li = em.getSingleton<LevelInfo>();
        auto* playerEnt = em.getEntityByID(li.playerID);

        // Si es player se aplica el efecto del objeto
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

                default:
                    break;
            }
            em.destroyEntity(ent.getID());
        }
    });
}