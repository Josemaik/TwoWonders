#include "ia_manager.hpp"

struct EnemyData {
            vec3f position;
            vec3f velocity;
            std::array<vec3f, 10> route;
            float stop{};
            float detect_radius;
            int num_lifes;
            float Xmin{},Xmax{},Zmin{},Zmax{};
            bool visible{};
};
//Creamos enemigos dado el type
// 0=>Patrol
// 1=>shoot player
// 2=> ia random
void createEnemiesofType(EntityManager& em, std::vector<EnemyData>vec, uint16_t type, uint16_t z) {
    // int num = 0;
    for (const auto& data : vec)
    {
        // num++;
        auto& enemy{ em.newEntity() };
        em.addTag<EnemyTag>(enemy);
        auto& r = em.addComponent<RenderComponent>(enemy, RenderComponent{ .position = data.position, .scale = { 1.0f, 1.0f, 1.0f }, .color = ORANGE ,.visible = data.visible });
        auto& p = em.addComponent<PhysicsComponent>(enemy, PhysicsComponent{ .position = { r.position }, .velocity = { .2f, .0f, .0f } });
        switch (type)
        {
        case 0: em.addComponent<PatrolComponent>(enemy, PatrolComponent{ .patrol = data.route });
                em.addComponent<ZoneComponent>(enemy,ZoneComponent{.zone = z});
            break;
        case 1:
            em.addComponent<ShootPlayerComponent>(enemy, ShootPlayerComponent{
                .Xmin = data.Xmin,
                .Xmax = data.Xmax,
                .Zmin = data.Zmin,
                .Zmax = data.Zmax
                });
            em.addComponent<AttackComponent>(enemy, AttackComponent{ .countdown = 3.0f });
            em.addComponent<ZoneComponent>(enemy,ZoneComponent{.zone = z});
            break;
        case 2:
            em.addComponent<RandomShootComponent>(enemy, RandomShootComponent{
                .Xmin = data.Xmin,
                .Xmax = data.Xmax,
                .Zmin = data.Zmin,
                .Zmax = data.Zmax
                });
            em.addComponent<AttackComponent>(enemy, AttackComponent{ .countdown = 0.0f });
            em.addComponent<ZoneComponent>(enemy,ZoneComponent{.zone = z});
            break;
        default:
            break;
        }
        em.addComponent<LifeComponent>(enemy, LifeComponent{ .life = data.num_lifes });
        em.addComponent<ColliderComponent>(enemy, ColliderComponent{ p.position, r.scale, BehaviorType::ENEMY });
    }
    // std::printf("Creados: %d\n",num);
}
bool Ia_man::checkEnemiesCreaeted(uint16_t zone){
    switch (zone)
    {
        case 2: return createdzone2; break;
        case 3: return createdzone3; break;
        case 4: return createdzone4; break;
        case 5: return createdzone5; break;
        case 6: return createdzone6; break;
        default:
            break;
    }
    return false;
}
void Ia_man::createEnemiesZone(EntityManager& em,uint16_t zone){
    switch (zone)
    {
    case 2: this->createEnemiesZone2(em,zone); 
        break;
    case 3: this->createEnemiesZone3(em,zone);
        break;
    case 4:this->createEnemiesZone4(em,zone);
        break;
    case 5: this->createEnemiesZone5(em,zone);
        break;
    case 6: this->createEnemiesZone6(em,zone);
        break;
    default:
        break;
    };
}
void Ia_man::createEnemiesZone2(EntityManager& em,uint16_t z){
    createdzone2 = true;
    //Creamos Patrol Enemies
    std::vector<EnemyData> Vec_patrolData = {
       { {-9.0f, 0.f, -14.0f},vec3f{},
        {
            vec3f{-9.0f, 0.f, -14.0f},
            { -9.0f, 0.f, -10.0f },
            { 9.0f, 0.f, -10.0f },
            { 9.0f, 0.f, -14.0f },
            PatrolComponent::invalid
        },0.0f,5.0f,1,0.0f,0.0f,0.0f,0.0f,true}
    };
    createEnemiesofType(em, Vec_patrolData, 0,z);
}
vec3f getRandomPosinRange(float xmin, float xmax, float zmin, float zmax) {
    //Semilla para generar numeros aleatorios
    std::random_device rd;
    std::mt19937 gen(rd());
    // creo rangos
    std::uniform_real_distribution<float> rangoX(xmin, xmax);
    std::uniform_real_distribution<float> rangoZ(zmin, zmax);
    // obtengo x y z aleatoria
    float x = rangoX(gen);
    float z = rangoZ(gen);
    //devuelvo vector
    return vec3f{ x,0.0f,z };

}
void Ia_man::createEnemiesZone3(EntityManager& em,uint16_t z){
    createdzone3 = true;
    //Crearemos IA random
     std::vector<EnemyData> Vec_RandomShoot_3 = {
        {getRandomPosinRange(-31.0f,-13.0f,-6.0f,-3.0f),{0.2f,0.0f,0.0f},{},3.5f,0.f,1,-31.0f,-11.0f,-7.0f,8.0f,true},
        {getRandomPosinRange(-31.0f,-13.0f,-3.0f,-1.0f),{0.2f,0.0f,0.0f},{},3.5f,0.f,1,-31.0f,-11.0f,-7.0f,8.0f,true},
        {getRandomPosinRange(-31.0f,-13.0f,-1.0f,2.0f),{0.2f,0.0f,0.0f},{},3.5f,0.f,1,-31.0f,-11.0f,-7.0f,8.0f,true},
        {getRandomPosinRange(-31.0f,-13.0f,2.0f,4.0f),{0.2f,0.0f,0.0f},{},3.5f,0.f,1,-31.0f,-11.0f,-7.0f,8.0f,true},
        {getRandomPosinRange(-31.0f,-13.0f,4.0f,6.0f),{0.2f,0.0f,0.0f},{},3.5f,0.f,1,-31.0f,-11.0f,-7.0f,8.0f,true}
     };
     createEnemiesofType(em, Vec_RandomShoot_3,2,z);
}
void Ia_man::createEnemiesZone4(EntityManager& em,uint16_t z){
    createdzone4 = true;
    //Creamos Random enemies
    std::vector<EnemyData> Vec_RandomShoot = {
        {getRandomPosinRange(-32.0f,-13.0f,-22.0f,-10.0f),{0.2f,0.0f,0.0f},{},3.5f,0.f,1,-32.0f,-11.0f,-24.0f,-9.0f,true},
        {getRandomPosinRange(-32.0f,-13.0f,-22.0f,-10.0f),{0.2f,0.0f,0.0f},{},2.0f,0.f,1,-32.0f,-11.0f,-24.0f,-9.0f,true},
        {getRandomPosinRange(-32.0f,-13.0f,-22.0f,-10.0f),{0.2f,0.0f,0.0f},{},1.0f,0.f,1,-32.0f,-11.0f,-24.0f,-9.0f,true},
        {getRandomPosinRange(-32.0f,-13.0f,-22.0f,-10.0f),{0.2f,0.0f,0.0f},{},1.0f,0.f,1,-32.0f,-11.0f,-24.0f,-9.0f,true},
        {getRandomPosinRange(-32.0f,-13.0f,-22.0f,-10.0f),{0.2f,0.0f,0.0f},{},1.0f,0.f,1,-32.0f,-11.0f,-24.0f,-9.0f,true}
    };
    createEnemiesofType(em, Vec_RandomShoot, 2,z);
}
void Ia_man::createEnemiesZone5(EntityManager& em,uint16_t z){
    createdzone5 = true;
    std::vector<EnemyData> Vec_ShootPlayerData = {
        { {-45.0f, 0.0f, 4.0f},vec3f{},
         { vec3f{}},0.0f,10.0f,2,-43.0f,-46.0f,3.0f,-4.0f,false
        }
    };
    createEnemiesofType(em, Vec_ShootPlayerData, 1,z);
}
void Ia_man::createEnemiesZone6(EntityManager& em,uint16_t z){
    createdzone6 = true;
    std::vector<EnemyData> Vec_ShootPlayerData = {
        { {-46.0f, 0.0f, -20.0f},vec3f{},
         {vec3f{},},0.0f,10.0f,2,-43.0f,-46.0f,-11.0f,-20.0f,false
        }
    };
    createEnemiesofType(em, Vec_ShootPlayerData, 1,z);
}

void Ia_man::deleteEnemiesZone(EntityManager& em,uint16_t zone){
    switch (zone)
    {
    case 2: this->deleteEnemiesinZone(em,zone); 
        break;
    case 3: this->deleteEnemiesinZone(em,zone);
        break;
    case 4:this->deleteEnemiesinZone(em,zone);
        break;
    case 5: this->deleteEnemiesinZone(em,zone);
        break;
    case 6: this->deleteEnemiesinZone(em,zone);
        break;
    default:
        break;
    };
}
void Ia_man::deleteEnemiesinZone(EntityManager& em,uint16_t z){
    // int xd = 0;
    auto entities = em.getEntities();
    auto entitiesCopy = entities;  // Hacer una copia de la colección

    for (auto &ent : entitiesCopy) {
        if (ent.hasComponent<ZoneComponent>() && ent.hasTag<EnemyTag>()) {
            if (em.getComponent<ZoneComponent>(ent).zone == z) {
                // std::printf("Zona: %d\n",z);
                em.destroyEntity(ent.getID());
                // xd++;
            }
        }
    }

    // std::printf("Eliminados: %d\n", xd);
    setCreatedtofalse(z);
}
void Ia_man::setCreatedtofalse(uint16_t z){
    switch (z)
    {
        case 2: createdzone2=false; break;
        case 3: createdzone3=false; break;
        case 4: createdzone4=false; break;
        case 5: createdzone5=false; break;
        case 6: createdzone6=false; break;
        default:
            break;
    }
}