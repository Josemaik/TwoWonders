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
    float vx{};
};

struct EntityManager
{
    static constexpr std::size_t MAX_ENTITIES{500};

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

    std::size_t freeEntities() const noexcept { return MAX_ENTITIES - alive_; }

    std::span<Entity const> getEntities() const { return std::span{entities_.begin(), alive_}; }

    std::span<Entity> getEntities() { return std::span{entities_.begin(), alive_}; }

private:
    std::size_t alive_{};
    std::array<Entity, MAX_ENTITIES> entities_{};
};

void render_system(EntityManager &EM)
{
    BeginDrawing();

    ClearBackground(BLACK);

    for (const Entity &e : EM.getEntities())
        DrawPixel(static_cast<int>(e.x), static_cast<int>(e.y), YELLOW);

    EndDrawing();
}

void physics_system(EntityManager &em)
{
    auto i{0uz};
    while (i < em.getEntities().size())
    {
        Entity &e = em.getEntities()[i];
        e.x -= e.vx;

        if ((e.x - e.vx) < 0)
        {
            em.destroyEntity(i);
        }
        else
        {
            i += 1;
        }
    }
}

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Zelda");
    EntityManager EM{};

    SetTargetFPS(120);

    Entity &player = EM.newEntity();

    while (!WindowShouldClose())
    {
        physics_system(EM);
        render_system(EM);
    }

    CloseWindow();
}