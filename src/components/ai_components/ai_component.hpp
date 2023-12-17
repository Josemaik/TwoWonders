#pragma once

struct BehaviourTree_t;

struct AIComponent{
    //Enum of types
    enum struct AI_type{Patrol,RandomShoot,ShootPlayer,Diagonal,Drake};
    //Posible bahaviour tree
    BehaviourTree_t* behaviourTree{nullptr};
};