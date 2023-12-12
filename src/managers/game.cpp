#include "game.hpp"
#include "game_engine.hpp"
//#include "../utils/memory_viewer.hpp"

void createWallsZelda(EntityManager& em) 
{
    std::vector<std::pair<vec3f, vec3f>> wallData = {

        // ZONA 1

        { { -5.5f, 0.0f, -8.0f }, { 8.0f, 1.0f, 3.0f } },    // |
        { { 5.5f, 0.0f, -8.0f }, { 8.0f, 1.0f, 3.0f } },     // | Pared Horizontal Arriba

        { { -11.0f, 0.0f, -5.5f }, { 3.0f, 1.0f, 8.0f } },   // | 
        { { -11.0f, 0.0f, 5.5f }, { 3.0f, 1.0f, 8.0f } },    // | Pared Vertical Izquierda
        
        { { 0.0f, 0.0f, 8.0f }, { 19.0f, 1.0f, 3.0f } },     // | Pared Horizontal Abajo
        { { 11.0f, 0.0f, 0.0f }, { 3.0f, 1.0f, 19.0f } },    // | Pared Vertical Derecha

        // ZONA 2

        { { -11.0f, 0.0f, -12.0f }, { 3.0f, 1.0f, 5.0f } },  // | 
        { { -11.0f, 0.0f, -21.5f }, { 3.0f, 1.0f, 8.0f } },  // | Pared Vertical Izquierda

        { { 11.0f, 0.0f, -17.5f }, { 3.0f, 1.0f, 16.0f } },  // | Pared Vertical Derecha
        { { 0.0f, 0.0f, -24.0f }, { 19.0f, 1.0f, 3.0f } },   // | Pared Horizontal Arriba

        { { 7.0f, 0.0f, -20.0f }, { 1.0f, 1.0f, 1.0f } },    // | 
        { { 7.0f, 0.0f, -16.0f }, { 1.0f, 1.0f, 1.0f } },    // | Paredes Chiquitas Derecha
        { { 7.0f, 0.0f, -12.0f }, { 1.0f, 1.0f, 1.0f } },    // | 

        { { -7.0f, 0.0f, -20.0f }, { 1.0f, 1.0f, 1.0f } },   // | 
        { { -7.0f, 0.0f, -16.0f }, { 1.0f, 1.0f, 1.0f } },   // | 
        { { -7.0f, 0.0f, -12.0f }, { 1.0f, 1.0f, 1.0f } },   // | 
        { { -4.0f, 0.0f, -20.0f }, { 1.0f, 1.0f, 1.0f } },   // | Paredes Chiquitas Izquierda
        { { -4.0f, 0.0f, -16.0f }, { 1.0f, 1.0f, 1.0f } },   // | 
        { { -4.0f, 0.0f, -12.0f }, { 1.0f, 1.0f, 1.0f } },   // | 

        // ZONA 3

        { { -22.0f, 0.0f, 8.0f }, { 19.0f, 1.0f, 3.0f } },   // | Pared Horizontal Abajo
        { { -23.5f, 0.0f, -8.0f }, { 16.0f, 1.0f, 3.0f } },  // | Pared Horizontal Arriba

        { { -22.0f, 0.0f, -4.0f }, { 1.0f, 1.0f, 1.0f } },  // | 
        { { -22.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f } },   // | 
        { { -22.0f, 0.0f, 4.0f }, { 1.0f, 1.0f, 1.0f } },   // | 
        { { -19.0f, 0.0f, -2.0f }, { 1.0f, 1.0f, 1.0f } },  // | Paredes Chiquitas
        { { -19.0f, 0.0f, 2.0f }, { 1.0f, 1.0f, 1.0f } },   // | 
        { { -25.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f } },   // | 
        { { -28.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f } },   // | 

        // ZONA 4

        { { -22.0f, 0.0f, -24.0f }, { 19.0f, 1.0f, 3.0f } },  // | Pared Horizontal Arriba

        { { -20.0f, 0.0f, -18.0f }, { 3.0f, 1.0f, 1.0f } },   // | 
        { { -20.0f, 0.0f, -14.0f }, { 3.0f, 1.0f, 1.0f } },   // | 
        { { -25.0f, 0.0f, -20.0f }, { 4.0f, 1.0f, 1.0f } },   // | Paredes Chiquitas
        { { -25.0f, 0.0f, -16.0f }, { 4.0f, 1.0f, 1.0f } },   // | 
        { { -25.0f, 0.0f, -12.0f }, { 4.0f, 1.0f, 1.0f } },   // | 

        // ZONA 5

        { { -37.0f, 0.0f, 8.0f }, { 11.0f, 1.0f, 3.0f } },   // | 
        { { -50.0f, 0.0f, 8.0f }, { 5.0f, 1.0f, 3.0f } },    // | Pared Horizontal Abajo
        
        { { -51.5f, 0.0f, -8.0f }, { 2.0f, 1.0f, 3.0f } },   // | 
        { { -37.0f, 0.0f, -8.0f }, { 11.0f, 1.0f, 3.0f } },  // | Pared Horizontal Arriba

        { { -40.5f, 0.0f, -4.0f }, { 1.0f, 1.0f, 1.0f } },   // | 
        { { -40.5f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f } },    // | 
        { { -40.5f, 0.0f, 4.0f }, { 1.0f, 1.0f, 1.0f } },    // | 
        { { -37.5f, 0.0f, -2.0f }, { 1.0f, 1.0f, 1.0f } },   // | Paredes Chiquitas
        { { -37.5f, 0.0f, 2.0f }, { 1.0f, 1.0f, 1.0f } },    // | 
        { { -34.5f, 0.0f, -4.0f }, { 1.0f, 1.0f, 1.0f } },   // | 
        { { -34.5f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f } },    // | 
        { { -34.5f, 0.0f, 4.0f }, { 1.0f, 1.0f, 1.0f } },    // | 
        { { -50.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f } },    // | 

        { { -54.0f, 0.0f, 0.0f }, { 3.0f, 1.0f, 19.0f } },   // | Pared Vertical Izquierda


        // ZONA 6

        { { -37.0f, 0.0f, -24.0f }, { 11.0f, 1.0f, 3.0f } },   // | 
        { { -50.0f, 0.0f, -24.0f }, { 5.0f, 1.0f, 3.0f } },    // | Pared Horizontal Arriba

        { { -54.0f, 0.0f, -17.5f }, { 3.0f, 1.0f, 16.0f } },   // | Pared Vertical Izquierda

        { { -40.5f, 0.0f, -20.0f }, { 1.0f, 1.0f, 1.0f } },   // | 
        { { -40.5f, 0.0f, -16.0f }, { 1.0f, 1.0f, 1.0f } },   // | 
        { { -40.5f, 0.0f, -12.0f }, { 1.0f, 1.0f, 1.0f } },   // | Paredes Chiquitas
        { { -37.5f, 0.0f, -18.0f }, { 1.0f, 1.0f, 1.0f } },   // | 
        { { -37.5f, 0.0f, -14.0f }, { 1.0f, 1.0f, 1.0f } },   // | 



    };

    for (const auto& [pos, scl] : wallData)
    {
        auto& wall{ em.newEntity() };
        auto& wr = em.addComponent<RenderComponent>(wall, RenderComponent{ .position = pos, .scale = scl, .color = GRAY });
        auto& wp = em.addComponent<PhysicsComponent>(wall, PhysicsComponent{ .position = { wr.position }, .velocity = { .0f, .0f, .0f } });
        em.addComponent<ColliderComponent>(wall, ColliderComponent{ wp.position, wr.scale, BehaviorType::STATIC });
    }
}

void createGroundWaterZelda(EntityManager& em){
    struct EntityData
    {
        vec3f position;
        vec3f scale;
        Color color;
    };

    EntityData entitiesG[] = {
        { { 0.f, -1.5f, 0.f }, { 85.0f, 2.f, 100.f }, GREEN },
        { { -52.5f, -1.5f, 0.f }, { 10.0f, 2.f, 100.f }, GREEN },
        { { -45.f, -1.5f, -16.f }, { 5.0f, 2.f, 2.0f }, ORANGE },
        //{ { 30.0f, -1.5f, 0.0f }, { 25.0f, 2.f, 30.0f }, GREEN },
    };

    for (const auto& data : entitiesG)
    {
        auto& entity = em.newEntity();
        em.addTag<GroundTag>(entity);
        auto& renderComponent = em.addComponent<RenderComponent>(entity, RenderComponent{ .position = data.position, .scale = data.scale, .color = data.color });
        auto& physicsComponent = em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = renderComponent.position, .velocity = { .0f, .0f, .0f }, .gravity = .0f });
        em.addComponent<ColliderComponent>(entity, ColliderComponent{ physicsComponent.position, renderComponent.scale, BehaviorType::STATIC });
    }

    EntityData entitiesW[] = {
    { { -45.f, -1.5f, -0.0f }, { 5.0f, 2.f, 30.f }, SKYBLUE },
    { { -45.f, -1.5f, -30.0f }, { 5.0f, 2.f, 30.f }, SKYBLUE },
    //{ { 15.f, -1.5f, 8.75f }, { 5.0f, 2.f, 12.5f }, SKYBLUE },
    //{ { 15.f, -1.5f, 17.5f }, { 55.0f, 2.f, 5.f }, SKYBLUE },
    //{ { 15.f, -1.5f, -17.5f }, { 55.0f, 2.f, 5.f }, SKYBLUE },
    //{ { -15.f, -1.5f, 0.f }, { 5.0f, 2.f, 40.f }, SKYBLUE },
    //{ { 45.f, -1.5f, 0.f }, { 5.0f, 2.f, 40.f }, SKYBLUE }
    };

    for (const auto& data : entitiesW)
    {
        auto& entity = em.newEntity();
        em.addTag<WaterTag>(entity);
        auto& renderComponent = em.addComponent<RenderComponent>(entity, RenderComponent{ .position = data.position, .scale = data.scale, .color = data.color });
        auto& physicsComponent = em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = renderComponent.position, .velocity = { .0f, .0f, .0f }, .gravity = .0f });
        em.addComponent<ColliderComponent>(entity, ColliderComponent{ physicsComponent.position, renderComponent.scale, BehaviorType::STATIC });
    }
} 

void createWalls(EntityManager& em)
{
    std::vector<std::pair<vec3f, vec3f>> wallData = {
        { { 38.5f, 0.0f, 6.0f }, { 1.0f, 1.0f, 7.0f } },  // |
        { { 21.5f, 0.0f, 6.0f }, { 1.0f, 1.0f, 7.0f } },  // |
        { { 38.5f, 0.0f, -6.0f }, { 1.0f, 1.0f, 7.0f } }, // |
        { { 21.5f, 0.0f, -6.0f }, { 1.0f, 1.0f, 7.0f } }, // | - Paredes
        { { 24.5f, 0.0f, -9.5f }, { 7.0f, 1.0f, 1.0f } }, // | - Exteriores
        { { 24.5f, 0.0f, 9.5f }, { 7.0f, 1.0f, 1.0f } },  // |
        { { 35.5f, 0.0f, -9.5f }, { 7.0f, 1.0, 1.0f } },  // |
        { { 35.5f, 0.0f, 9.5f }, { 7.0f, 1.0f, 1.0f } },  // |

        { { 5.f, 0.0f, 5.f }, { 3.0f, 1.0f, 3.0f } },     // |
        { { 5.f, 0.0f, -5.f }, { 3.0f, 1.0f, 3.0f } },    // |
        { { -5.f, 0.0f, 5.f }, { 3.0f, 1.0f, 3.0f } },    // |
        { { -5.f, 0.0f, -5.f }, { 3.0f, 1.0f, 3.0f } },   // | - Cuadrados
        { { 5.f, 0.0f, 0.f }, { 3.0f, 1.0f, 3.0f } },     // | - Interiores
        { { -5.f, 0.0f, 0.f }, { 3.0f, 1.0f, 3.0f } },    // |
        { { 0.f, 0.0f, -5.f }, { 3.0f, 1.0f, 3.0f } },    // |
        { { 0.f, 0.0f, 5.f }, { 3.0f, 1.0f, 3.0f } },     // |
    };

    for (const auto& [pos, scl] : wallData)
    {
        auto& wall{ em.newEntity() };
        auto& wr = em.addComponent<RenderComponent>(wall, RenderComponent{ .position = pos, .scale = scl, .color = GRAY });
        auto& wp = em.addComponent<PhysicsComponent>(wall, PhysicsComponent{ .position = { wr.position }, .velocity = { .0f, .0f, .0f } });
        em.addComponent<ColliderComponent>(wall, ColliderComponent{ wp.position, wr.scale, BehaviorType::STATIC });
    }
}
struct EnemyData {
    AIComponent::AI_type aiType;
    vec3f position;
    vec3f velocity;
    std::array<vec3f, 10> route;
    float stop{};
    float detect_radius;
    int num_lifes;
    float Xmin{},Xmax{},Zmin{},Zmax{};
    bool visible{};
};
void createEnemies(EntityManager& em)
{
    std::vector<EnemyData> enemyData = {
       {  AIComponent::AI_type::PatrolEnemy,
         {0.0f, 0.0f, -8.0f},vec3f{},
         {
             vec3f{0.f, 0.f, -8.0f},
             { -8.5f, 0.f, -8.0f },
             { -8.5f, 0.f, 8.0f },
             { 0.f, 0.f, 8.0f },
             { -8.5f, 0.f, 8.0f },
             { -8.5f, 0.f, -8.0f },
             AIComponent::invalid
         },0.0f,5.0f,1,0.0f,0.0f,0.0f,0.0f,true},
       {  AIComponent::AI_type::PatrolEnemy,
         {0.0f, 0.0f, 8.0f},vec3f{},
         {
             vec3f{0.f, 0.f, 8.0f},
             { 8.5f, 0.f, 8.0f },
             { 8.5f, 0.f, -8.0f },
             { 0.f, 0.f, -8.0f },
             { 8.5f, 0.f, -8.0f },
             { 8.5f, 0.f, 8.0f },
             AIComponent::invalid
         },0.0f,5.0f,1,0.0f,0.0f,0.0f,0.0f,true},
         {  AIComponent::AI_type::PatrolFollowEnemy,
         {-2.0f, 0.0f, 10.0f},vec3f{},
         {
             vec3f{0.0f, 0.f, 13.0f},
             { 2.0f, 0.f, 10.0f },
             { -2.0f, 0.0f, 10.0f },
             AIComponent::invalid
         },0.0f,5.0f,1,0.0f,0.0f,0.0f,0.0f,true},
         {  AIComponent::AI_type::PatrolFollowEnemy,
         {-2.0f, 0.f, -13.0f},vec3f{},
         {
             vec3f{-1.0f, 0.f, -9.0f},
             { 2.0f, 0.f, -12.0f },
             {-2.0f, 0.f, -13.0f },
             AIComponent::invalid
         },0.0f,5.0f,1,0.0f,0.0f,0.0f,0.0f,true},
    //      { AIComponent::AI_type::ShoterEnemy,
    //      {25.0f, 0.0f, -7.0f},
    //      {
    //          vec3f{25.0f, 0.0f, -2.0f},
    //          {37.0f, 0.0f, -2.0f},
    //           {37.0f, 0.0f, -7.0f},
    //           {25.0f, 0.0f, -7.0f},
    //          AIComponent::invalid
    //      },5.0f,1},
        //  { AIComponent::AI_type::ShoterEnemy,
        //  {31.0f, 0.0f, 1.0f},
        //  {
        //      vec3f{31.0f, 0.0f, 6.0f},
        //      {24.0f, 0.0f, 6.0f},
        //       {24.0f, 0.0f, 1.0f},
        //       {31.0f, 0.0f, 1.0f},
        //      AIComponent::invalid
        //  },5.0f,1}
        //  { AIComponent::AI_type::ShoterEnemy,
        //  {35.0f, 0.0f, 0.0f},
        //  {
        //      vec3f{33.0f, 0.0f, 3.0f},
        //      {35.0f, 0.0f, 7.0f},
        //       {37.0f, 0.0f, 4.0f},
        //       {35.0f, 0.0f, 0.0f},
        //      AIComponent::invalid
        //  },5.0f,1},
        // { AIComponent::AI_type::ShoterEnemy, //**********
        //  {30.0f, 0.0f, 2.0f},
        //  {},5.0f,1}
         { AIComponent::AI_type::ShoterEnemy2,
         {15.0f, 0.0f, -4.0f},vec3f{},
         {
             vec3f{13.0f, 0.0f, -8.0f},
             {15.0f, 0.0f, -4.0f},
             AIComponent::invalid
         },0.0f,10.0f,2,13.0f,16.0f,-10.0f,-3.0f,false},
          { AIComponent::AI_type::ShoterEnemy2,
         {16.0f, 0.0f, 4.0f},vec3f{},
         {
             vec3f{13.0f, 0.0f, 10.0f},
             {16.0f, 0.0f, 4.0f},
             AIComponent::invalid
         },0.0f,10.0f,2,13.0f,16.0f,4.0f,10.0f,false},
        {AIComponent::AI_type::RandomEnemy,
        {30.0f, 0.0f, 2.0f},{0.2f,0.0f,0.0f},{},3.5f,0.f,1,0.0f,0.0f,0.0f,0.0f,true},
        {AIComponent::AI_type::RandomEnemy,
        {24.0f, 0.0f, 6.0f},{0.2f,0.0f,0.0f},{},2.0f,0.f,1,0.0f,0.0f,0.0f,0.0f,true},
        {AIComponent::AI_type::RandomEnemy,
        {31.0f, 0.0f, -4.0f},{0.2f,0.0f,0.0f},{},1.0f,0.f,1,0.0f,0.0f,0.0f,0.0f,true}
    };
    //POner por parametro tiempo de culldown para disparar
    for (const auto& data : enemyData)
    {
        auto& enemy{ em.newEntity() };
        em.addTag<EnemyTag>(enemy);
        auto& r = em.addComponent<RenderComponent>(enemy, RenderComponent{ .position = data.position, .scale = { 1.0f, 1.0f, 1.0f }, .color = ORANGE ,.visible=data.visible});
        auto& p = em.addComponent<PhysicsComponent>(enemy, PhysicsComponent{ .position = { r.position }, .velocity = { .2f, .0f, .0f } });
        em.addComponent<AIComponent>(enemy, AIComponent{ .current_type = data.aiType,
        .patrol = data.route,
        .detect_radius = data.detect_radius,
        .elapsed_stop = data.stop,
        .Xmin = data.Xmin,
        .Xmax = data.Xmax,
        .Zmin = data.Zmin,
        .Zmax = data.Zmax
            });
        em.addComponent<LifeComponent>(enemy, LifeComponent{ .life = data.num_lifes });
        em.addComponent<ColliderComponent>(enemy, ColliderComponent{ p.position, r.scale, BehaviorType::ENEMY });
        if (data.aiType == AIComponent::AI_type::ShoterEnemy2) {
            em.addComponent<AttackComponent>(enemy, AttackComponent{ .countdown = 3.0f });
        }
        if(data.aiType == AIComponent::AI_type::RandomEnemy){
             em.addComponent<AttackComponent>(enemy, AttackComponent{ .countdown = 0.0f });
        }
    }
}

void createGroundWater(EntityManager& em)
{
    struct EntityData
    {
        vec3f position;
        vec3f scale;
        Color color;
    };

    EntityData entitiesG[] = {
        { { 0.0f, -1.5f, 0.0f }, { 25.0f, 2.f, 30.0f }, GREEN },
        { { 15.f, -1.5f, 0.f }, { 5.0f, 2.f, 5.f }, GREEN },
        { { 30.0f, -1.5f, 0.0f }, { 25.0f, 2.f, 30.0f }, GREEN },
    };

    for (const auto& data : entitiesG)
    {
        auto& entity = em.newEntity();
        em.addTag<GroundTag>(entity);
        auto& renderComponent = em.addComponent<RenderComponent>(entity, RenderComponent{ .position = data.position, .scale = data.scale, .color = data.color });
        auto& physicsComponent = em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = renderComponent.position, .velocity = { .0f, .0f, .0f }, .gravity = .0f });
        em.addComponent<ColliderComponent>(entity, ColliderComponent{ physicsComponent.position, renderComponent.scale, BehaviorType::STATIC });
    }

    EntityData entitiesW[] = {
    { { 15.f, -1.5f, -8.75f }, { 5.0f, 2.f, 12.5f }, SKYBLUE },
    { { 15.f, -1.5f, 8.75f }, { 5.0f, 2.f, 12.5f }, SKYBLUE },
    { { 15.f, -1.5f, 17.5f }, { 55.0f, 2.f, 5.f }, SKYBLUE },
    { { 15.f, -1.5f, -17.5f }, { 55.0f, 2.f, 5.f }, SKYBLUE },
    { { -15.f, -1.5f, 0.f }, { 5.0f, 2.f, 40.f }, SKYBLUE },
    { { 45.f, -1.5f, 0.f }, { 5.0f, 2.f, 40.f }, SKYBLUE }
    };

    for (const auto& data : entitiesW)
    {
        auto& entity = em.newEntity();
        em.addTag<WaterTag>(entity);
        auto& renderComponent = em.addComponent<RenderComponent>(entity, RenderComponent{ .position = data.position, .scale = data.scale, .color = data.color });
        auto& physicsComponent = em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = renderComponent.position, .velocity = { .0f, .0f, .0f }, .gravity = .0f });
        em.addComponent<ColliderComponent>(entity, ColliderComponent{ physicsComponent.position, renderComponent.scale, BehaviorType::STATIC });
    }
}

void createSword(EntityManager& em)
{
    auto& e{ em.newEntity() };
    em.addTag<Object>(e);
    auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = { -6.0f, 0.f, -5.0f }, .scale = { 1.0f, 0.3f, 0.3f }, .color = LIGHTGRAY });
    auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position = { r.position }, .velocity = { .1f, .0f, .0f } });
    em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::STATIC });
    em.addComponent<ObjectComponent>(e, ObjectComponent{ .type = Object_type::Sword});
}

void createEntities(EntityManager& em)
{
    // Player
    auto& e{ em.newEntity() };
    em.addTag<PlayerTag>(e);
    auto& r = em.addComponent<RenderComponent>(e, RenderComponent{ .position = { -52.0f, 0.f, 0.0f }, .scale = { 1.0f, 1.0f, 1.0f }, .color = PINK });
    auto& p = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position = { r.position }, .velocity = { .1f, .0f, .0f } });
    em.addComponent<InputComponent>(e, InputComponent{});
    em.addComponent<LifeComponent>(e, LifeComponent{ .life = 6 });
    em.addComponent<ColliderComponent>(e, ColliderComponent{ p.position, r.scale, BehaviorType::PLAYER });
    em.addComponent<InformationComponent>(e, InformationComponent{});

    // Sword
    createSword(em);

    // Ground and water (Zelda NES)
    createGroundWaterZelda(em);
    
    // Ground and water
    // createGroundWater(em);

    // Walls (Zelda NES)
    createWallsZelda(em);

    // Walls
    // createWalls(em);

    // Enemies
    // createEnemies(em);

    auto& li = em.getSingleton<LevelInfo>();
    li.playerID = e.getID();
}

void game()
{
    GameEngine engine{ SCREEN_WIDTH, SCREEN_HEIGHT };
    EntityManager em{};
    PhysicsSystem physics_system{};
    RenderSystem render_system{};
    InputSystem input_system{};
    CollisionSystem collision_system{};
    LifeSystem life_system{};
    AISystem   ai_sys{};
    GameTimer gtime{};
    AttackSystem attack_system{};
    ProjectileSystem projectile_system{};
    ObjectSystem object_system{};

    createEntities(em);

    engine.setTargetFPS(30);

    // MemoryViewer mv{ em.getCMPStorage<ColliderComponent>() };
    // mv.printMemory();

    // Inicializa una variabloe donde tener el tiempo entre frames
    float deltaTime;
    while (!engine.windowShouldClose())
    {
        deltaTime = engine.getFrameTime();

        input_system.update(em);
        ai_sys.update(em, deltaTime);
        physics_system.update(em);
        collision_system.update(em);

        object_system.update(em, deltaTime);
        attack_system.update(em, deltaTime);
        projectile_system.update(em, deltaTime);
        life_system.update(em, deltaTime);

        if (!render_system.update(em, engine))
            createEntities(em);

    }

    engine.closeWindow();

}