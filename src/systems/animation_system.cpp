#include "animation_system.hpp"

void AnimationSystem::update(EntityManager& em,GameEngine& engine){
    em.forEach<SYSCMPs, SYSTAGs>([&](Entity&,AnimationComponent& ac,RenderComponent& rc){
        if(ac.currentanimation == std::numeric_limits<std::size_t>::max()){
            auto id = engine.PlayAnimation(ac.animations[0]);
            auto& meshes = rc.node->getEntity<DarkMoon::Model>()->getMeshes();
            for(auto& mesh : meshes){
                if(mesh->hasBones)
                    mesh->animID = id;
            }
            ac.currentanimation = 0;
        }
    }); 

    engine.UpdateAnimations();
}