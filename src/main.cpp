#include <raylib.h>
#include <array>
#include <cstdint>
#include <cstdlib>
#include <cassert>
#include <span>

constexpr u_int16_t SCREEN_WIDTH{800};
constexpr u_int16_t SCREEN_HEIGHT{600};

struct Entity
{
    float x{}, y{};
    uint8_t vx{};
};

struct EntityManager
{
    static constexpr std::size_t MAX_ENTITIES{500};
    std::array<Vector2, 5> positions{{{0, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 0}}};

    Entity &newEntity() noexcept
    {
        assert(alive_ < MAX_ENTITIES);

        alive_ += 1;
        return entities_[alive_ - 1];
    }

    void destroyEntity(size_t index)
    {
        assert(index < alive_);
        assert(alive_ > 0);

        entities_[index] = entities_[alive_ - 1];

        alive_ -= 1;
    }

    void updateVelocity(size_t index)
    {
        assert(index < alive_);
        this->entities_[index].x -= this->entities_[index].vx;
    }

    std::size_t freeEntities() const noexcept { return MAX_ENTITIES - alive_; }

    std::span<Entity const> getEntities() const
    {
        return std::span{entities_.begin(), alive_};
    }

private:
    std::size_t alive_{};
    std::array<Entity, MAX_ENTITIES> entities_{};
};

void render_system(EntityManager &EM)
{
    BeginDrawing();

    ClearBackground(BLACK);

    for (const Entity &e : EM.getEntities())
    {
        for (const Vector2 &pos : EM.positions)
        {
            DrawPixelV({e.x + pos.x, e.y + pos.y}, YELLOW);
        }
    }

    EndDrawing();
}

void generator_system(EntityManager &EM)
{
    if (EM.freeEntities() != 0)
    {
        Entity &e = EM.newEntity();
        e.x = static_cast<float>(SCREEN_WIDTH);
        e.y = static_cast<float>(GetRandomValue(0, SCREEN_HEIGHT));
        e.vx = static_cast<uint8_t>(GetRandomValue(1, 20));
    }
}

void physics_system(EntityManager &em)
{
    auto i{0uz};
    while (i < em.getEntities().size())
    {
        const Entity &e = em.getEntities()[i];

        if ((e.x - e.vx) < 0)
        {
            em.destroyEntity(i);
        }
        else
        {
            em.updateVelocity(i);
            i += 1;
        }
    }
}

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Starfield");
    EntityManager EM{};

    SetTargetFPS(120);

    while (!WindowShouldClose())
    {
        generator_system(EM);

        physics_system(EM);
        render_system(EM);
    }

    CloseWindow();
}