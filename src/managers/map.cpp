#include "map.hpp"

void Map::createMap(EntityManager& em) {
    createGroundWaterOverworld(em);
    createWallsOverworld(em);
    createZonesOverworld(em);
    createRamps(em);
}

// Se encarga de crear las paredes del OverWorld
void Map::createWallsOverworld(EntityManager& em) {

    WallData wallData[] = {

        // ZONA 1

        { { -5.5, 0.0, -8.0 }, { 8.0, 1.0, 3.0 }, LIME },    // |
        { { 5.5, 0.0, -8.0 }, { 8.0, 1.0, 3.0 }, LIME },     // | Pared Horizontal Arriba

        { { -11.0, 0.0, -5.5 }, { 3.0, 1.5, 8.0 }, LIME },   // |
        { { -11.0, 0.0, 5.5 }, { 3.0, 1.0, 8.0}, LIME },    // | Pared Vertical Izquierda
        { { 0.0, 0.0, 8.0 }, { 19.0, 1.0, 3.0 }, LIME },     // | Pared Horizontal Abajo
        { { 11.0, 0.0, 0.0 }, { 3.0, 1.0, 19.0 }, LIME },    // | Pared Vertical Derecha

        // ZONA 2

        { { -10.9, 0.0, -12.0 }, { 3.0, 2.0, 5.0 }, LIME },  // |
        { { -10.95, 0.0, -21.6 }, { 3.0, 2.0, 8.0 }, LIME },  // | Pared Vertical Izquierda

        { { 11.1, 0.0, -17.5 }, { 3.0, 1.0, 16.0 }, LIME },  // | Pared Vertical Derecha
        { { 0.01, 0.0, -24.0 }, { 19.0, 1.0, 3.0 }, LIME },   // | Pared Horizontal Arriba

        // { { 7.0, 0.0, -20.0 }, { 1.0, 1.0, 1.0 }, LIME },    // |
        // { { 7.0, 0.0, -16.0 }, { 1.0, 1.0, 1.0 }, LIME },    // | Paredes Chiquitas Derecha
        // { { 7.0, 0.0, -12.0 }, { 1.0, 1.0, 1.0 }, LIME },    // |

        // { { -7.0, 0.0, -20.0 }, { 1.0, 1.0, 1.0 }, LIME },   // |
        // { { -7.0, 0.0, -16.0 }, { 1.0, 1.0, 1.0 }, LIME },   // |
        // { { -7.0, 0.0, -12.0 }, { 1.0, 1.0, 1.0 }, LIME },   // |
        // { { -4.0, 0.0, -20.0 }, { 1.0, 1.0, 1.0 }, LIME },   // | Paredes Chiquitas Izquierda
        // { { -4.0, 0.0, -16.0 }, { 1.0, 1.0, 1.0 }, LIME },   // |
        // { { -4.0, 0.0, -12.0 }, { 1.0, 1.0, 1.0 }, LIME },   // |

        // ZONA 3

        { { -22.0, 0.0, 8.0 }, { 19.0, 1.0, 3.0 }, LIME },   // | Pared Horizontal Abajo
        { { -23.5, 0.0, -7.99 }, { 16.0, 2.0, 3.0 }, LIME },  // | Pared Horizontal Arriba

        { { -22.0, 0.0, -4.0 }, { 1.0, 1.0, 1.0 }, LIME },  // |
        { { -22.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 }, LIME },   // |
        { { -22.0, 0.0, 4.0 }, { 1.0, 1.0, 1.0 }, LIME },   // |
        { { -19.0, 0.0, -2.0 }, { 1.0, 1.0, 1.0 }, LIME },  // | Paredes Chiquitas
        { { -19.0, 0.0, 2.0 }, { 1.0, 1.0, 1.0 }, LIME },   // |
        { { -25.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 }, LIME },   // |
        { { -28.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 }, LIME },   // |

        // ZONA 4

        { { -22.0, 0.0, -24.1 }, { 19.0, 2.0, 3.0 }, LIME },  // | Pared Horizontal Arriba

        { { -20.0, 1.0, -18.0 }, { 3.0, 1.0, 1.0 }, DARKBROWN },   // |
        { { -20.0, 1.0, -14.0 }, { 3.0, 1.0, 1.0 }, DARKBROWN },   // |
        { { -25.0, 1.0, -20.0 }, { 4.0, 1.0, 1.0 }, DARKBROWN },   // | Paredes Chiquitas
        { { -25.0, 1.0, -16.0 }, { 4.0, 1.0, 1.0 }, DARKBROWN },   // |
        { { -25.0, 1.0, -12.0 }, { 4.0, 1.0, 1.0 }, DARKBROWN },   // |

        // ZONA 5

        { { -37.0, 0.0, 8.0 }, { 11.0, 1.0, 3.0 }, LIME },   // |
        { { -50.0, 0.0, 8.0 }, { 5.0, 1.0, 3.0 }, LIME },    // | Pared Horizontal Abajo
        { { -51.5, 0.0, -7.99 }, { 2.0, 2.0, 3.0 }, LIME },  // |
        { { -37.0, 0.0, -8.0 }, { 11.0, 2.0, 3.0 }, LIME },  // | Pared Horizontal Arriba

        { { -40.5, 0.0, -4.0 }, { 1.0, 1.0, 1.0 }, LIME },   // |
        { { -40.5, 0.0, 0.0 }, { 1.0, 1.0, 1.0 }, LIME },    // |
        { { -40.5, 0.0, 4.0 }, { 1.0, 1.0, 1.0 }, LIME },    // |
        { { -37.5, 0.0, -2.0 }, { 1.0, 1.0, 1.0 }, LIME },   // | Paredes Chiquitas
        { { -37.5, 0.0, 2.0 }, { 1.0, 1.0, 1.0 }, LIME },    // |
        { { -34.5, 0.0, -4.0 }, { 1.0, 1.0, 1.0 }, LIME },   // |
        { { -34.5, 0.0, 0.0 }, { 1.0, 1.0, 1.0 }, LIME },    // |
        { { -34.5, 0.0, 4.0 }, { 1.0, 1.0, 1.0 }, LIME },    // |
        { { -50.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 }, LIME },    // |
        { { -54.0, 0.0, 0.0 }, { 3.0, 1.0, 19.0 }, LIME },   // | Pared Vertical Izquierda

        // ZONA 6

        { { -37.0, 0.0, -24.1 }, { 11.0, 2.0, 3.0 }, LIME },   // |
        { { -50.0, 0.0, -24.1 }, { 5.0, 2.0, 3.0 }, LIME },    // | Pared Horizontal Arriba

        { { -54.0, 0.0, -17.5 }, { 3.0, 2.0, 16.0 }, LIME },   // | Pared Vertical Izquierda

        { { -40.5, 1.0, -20.0 }, { 1.0, 1.0, 1.0 }, DARKBROWN },   // |
        { { -40.5, 1.0, -16.0 }, { 1.0, 1.0, 1.0 }, DARKBROWN },   // |
        { { -40.5, 1.0, -12.0 }, { 1.0, 1.0, 1.0 }, DARKBROWN },   // | Paredes Chiquitas
        { { -37.5, 1.0, -18.0 }, { 1.0, 1.0, 1.0 }, DARKBROWN },   // |
        { { -37.5, 1.0, -14.0 }, { 1.0, 1.0, 1.0 }, DARKBROWN },   // |

        // CUEVAS EN EL OVERWORLD

        { { -3.5, 0.0, -6.0 }, { 1.0, 1.0, 1.0 }, LIME },   // |
        { { -5.0, 0.0, -6.0 }, { 2.0, 1.0, 1.0 }, BLACK },  // | Cueva Espada
        { { -6.5, 0.0, -6.0 }, { 1.0, 1.0, 1.0 }, LIME },   // |

        { { 1.5, 0.0, -22.0 }, { 1.0, 1.0, 1.0 }, LIME },   // |
        { { 3.0, 0.0, -22.0 }, { 2.0, 1.0, 1.0 }, BLACK },  // | Cueva Monedas
        { { 4.5, 0.0, -22.0 }, { 1.0, 1.0, 1.0 }, LIME },   // |

        { { -15.5, 1.0, -22.0 }, { 1.0, 1.0, 1.0 }, LIME },   // |
        { { -17.0, 1.0, -22.0 }, { 2.0, 1.0, 1.0 }, BLACK },  // | Cueva Tienda
        { { -18.5, 1.0, -22.0 }, { 1.0, 1.0, 1.0 }, LIME },   // |

        { { -52.0, 0.0, -2.0 }, { 1.0, 1.0, 1.0 }, DARKBLUE },  // |
        { { -52.0, 0.0, -3.5 }, { 1.0, 1.0, 2.0 }, BLACK },     // | Cueva Mazmorra
        { { -52.0, 0.0, -5.0 }, { 1.0, 1.0, 1.0 }, DARKBLUE },  // |

        // CUEVAS

        { { 38.0, 0.0, 80.0 }, { 3.0, 1.0, 19.0 }, DARKBROWN },  // | Pared Vertical Izquierda
        { { 60.0, 0.0, 80.0 }, { 3.0, 1.0, 19.0 }, DARKBROWN },  // | Pared Vertical Central - I
        { { 82.0, 0.0, 80.0 }, { 3.0, 1.0, 19.0 }, DARKBROWN },  // | Pared Vertical Central - D
        { { 104.0, 0.0, 80.0 }, { 3.0, 1.0, 19.0 }, DARKBROWN }, // | Pared Vertical Derecha

        { { 49., 0., 72. }, { 19., 1., 3. }, DARKBROWN }, // |
        { { 71., 0., 72. }, { 19., 1., 3. }, DARKBROWN }, // | Paredes Horizontales Arriba
        { { 93., 0., 72. }, { 19., 1., 3. }, DARKBROWN }, // |

        { { 43.5, 0., 88. }, { 8., 1., 3. }, DARKBROWN }, // |
        { { 54.5, 0., 88. }, { 8., 1., 3. }, DARKBROWN }, // |
        { { 65.5, 0., 88. }, { 8., 1., 3. }, DARKBROWN }, // |
        { { 76.5, 0., 88. }, { 8., 1., 3. }, DARKBROWN }, // | Paredes Horizontales Abajo
        { { 87.5, 0., 88. }, { 8., 1., 3. }, DARKBROWN }, // |
        { { 98.5, 0., 88. }, { 8., 1., 3. }, DARKBROWN }, // |

        // MAZMORRA

        { { 50.0, 0.0, -79.0 }, { 3.0, 1.0, 35.0 }, DARKBLUE },  // | Pared Vertical Izquierda
        { { 94.0, 0.0, -79.0 }, { 3.0, 1.0, 35.0 }, DARKBLUE },  // | Pared Vertical Derecha

        { { 72.0, 0.0, -91.0 }, { 3.0, 1.0, 5.0 }, DARKBLUE },  // |
        { { 72.0, 0.0, -81.5 }, { 3.0, 1.0, 8.0 }, DARKBLUE },  // |
        { { 72.0, 0.0, -75.0 }, { 3.0, 1.0, 5.0 }, DARKBLUE },  // | Pared Vertical Enmedio
        { { 72.0, 0.0, -65.5 }, { 3.0, 1.0, 8.0 }, DARKBLUE },  // |

        { { 72.0, 0.0, -95.0 }, { 41.0, 1.0, 3.0 }, DARKBLUE },  // | Pared Horizontal Arriba

        { { 55.5, 0.0, -79.0 }, { 8.0, 1.0, 3.0 }, DARKBLUE },  // |
        { { 66.5, 0.0, -79.0 }, { 8.0, 1.0, 3.0 }, DARKBLUE },  // | Pared Horizontal Enmedio
        { { 83.0, 0.0, -79.0 }, { 19.0, 1.0, 3.0 }, DARKBLUE }, // |

        { { 55.5, 0.0, -63.0 }, { 8.0, 1.0, 3.0 }, DARKBLUE },  // |
        { { 66.5, 0.0, -63.0 }, { 8.0, 1.0, 3.0 }, DARKBLUE },  // | Pared Horizontal Abajo
        { { 83.0, 0.0, -63.0 }, { 19.0, 1.0, 3.0 }, DARKBLUE }, // |

        // ZONA 10

        { { 55.0, 0.0, -75.0 }, { 1.0, 1.0, 1.0 }, DARKBLUE },  // |
        { { 55.0, 0.0, -71.0 }, { 1.0, 1.0, 1.0 }, DARKBLUE },  // |
        { { 55.0, 0.0, -67.0 }, { 1.0, 1.0, 1.0 }, DARKBLUE },  // |
        { { 67.0, 0.0, -75.0 }, { 1.0, 1.0, 1.0 }, DARKBLUE },  // | Paredes Chiquitas
        { { 67.0, 0.0, -71.0 }, { 1.0, 1.0, 1.0 }, DARKBLUE },  // |
        { { 67.0, 0.0, -67.0 }, { 1.0, 1.0, 1.0 }, DARKBLUE },  // |

        // ZONA 12

        { { 78.0, 0.0, -71.0 }, { 2.0, 1.0, 3.0 }, DARKBLUE },  // |
        { { 88.0, 0.0, -71.0 }, { 2.0, 1.0, 3.0 }, DARKBLUE },  // |

    };

    for (const auto& data : wallData)
    {
        auto& wall{ em.newEntity() };
        auto& wr = em.addComponent<RenderComponent>(wall, RenderComponent{ .position = vec3d(data.position), .scale = vec3d(data.scale), .color = data.color });
        auto& wp = em.addComponent<PhysicsComponent>(wall, PhysicsComponent{ .position = vec3d(wr.position), .velocity = { .0, .0, .0 }, .gravity = .0 });
        em.addComponent<ColliderComponent>(wall, ColliderComponent{ wp.position, wr.scale, BehaviorType::STATIC });
    }

    auto& door{ em.newEntity() };
    em.addTag<DoorTag>(door);
    auto& wr = em.addComponent<RenderComponent>(door, RenderComponent{ .position = { 61., 0., -78. }, .scale = { 2., 4., 1. }, .color = BROWN });
    auto& wp = em.addComponent<PhysicsComponent>(door, PhysicsComponent{ .position = { wr.position }, .velocity = { .0, .0, .0 } });
    em.addComponent<ColliderComponent>(door, ColliderComponent{ wp.position, wr.scale, BehaviorType::STATIC });
}

// Se encarga de crear el suelo del OverWorld
void Map::createGroundWaterOverworld(EntityManager& em)
{
    EntityData entitiesG[] = {
        // OverWorld
        { { 0., -1.5, 0. }, { 85.0, 2., 100. }, BEIGE },
        { { -72.5, -1.5, 0. }, { 50.0, 2., 100. }, BEIGE },
        { { -27.5, 0.0, -16.0 }, { 30.0, 1.0, 13.0 }, BEIGE },
        { { -50., 0.0, -16.0 }, { 4.95, 1.0, 13.0 }, BEIGE },

        { { -45., 0.0, -16. }, { 5.0, 1.0, 2.0 }, BROWN },
        // Cuevas
        { { 71., -1.5, 80. }, { 69.0, 2., 19.0 }, BROWN },
        // Mazmorra
        { { 72., -1.5, -79. }, { 47.0, 2., 35.0 }, BLUE },
    };

    for (const auto& data : entitiesG)
    {
        auto& entity = em.newEntity();
        em.addTag<GroundTag>(entity);
        auto& r = em.addComponent<RenderComponent>(entity, RenderComponent{ .position = vec3d(data.position), .scale = vec3d(data.scale),.color = data.color });
        auto& p = em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = vec3d(r.position), .velocity = vec3d::zero(), .gravity = .0 });
        em.addComponent<ColliderComponent>(entity, ColliderComponent{ p.position, r.scale, BehaviorType::STATIC });
    }

    EntityData entitiesW[] = {
    { { -45., -1.5, 13.0 }, { 5.0, 2.0, 45.0 }, SKYBLUE },
    { { -45., 0.0, -10.75 }, { 5.0, 1.0, 8.5 }, SKYBLUE },
    { { -45., 0.0, -32.0 }, { 5.0, 1.0, 30.0 }, SKYBLUE },
    };

    for (const auto& data : entitiesW)
    {
        auto& entity = em.newEntity();
        em.addTag<WaterTag>(entity);
        auto& renderComponent = em.addComponent<RenderComponent>(entity, RenderComponent{ .position = data.position, .scale = data.scale, .color = data.color });
        auto& physicsComponent = em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = renderComponent.position, .velocity = { .0, .0, .0 }, .gravity = .0 });
        em.addComponent<ColliderComponent>(entity, ColliderComponent{ physicsComponent.position, renderComponent.scale, BehaviorType::STATIC });
    }

    // EntityData entitiesRamp[] = {
    //     { { 1.0, -0.5, -5.0 }, { 2.0, 2.0, 2.0 }, WHITE },
    // };

    // for (const auto& data : entitiesRamp)
    // {
    //     auto& entity = em.newEntity();
    //     em.addTag<RampTag>(entity);
    //     auto& renderComponent = em.addComponent<RenderComponent>(entity, RenderComponent{ .position = data.position, .scale = data.scale, .color = data.color });
    //     auto& physicsComponent = em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = renderComponent.position, .velocity = { .0, .0, .0 }, .gravity = .0 });
    //     em.addComponent<ColliderComponent>(entity, ColliderComponent{ physicsComponent.position, renderComponent.scale, BehaviorType::STATIC });
    // }

}

void Map::createRamps(EntityManager& em)
{
    float ori = DEGTORAD * 72.f;

    RampData entitiesR[] = {
        { { -11.4, -1.4, -16.0 }, { 3.0, 3.2, 3.0 }, BEIGE, ori, { 0.0, 0.0, 1.0 } },
        { { -49., -1.4, -8.4 }, { 3.0, 3.2, 3.0 }, BEIGE, -ori, { 1.0, 0.0, 0.0 } },
        { { -14., -1.4, -8.4 }, { 3.0, 3.2, 3.0 }, BEIGE, -ori, { 1.0, 0.0, 0.0 } },
    };

    for (const auto& data : entitiesR)
    {
        auto& entity = em.newEntity();
        em.addTag<RampTag>(entity);
        auto& r = em.addComponent<RenderComponent>(entity, RenderComponent{ .position = data.position, .scale = data.scale, .color = data.color, .rotationVec = data.rotationVec });
        em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = r.position, .velocity = { .0, .0, .0 }, .gravity = .0, .orientation = data.orientation, .rotationVec = data.rotationVec });
    }
}

// Se encarga de crear las zonas
void Map::createZonesOverworld(EntityManager& em) {

    ZoneData entitiesZ[] = {
        // OVERWORLD //
        // Zonas //
        { { 0., 0., 0. }, {19.f, 1.f, 13.f }, 1 },
        { { 0., 0., -16. }, {19.f, 1.f, 13.f }, 2 },
        { { -22., 0., 0. }, {19.f, 1.f, 13.f }, 3 },
        { { -22., 0., -16. }, {19.f, 1.f, 13.f }, 4 },
        { { -43., 0., 0. }, {19.f, 1.f, 13.f }, 5 },
        { { -43., 0., -16. }, {19.f, 1.f, 13.f }, 6 },
        // TP //
        { { -5., 0., -5.5 }, { 2.f, 1.f, 0.5f }, 14 },   // Espada
        { {  3., 0., -21.5 }, { 2.f, 1.f, 0.5f }, 16 },  // Monedas
        { { -17., 1., -21.5 }, { 2.f, 2.f, 0.5f }, 18 }, // Tienda
        // CUEVAS //
        // Zonas //
        { { 49., 0., 80. }, { 19.f, 1.f, 13.f }, 7 },
        { { 71., 0., 80. }, { 19.f, 1.f, 13.f }, 8 },
        { { 93., 0., 80. }, { 19.f, 1.f, 13.f }, 9 },
        // TP //
        { { 49., 0., 88.5 }, { 3.f, 1.f, 2.f }, 15 }, // OverWorld
        { { 71., 0., 88.5 }, { 3.f, 1.f, 2.f }, 17 }, // OverWorld
        { { 93., 0., 88.5 }, { 3.f, 1.f, 2.f }, 19 }, // OverWorld
        // MAZMORRA //
        { { 61., 0.0, -71. }, { 19.0f, 1.f, 13.0f }, 10 },
        { { 61., 0.0, -87. }, { 19.0f, 1.f, 13.0f }, 11 },
        { { 83., 0.0, -71. }, { 19.0f, 1.f, 13.0f }, 12 },
        { { 83., 0.0, -87. }, { 19.0f, 1.f, 13.0f }, 13 },
        // TP //
        { { -51.5, 0., -3.5 }, { 0.5f, 1.f, 2.0f }, 20 }, // Mazmorra
        { { 61.0, 0., -62.5 }, { 3.0f, 1.f, 2.0f }, 21 }, // OverWorld
    };

    for (const auto& data : entitiesZ)
    {
        auto& entity = em.newEntity();
        em.addTag<ZoneTag>(entity);
        em.addComponent<ZoneComponent>(entity, ZoneComponent{ .zone = data.zone });
        auto& r = em.addComponent<RenderComponent>(entity, RenderComponent{ .position = data.position, .scale = data.scale, .visible = false });
        auto& p = em.addComponent<PhysicsComponent>(entity, PhysicsComponent{ .position = r.position, .velocity = vec3d::zero(), .gravity = .0 });
        em.addComponent<ColliderComponent>(entity, ColliderComponent{ p.position, r.scale, BehaviorType::ZONE });
    }
}
