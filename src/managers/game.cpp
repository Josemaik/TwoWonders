#include "game.hpp"

// Estructura de ejemplo para el Slotmap - Borrar si eso
struct jeje
{
    char c[8] = "";
};


void game()
{

    GameEngine engine{ SCREEN_WIDTH, SCREEN_HEIGHT };
    EntityManager<PhysicsComponent, RenderComponent, Entity> em{};
    PhysicsSystem physics_system{};
    RenderSystem render_system{};
    InputSystem input_system{};
    CollisionSystem collision_system{};

    auto& e{ em.newEntity() };

    Cubo c{};
    e.render.setAll({ c.position.x, c.position.y, c.position.z }, { c.scale.x, c.scale.y, c.scale.z }, c.color);

    // Ejemplo Slotmap - Borrar si eso
    Slotmap<jeje, 10, std::uint16_t> physics{};
    using key_t = Slotmap<jeje, 10, std::uint16_t>::key_type;

    std::array<key_t, 10> keys{};

    // Ejemplo Visualizador de Memoria junto Slotmap - Borrar si eso
    MemoryViewer mv{ physics };
    mv.printMemory();
    keys[0] = physics.push_back(jeje{ .c{"##HEJE#"} });
    keys[1] = physics.push_back(jeje{ .c{"##JEJE#"} });
    keys[2] = physics.push_back(jeje{ .c{"##HAHA#"} });
    mv.printMemory();
    physics.erase(keys[0]);
    mv.printMemory();
    physics.erase(keys[1]);
    mv.printMemory();
    jeje j{ .c{"##HOJE#"} };
    keys[0] = physics.push_back(j);
    keys[1] = physics.push_back(jeje{ .c{"##JOJO#"} });
    mv.printMemory();

    for (auto const& item : physics.getData()) {
        std::cout << "Item: " << item.c << std::endl;
    }

    while (engine.run())
    {
        input_system.update(em);
        physics_system.update(em);
        collision_system.update(em);
        render_system.update(em, engine);
    }

    engine.endWindow();

}