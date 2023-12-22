#include "map.hpp"

void Map::createMap(EntityManager& em) {
    createGroundWaterOverworld(em);
    createWallsOverworld(em);

    createZonesOverworld(em);
}

// Se encarga de crear las paredes del OverWorld
void Map::createWallsOverworld(EntityManager& em) {

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

        { { -20.0f, 0.0f, -18.0f }, { 3.0f, 1.0f, 1.0f }, DARKBROWN },   // | 
        { { -20.0f, 0.0f, -14.0f }, { 3.0f, 1.0f, 1.0f }, DARKBROWN },   // | 
        { { -25.0f, 0.0f, -20.0f }, { 4.0f, 1.0f, 1.0f }, DARKBROWN },   // | Paredes Chiquitas
        { { -25.0f, 0.0f, -16.0f }, { 4.0f, 1.0f, 1.0f }, DARKBROWN },   // | 
        { { -25.0f, 0.0f, -12.0f }, { 4.0f, 1.0f, 1.0f }, DARKBROWN },   // | 

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

        { { -40.5f, 0.0f, -20.0f }, { 1.0f, 1.0f, 1.0f }, DARKBROWN },   // | 
        { { -40.5f, 0.0f, -16.0f }, { 1.0f, 1.0f, 1.0f }, DARKBROWN },   // | 
        { { -40.5f, 0.0f, -12.0f }, { 1.0f, 1.0f, 1.0f }, DARKBROWN },   // | Paredes Chiquitas
        { { -37.5f, 0.0f, -18.0f }, { 1.0f, 1.0f, 1.0f }, DARKBROWN },   // | 
        { { -37.5f, 0.0f, -14.0f }, { 1.0f, 1.0f, 1.0f }, DARKBROWN },   // | 

        // CUEVAS EN EL OVERWORLD

        { { -3.5f, 0.0f, -6.0f }, { 1.0f, 1.0f, 1.0f }, LIME },   // | 
        { { -5.0f, 0.0f, -6.0f }, { 2.0f, 1.0f, 1.0f }, BLACK },  // | Cueva Espada
        { { -6.5f, 0.0f, -6.0f }, { 1.0f, 1.0f, 1.0f }, LIME },   // |

        { { 1.5f, 0.0f, -22.0f }, { 1.0f, 1.0f, 1.0f }, LIME },   // | 
        { { 3.0f, 0.0f, -22.0f }, { 2.0f, 1.0f, 1.0f }, BLACK },  // | Cueva Monedas
        { { 4.5f, 0.0f, -22.0f }, { 1.0f, 1.0f, 1.0f }, LIME },   // |  

        { { -15.5f, 0.0f, -22.0f }, { 1.0f, 1.0f, 1.0f }, LIME },   // | 
        { { -17.0f, 0.0f, -22.0f }, { 2.0f, 1.0f, 1.0f }, BLACK },  // | Cueva Tienda
        { { -18.5f, 0.0f, -22.0f }, { 1.0f, 1.0f, 1.0f }, LIME },   // | 

        { { -52.0f, 0.0f, -2.0f }, { 1.0f, 1.0f, 1.0f }, DARKBLUE },  // | 
        { { -52.0f, 0.0f, -3.5f }, { 1.0f, 1.0f, 2.0f }, BLACK },     // | Cueva Mazmorra
        { { -52.0f, 0.0f, -5.0f }, { 1.0f, 1.0f, 1.0f }, DARKBLUE },  // |  

        // CUEVAS

        { { 38.0f, 0.0f, 80.0f }, { 3.0f, 1.0f, 19.0f }, DARKBROWN },  // | Pared Vertical Izquierda
        { { 60.0f, 0.0f, 80.0f }, { 3.0f, 1.0f, 19.0f }, DARKBROWN },  // | Pared Vertical Central - I
        { { 82.0f, 0.0f, 80.0f }, { 3.0f, 1.0f, 19.0f }, DARKBROWN },  // | Pared Vertical Central - D
        { { 104.0f, 0.0f, 80.0f }, { 3.0f, 1.0f, 19.0f }, DARKBROWN }, // | Pared Vertical Derecha

        { { 49.f, 0.f, 72.f }, { 19.f, 1.f, 3.f }, DARKBROWN }, // |
        { { 71.f, 0.f, 72.f }, { 19.f, 1.f, 3.f }, DARKBROWN }, // | Paredes Horizontales Arriba
        { { 93.f, 0.f, 72.f }, { 19.f, 1.f, 3.f }, DARKBROWN }, // |

        { { 43.5f, 0.f, 88.f }, { 8.f, 1.f, 3.f }, DARKBROWN }, // | 
        { { 54.5f, 0.f, 88.f }, { 8.f, 1.f, 3.f }, DARKBROWN }, // | 
        { { 65.5f, 0.f, 88.f }, { 8.f, 1.f, 3.f }, DARKBROWN }, // | 
        { { 76.5f, 0.f, 88.f }, { 8.f, 1.f, 3.f }, DARKBROWN }, // | Paredes Horizontales Abajo
        { { 87.5f, 0.f, 88.f }, { 8.f, 1.f, 3.f }, DARKBROWN }, // | 
        { { 98.5f, 0.f, 88.f }, { 8.f, 1.f, 3.f }, DARKBROWN }, // | 

        // MAZMORRA

        { { 50.0f, 0.0f, -79.0f }, { 3.0f, 1.0f, 35.0f }, DARKBLUE },  // | Pared Vertical Izquierda
        { { 94.0f, 0.0f, -79.0f }, { 3.0f, 1.0f, 35.0f }, DARKBLUE },  // | Pared Vertical Derecha

        { { 72.0f, 0.0f, -91.0f }, { 3.0f, 1.0f, 5.0f }, DARKBLUE },  // | 
        { { 72.0f, 0.0f, -81.5f }, { 3.0f, 1.0f, 8.0f }, DARKBLUE },  // | 
        { { 72.0f, 0.0f, -75.0f }, { 3.0f, 1.0f, 5.0f }, DARKBLUE },  // | Pared Vertical Enmedio
        { { 72.0f, 0.0f, -65.5f }, { 3.0f, 1.0f, 8.0f }, DARKBLUE },  // | 

        { { 72.0f, 0.0f, -95.0f }, { 41.0f, 1.0f, 3.0f }, DARKBLUE },  // | Pared Horizontal Arriba

        { { 55.5f, 0.0f, -79.0f }, { 8.0f, 1.0f, 3.0f }, DARKBLUE },  // | 
        { { 66.5f, 0.0f, -79.0f }, { 8.0f, 1.0f, 3.0f }, DARKBLUE },  // | Pared Horizontal Enmedio
        { { 83.0f, 0.0f, -79.0f }, { 19.0f, 1.0f, 3.0f }, DARKBLUE }, // | 

        { { 55.5f, 0.0f, -63.0f }, { 8.0f, 1.0f, 3.0f }, DARKBLUE },  // | 
        { { 66.5f, 0.0f, -63.0f }, { 8.0f, 1.0f, 3.0f }, DARKBLUE },  // | Pared Horizontal Abajo
        { { 83.0f, 0.0f, -63.0f }, { 19.0f, 1.0f, 3.0f }, DARKBLUE }, // | 

        // ZONA 10

        { { 55.0f, 0.0f, -75.0f }, { 1.0f, 1.0f, 1.0f }, DARKBLUE },  // | 
        { { 55.0f, 0.0f, -71.0f }, { 1.0f, 1.0f, 1.0f }, DARKBLUE },  // | 
        { { 55.0f, 0.0f, -67.0f }, { 1.0f, 1.0f, 1.0f }, DARKBLUE },  // | 
        { { 67.0f, 0.0f, -75.0f }, { 1.0f, 1.0f, 1.0f }, DARKBLUE },  // | Paredes Chiquitas
        { { 67.0f, 0.0f, -71.0f }, { 1.0f, 1.0f, 1.0f }, DARKBLUE },  // | 
        { { 67.0f, 0.0f, -67.0f }, { 1.0f, 1.0f, 1.0f }, DARKBLUE },  // | 

        // ZONA 12

        { { 78.0f, 0.0f, -71.0f }, { 2.0f, 1.0f, 3.0f }, DARKBLUE },  // | 
        { { 88.0f, 0.0f, -71.0f }, { 2.0f, 1.0f, 3.0f }, DARKBLUE },  // | 

    };

    for (const auto& data : wallData)
    {
        auto& wall{ em.newEntity() };
        auto& wr = em.addComponent<RenderComponent>(wall, RenderComponent{ .position = data.position, .scale = data.scale, .color = data.color });
        auto& wp = em.addComponent<PhysicsComponent>(wall, PhysicsComponent{ .position = { wr.position }, .velocity = { .0f, .0f, .0f } });
        em.addComponent<ColliderComponent>(wall, ColliderComponent{ wp.position, wr.scale, BehaviorType::STATIC });
    }

    auto& door{ em.newEntity() };
    em.addTag<DoorTag>(door);
    auto& wr = em.addComponent<RenderComponent>(door, RenderComponent{ .position = { 61.f, 0.f, -78.f }, .scale = { 2.f, 4.f, 1.f }, .color = BROWN });
    auto& wp = em.addComponent<PhysicsComponent>(door, PhysicsComponent{ .position = { wr.position }, .velocity = { .0f, .0f, .0f } });
    em.addComponent<ColliderComponent>(door, ColliderComponent{ wp.position, wr.scale, BehaviorType::STATIC });
}

// Se encarga de crear el suelo del OverWorld
void Map::createGroundWaterOverworld(EntityManager& em) {

    struct EntityData
    {
        vec3f position;
        vec3f scale;
        Color color;
    };

    EntityData entitiesG[] = {
        // OverWorld
        { { 0.f, -1.5f, 0.f }, { 85.0f, 2.f, 100.f }, BEIGE },
        { { -52.5f, -1.5f, 0.f }, { 10.0f, 2.f, 100.f }, BEIGE },
        { { -45.f, -1.5f, -16.f }, { 5.0f, 2.f, 2.0f }, BROWN },
        // Cuevas
        { { 71.f, -1.5f, 80.f }, { 69.0f, 2.f, 19.0f }, BROWN },
        // Mazmorra
        { { 72.f, -1.5f, -79.f }, { 47.0f, 2.f, 35.0f }, BLUE },
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

// Se encarga de crear las zonas
void Map::createZonesOverworld(EntityManager& em) {

    struct EntityData
    {
        vec3f position;
        vec3f scale;
        uint16_t zone;
    };

    EntityData entitiesG[] = {
        // OVERWORLD //
        // Zonas //
        { { 0.f, 0.f, 0.f }, {19.f, 1.f, 13.f }, 1 },
        { { 0.f, 0.f, -16.f }, {19.f, 1.f, 13.f }, 2 },
        { { -22.f, 0.f, 0.f }, {19.f, 1.f, 13.f }, 3 },
        { { -22.f, 0.f, -16.f }, {19.f, 1.f, 13.f }, 4 },
        { { -43.f, 0.f, 0.f }, {19.f, 1.f, 13.f }, 5 },
        { { -43.f, 0.f, -16.f }, {19.f, 1.f, 13.f }, 6 },
        // TP //
        { { -5.f, 0.f, -5.5f }, { 2.f, 1.f, 0.5f }, 14 },   // Espada
        { {  3.f, 0.f, -21.5f }, { 2.f, 1.f, 0.5f }, 16 },  // Monedas
        { { -17.f, 0.f, -21.5f }, { 2.f, 1.f, 0.5f }, 18 }, // Tienda
        // CUEVAS //
        // Zonas //
        { { 49.f, 0.f, 80.f }, { 19.f, 1.f, 13.f }, 7 },
        { { 71.f, 0.f, 80.f }, { 19.f, 1.f, 13.f }, 8 },
        { { 93.f, 0.f, 80.f }, { 19.f, 1.f, 13.f }, 9 },
        // TP //
        { { 49.f, 0.f, 88.5f }, { 3.f, 1.f, 2.f }, 15 }, // OverWorld
        { { 71.f, 0.f, 88.5f }, { 3.f, 1.f, 2.f }, 17 }, // OverWorld
        { { 93.f, 0.f, 88.5f }, { 3.f, 1.f, 2.f }, 19 }, // OverWorld
        // MAZMORRA //
        { { 61.f, 0.0f, -71.f }, { 19.0f, 1.f, 13.0f }, 10 },
        { { 61.f, 0.0f, -87.f }, { 19.0f, 1.f, 13.0f }, 11 },
        { { 83.f, 0.0f, -71.f }, { 19.0f, 1.f, 13.0f }, 12 },
        { { 83.f, 0.0f, -87.f }, { 19.0f, 1.f, 13.0f }, 13 },
        // TP //
        { { -51.5f, 0.f, -3.5f }, { 0.5f, 1.f, 2.0f }, 20 }, // Mazmorra
        { { 61.0f, 0.f, -62.5f }, { 3.0f, 1.f, 2.0f }, 21 }, // OverWorld
    };

    for (const auto& data : entitiesG)
    {
        auto& entity = em.newEntity();
        em.addTag<ZoneTag>(entity);
        em.addComponent<ZoneComponent>(entity, ZoneComponent{ .zone = data.zone });
        auto& r = em.addComponent<RenderComponent>(entity, RenderComponent{ .position = data.position, .scale = data.scale, .visible = false });
        auto& p = em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = r.position, .velocity = vec3f::zero(), .gravity = .0f });
        em.addComponent<ColliderComponent>(entity, ColliderComponent{ p.position, r.scale, BehaviorType::ZONE });
    }
}