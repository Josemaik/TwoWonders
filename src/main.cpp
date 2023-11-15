#include <raylib.h>
#include <array>
#include <cstdint>
#include <cstdlib>
#include <cassert>
#include <span>

constexpr u_int16_t SCREEN_WIDTH  { 800 };
constexpr u_int16_t SCREEN_HEIGHT { 600 };

struct Entity {
   uint16_t x{}, y{};
   uint8_t vx{};
};

struct EntityManager {
   static constexpr std::size_t MAX_ENTITIES { 500 };
   
   Entity& newEntity() noexcept {
      assert(alive_ < MAX_ENTITIES);

      alive_ += 1;
      return entities_[alive_ - 1];
   }

   // destructor de las entidades

   // mirar como hacer para actualizar la posicion de las entidades

   std::size_t freeEntities() const noexcept { return MAX_ENTITIES - alive_; }

   std::span<Entity const> getEntities() const { 
      return std::span{ entities_.begin(), alive_ }; 
   }

private:
   std::size_t alive_ {};
   std::array<Entity, MAX_ENTITIES> entities_{};
};

void render_system(EntityManager& EM) {
   BeginDrawing();

   ClearBackground(BLACK);

   for(auto& p : EM.getEntities()) {
      DrawPixel(p.x, p.y, YELLOW);
   }
   
   EndDrawing();
}

void generator_system(EntityManager& EM){
   if(EM.freeEntities() != 0){
      Entity& e = EM.newEntity();
      e.x  = static_cast<uint16_t>(rand()) % SCREEN_WIDTH;
      e.y  = static_cast<uint16_t>(rand()) % SCREEN_HEIGHT;
      e.vx = static_cast<uint8_t>(rand()) % 3 + 1;
   }
}

void physics_system(EntityManager& EM){
   for(auto& p : EM.getEntities()) {
      //p.x = p.x - p.vx;
   }
}

int main() {
   InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Starfield");
   EntityManager  EM{};
 
   // SetTargetFPS(120);

   while ( ! WindowShouldClose() ) {
      generator_system(EM);

      physics_system(EM);
      render_system(EM);
   }
 
   CloseWindow();
}