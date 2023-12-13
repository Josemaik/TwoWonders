#include "map.hpp"

void Map::createMap(EntityManager& em){
    createGroundWaterOverworld(em);
    createWallsOverworld(em);
}

// Se encarga de crear las paredes del OverWorld
void Map::createWallsOverworld(EntityManager& em){

    struct WallData
    {
        vec3f position;
        vec3f scale;
        Color color;
    };

    WallData wallData[] = {
        
        // ZONA 1

        { { -5.5f, 0.0f, -8.0f }, { 8.0f, 1.0f, 3.0f }, LIME },    // |
        { { 5.5f, 0.0f, -8.0f }, { 8.0f, 1.0f, 3.0f }, LIME },     // | Pared Horizontal Arriba

        { { -11.0f, 0.0f, -5.5f }, { 3.0f, 1.0f, 8.0f }, LIME },   // | 
        { { -11.0f, 0.0f, 5.5f }, { 3.0f, 1.0f, 8.0f }, LIME },    // | Pared Vertical Izquierda
        { { 0.0f, 0.0f, 8.0f }, { 19.0f, 1.0f, 3.0f }, LIME },     // | Pared Horizontal Abajo
        { { 11.0f, 0.0f, 0.0f }, { 3.0f, 1.0f, 19.0f }, LIME },    // | Pared Vertical Derecha

        // ZONA 2

        { { -11.0f, 0.0f, -12.0f }, { 3.0f, 1.0f, 5.0f }, LIME },  // | 
        { { -11.0f, 0.0f, -21.5f }, { 3.0f, 1.0f, 8.0f }, LIME },  // | Pared Vertical Izquierda

        { { 11.0f, 0.0f, -17.5f }, { 3.0f, 1.0f, 16.0f }, LIME },  // | Pared Vertical Derecha
        { { 0.0f, 0.0f, -24.0f }, { 19.0f, 1.0f, 3.0f }, LIME },   // | Pared Horizontal Arriba

        { { 7.0f, 0.0f, -20.0f }, { 1.0f, 1.0f, 1.0f }, LIME },    // | 
        { { 7.0f, 0.0f, -16.0f }, { 1.0f, 1.0f, 1.0f }, LIME },    // | Paredes Chiquitas Derecha
        { { 7.0f, 0.0f, -12.0f }, { 1.0f, 1.0f, 1.0f }, LIME },    // | 

        { { -7.0f, 0.0f, -20.0f }, { 1.0f, 1.0f, 1.0f }, LIME },   // | 
        { { -7.0f, 0.0f, -16.0f }, { 1.0f, 1.0f, 1.0f }, LIME },   // | 
        { { -7.0f, 0.0f, -12.0f }, { 1.0f, 1.0f, 1.0f }, LIME },   // | 
        { { -4.0f, 0.0f, -20.0f }, { 1.0f, 1.0f, 1.0f }, LIME },   // | Paredes Chiquitas Izquierda
        { { -4.0f, 0.0f, -16.0f }, { 1.0f, 1.0f, 1.0f }, LIME },   // | 
        { { -4.0f, 0.0f, -12.0f }, { 1.0f, 1.0f, 1.0f }, LIME },   // | 

        // ZONA 3

        { { -22.0f, 0.0f, 8.0f }, { 19.0f, 1.0f, 3.0f }, LIME },   // | Pared Horizontal Abajo
        { { -23.5f, 0.0f, -8.0f }, { 16.0f, 1.0f, 3.0f }, LIME },  // | Pared Horizontal Arriba

        { { -22.0f, 0.0f, -4.0f }, { 1.0f, 1.0f, 1.0f }, LIME },  // | 
        { { -22.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, LIME },   // | 
        { { -22.0f, 0.0f, 4.0f }, { 1.0f, 1.0f, 1.0f }, LIME },   // | 
        { { -19.0f, 0.0f, -2.0f }, { 1.0f, 1.0f, 1.0f }, LIME },  // | Paredes Chiquitas
        { { -19.0f, 0.0f, 2.0f }, { 1.0f, 1.0f, 1.0f }, LIME },   // | 
        { { -25.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, LIME },   // | 
        { { -28.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, LIME },   // | 

        // ZONA 4

        { { -22.0f, 0.0f, -24.0f }, { 19.0f, 1.0f, 3.0f }, LIME },  // | Pared Horizontal Arriba

        { { -20.0f, 0.0f, -18.0f }, { 3.0f, 1.0f, 1.0f }, LIME },   // | 
        { { -20.0f, 0.0f, -14.0f }, { 3.0f, 1.0f, 1.0f }, LIME },   // | 
        { { -25.0f, 0.0f, -20.0f }, { 4.0f, 1.0f, 1.0f }, LIME },   // | Paredes Chiquitas
        { { -25.0f, 0.0f, -16.0f }, { 4.0f, 1.0f, 1.0f }, LIME },   // | 
        { { -25.0f, 0.0f, -12.0f }, { 4.0f, 1.0f, 1.0f }, LIME },   // | 

        // ZONA 5

        { { -37.0f, 0.0f, 8.0f }, { 11.0f, 1.0f, 3.0f }, LIME },   // | 
        { { -50.0f, 0.0f, 8.0f }, { 5.0f, 1.0f, 3.0f }, LIME },    // | Pared Horizontal Abajo
        { { -51.5f, 0.0f, -8.0f }, { 2.0f, 1.0f, 3.0f }, LIME },   // | 
        { { -37.0f, 0.0f, -8.0f }, { 11.0f, 1.0f, 3.0f }, LIME },  // | Pared Horizontal Arriba

        { { -40.5f, 0.0f, -4.0f }, { 1.0f, 1.0f, 1.0f }, LIME },   // | 
        { { -40.5f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, LIME },    // | 
        { { -40.5f, 0.0f, 4.0f }, { 1.0f, 1.0f, 1.0f }, LIME },    // | 
        { { -37.5f, 0.0f, -2.0f }, { 1.0f, 1.0f, 1.0f }, LIME },   // | Paredes Chiquitas
        { { -37.5f, 0.0f, 2.0f }, { 1.0f, 1.0f, 1.0f }, LIME },    // | 
        { { -34.5f, 0.0f, -4.0f }, { 1.0f, 1.0f, 1.0f }, LIME },   // | 
        { { -34.5f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, LIME },    // | 
        { { -34.5f, 0.0f, 4.0f }, { 1.0f, 1.0f, 1.0f }, LIME },    // | 
        { { -50.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, LIME },    // | 

        { { -54.0f, 0.0f, 0.0f }, { 3.0f, 1.0f, 19.0f }, LIME },   // | Pared Vertical Izquierda


        // ZONA 6

        { { -37.0f, 0.0f, -24.0f }, { 11.0f, 1.0f, 3.0f }, LIME },   // | 
        { { -50.0f, 0.0f, -24.0f }, { 5.0f, 1.0f, 3.0f }, LIME },    // | Pared Horizontal Arriba

        { { -54.0f, 0.0f, -17.5f }, { 3.0f, 1.0f, 16.0f }, LIME },   // | Pared Vertical Izquierda

        { { -40.5f, 0.0f, -20.0f }, { 1.0f, 1.0f, 1.0f }, LIME },   // | 
        { { -40.5f, 0.0f, -16.0f }, { 1.0f, 1.0f, 1.0f }, LIME },   // | 
        { { -40.5f, 0.0f, -12.0f }, { 1.0f, 1.0f, 1.0f }, LIME },   // | Paredes Chiquitas
        { { -37.5f, 0.0f, -18.0f }, { 1.0f, 1.0f, 1.0f }, LIME },   // | 
        { { -37.5f, 0.0f, -14.0f }, { 1.0f, 1.0f, 1.0f }, LIME },   // | 
    };

    for (const auto& data : wallData)
    {
        auto& wall{ em.newEntity() };
        auto& wr = em.addComponent<RenderComponent>(wall, RenderComponent{ .position = data.position, .scale = data.scale, .color = data.color });
        auto& wp = em.addComponent<PhysicsComponent>(wall, PhysicsComponent{ .position = { wr.position }, .velocity = { .0f, .0f, .0f } });
        em.addComponent<ColliderComponent>(wall, ColliderComponent{ wp.position, wr.scale, BehaviorType::STATIC });
    }
}

// Se encarga de crear el suelo del OverWorld
void Map::createGroundWaterOverworld(EntityManager& em){

    struct EntityData
    {
        vec3f position;
        vec3f scale;
        Color color;
    };

    EntityData entitiesG[] = {
        { { 0.f, -1.5f, 0.f }, { 85.0f, 2.f, 100.f }, BEIGE },
        { { -52.5f, -1.5f, 0.f }, { 10.0f, 2.f, 100.f }, BEIGE },
        { { -45.f, -1.5f, -16.f }, { 5.0f, 2.f, 2.0f }, ORANGE },
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
    { { -45.f, -1.5f, -32.0f }, { 5.0f, 2.f, 30.f }, SKYBLUE },
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