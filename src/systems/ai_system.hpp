#pragma once
#ifndef AI_SYSTEM
#define AI_SYSTEM
#include "../utils/types.hpp"
#include "../utils/vec3D.hpp"
#include "../utils/bt/behaviourtree.hpp"
#include <iostream>

constexpr float SPEED_AI = 0.2f;

struct AISystem
{
    // Se van a buscar las entidad que tengan estos componentes y tags
    using SYSCMPs_Patrol = MP::TypeList<PhysicsComponent, PatrolComponent>;
    using SYSCMPs_ShootPlayer = MP::TypeList<PhysicsComponent, ShootPlayerComponent>;
    using SYSCMPs_RandomShoot = MP::TypeList<PhysicsComponent, RandomShootComponent>;
    using SYSCMPs_Diagonal = MP::TypeList<PhysicsComponent, DiagonalComponent>;
    using SYSCMPs_Drake = MP::TypeList<PhysicsComponent, DrakeComponent>;
    using SYSTAGs = MP::TypeList<>;

    void update(EntityManager& em, float dt);
    
    private:
        // AI functions
        template <typename CMP>
        vec3f FollowPatrol(PhysicsComponent& p, CMP& cmp);
        void ShotandMove(ShootPlayerComponent& spc, PhysicsComponent& p, EntityManager& em, Entity& e, float dt);
        void RandomAI(RandomShootComponent& rsc, PhysicsComponent& p, EntityManager& em, Entity& e, float dt);
        void DiagonalAI(DiagonalComponent& dc,PhysicsComponent& p, float dt);
        vec3f DrakeAI(DrakeComponent& dc,PhysicsComponent& p,EntityManager& em, Entity& e,float dt);
        //Operations
        void setVelocity(PhysicsComponent& p, vec3f distance);
        template <typename CMP>
        void setVelocityinRange(PhysicsComponent& p,CMP& cmp);
        vec3f getVelocityonDirecion(vec3f vector);
        vec3f getRandomDir();
        vec3f getRandomDirwithDiagonals();
        vec3f getRandomPosinRange(float xmin, float xmax, float zmin, float zmax);
        bool isInDesiredRange(const vec3f& direction, float xmin, float xmax, float zmin, float zmax);
        //Obtain player data
        [[nodiscard]] bool isPlayerDetected(EntityManager& EM, PhysicsComponent const& p, ShootPlayerComponent const& spc) const noexcept;
        [[nodiscard]] vec3f getPlayerDistance(EntityManager& EM, PhysicsComponent const& p, ShootPlayerComponent& spc) const noexcept;

};

#endif // !AI_SYSTEM