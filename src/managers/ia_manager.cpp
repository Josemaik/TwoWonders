#include "ia_manager.hpp"
////////////////////////////////////////////////////
//Funciones para obtener datos de forma aleatoria///
//////////////////////////////////////////////////////---------------------------------

//DEvuelve un tiempo en segundos random
float getRandomStop() {
    switch (std::rand() % 2) {
    case 0:  return 10.0f; break;
    case 1:  return 6.0f; break;
    case 2:  return 8.0f; break;
    default: return 6.5f; break;
    }
}
//Devuelve una posición random dado un rango
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
//-----------------------------------------------------------------------------------
/////////////////////////////////////////////////////7
///Clase con los Datos necesarios para los enemigos
///////////////////////////////////////////////////////----------------------------
struct EnemyData {
    vec3f position;
    std::array<vec3f, 10> route;
    int num_lifes;
    float Xmin{}, Xmax{}, Zmin{}, Zmax{};
    bool visible{};
    Color color{};
};
//------------------------------------------------------------------------------------------------
// Función que crea enemigos dado un tipo ,vector de datos y zona //
// 0=>Patrol
// 1=>shoot player
// 2=> ia random
// 3=> diagonal
// 4=> drake
void createEnemiesofType(EntityManager& em, std::vector<EnemyData>vec, uint16_t type, uint16_t z) {
    // Obtenemos la clase con información del nivel
    auto& li = em.getSingleton<LevelInfo>();
    // Vacíamos enemigos de la zona anterior del vector
    li.enemiesID.clear();
    //Creamos enemigos
    for (const auto& data : vec)
    {
        auto& enemy{ em.newEntity() };
        em.addTag<EnemyTag>(enemy);
        auto& r = em.addComponent<RenderComponent>(enemy, RenderComponent{ .position = data.position, .scale = { 1.0f, 1.0f, 1.0f }, .color = data.color ,.visible = data.visible });
        auto& p = em.addComponent<PhysicsComponent>(enemy, PhysicsComponent{ .position = { r.position }, .velocity = { .2f, .0f, .0f } });
        em.addComponent<LifeComponent>(enemy, LifeComponent{ .life = data.num_lifes });
        em.addComponent<ColliderComponent>(enemy, ColliderComponent{ p.position, r.scale, BehaviorType::ENEMY });

        switch (type)
        {
        case 0: em.addComponent<PatrolComponent>(enemy, PatrolComponent{ .patrol = data.route });
            break;
        case 1:em.addComponent<ShootPlayerComponent>(enemy, ShootPlayerComponent{ .Xmin = data.Xmin,.Xmax = data.Xmax,.Zmin = data.Zmin,.Zmax = data.Zmax });
            em.addComponent<AttackComponent>(enemy, AttackComponent{ .countdown = 3.0f });
            break;
        case 2:
            em.addComponent<RandomShootComponent>(enemy, RandomShootComponent{ .countdown_stop = getRandomStop(),.Xmin = data.Xmin,.Xmax = data.Xmax,.Zmin = data.Zmin,.Zmax = data.Zmax });
            em.addComponent<AttackComponent>(enemy, AttackComponent{ .countdown = 0.0f });
            break;
        case 3:  em.addComponent<DiagonalComponent>(enemy, DiagonalComponent{ .countdown_stop = getRandomStop(),.Xmin = data.Xmin,.Xmax = data.Xmax,.Zmin = data.Zmin,.Zmax = data.Zmax });
            em.addComponent<ZoneComponent>(enemy, ZoneComponent{ .zone = z });
            break;
        case 4: em.addComponent<DrakeComponent>(enemy, DrakeComponent{ .patrol = data.route });
            em.addComponent<AttackComponent>(enemy, AttackComponent{ .countdown = 0.0f });
            r.scale = { 1.5f,2.0f,1.5f };
            break;
        default:
            break;
        }
        //Insertamos enemigos en el vector
        li.enemiesID.insert(enemy.getID());
    }
}
/////////////////////////////////////////////////////////////
//Funciones para crear los enemigos de cada Zona ////////////
/////////////////////////////////////////////////////////////--------------------------------------------------------
//Creamos los enemigos de la Zona 2
void Ia_man::createEnemiesZone2(EntityManager& em, uint16_t z) {
    createdzone2 = true;
    //Creamos Patrol Enemies
    std::vector<EnemyData> Vec_patrolData = {
       { {-9.0f, 0.f, -14.0f},
        {
            vec3f{-9.0f, 0.f, -14.0f},
            { -9.0f, 0.f, -10.0f },
            { 9.0f, 0.f, -10.0f },
            { 9.0f, 0.f, -14.0f },
            PatrolComponent::invalid
        },1,0.0f,0.0f,0.0f,0.0f,true, ORANGE}
    };
    createEnemiesofType(em, Vec_patrolData, 0, z);
}

void Ia_man::createEnemiesZone3(EntityManager& em, uint16_t z) {
    createdzone3 = true;
    //Crearemos IA random
    std::vector<EnemyData> Vec_RandomShoot_3 = {
       {getRandomPosinRange(-31.0f,-13.0f,-6.0f,-3.0f),{},1,-31.0f,-11.0f,-7.0f,8.0f, true, ORANGE},
       {getRandomPosinRange(-31.0f,-13.0f,-3.0f,-1.0f),{},1,-31.0f,-11.0f,-7.0f,8.0f, true, ORANGE},
       {getRandomPosinRange(-31.0f,-13.0f,-1.0f,2.0f), {},1,-31.0f,-11.0f,-7.0f,8.0f, true, ORANGE},
       {getRandomPosinRange(-31.0f,-13.0f,2.0f,4.0f),  {},1,-31.0f,-11.0f,-7.0f,8.0f, true, ORANGE},
       {getRandomPosinRange(-31.0f,-13.0f,4.0f,6.0f),  {},1,-31.0f,-11.0f,-7.0f,8.0f, true, ORANGE}
    };
    createEnemiesofType(em, Vec_RandomShoot_3, 2, z);
}
void Ia_man::createEnemiesZone4(EntityManager& em, uint16_t z) {
    createdzone4 = true;
    //Creamos Random enemies
    std::vector<EnemyData> Vec_RandomShoot = {
        {getRandomPosinRange(-32.0f,-13.0f,-22.0f,-10.0f),{},1,-32.0f,-11.0f,-24.0f,-9.0f,true, ORANGE},
        {getRandomPosinRange(-32.0f,-13.0f,-22.0f,-10.0f),{},1,-32.0f,-11.0f,-24.0f,-9.0f,true, ORANGE},
        {getRandomPosinRange(-32.0f,-13.0f,-22.0f,-10.0f),{},1,-32.0f,-11.0f,-24.0f,-9.0f,true, ORANGE},
        {getRandomPosinRange(-32.0f,-13.0f,-22.0f,-10.0f),{},1,-32.0f,-11.0f,-24.0f,-9.0f,true, ORANGE},
        {getRandomPosinRange(-32.0f,-13.0f,-22.0f,-10.0f),{},1,-32.0f,-11.0f,-24.0f,-9.0f,true, ORANGE}
    };
    createEnemiesofType(em, Vec_RandomShoot, 2, z);
}
void Ia_man::createEnemiesZone5(EntityManager& em, uint16_t z) {
    createdzone5 = true;
    std::vector<EnemyData> Vec_ShootPlayerData = {
        { {-45.0f, 0.0f, 4.0f},
         { vec3f{}},2,-43.0f,-46.0f,3.0f,-4.0f,false, ORANGE
        }
    };
    createEnemiesofType(em, Vec_ShootPlayerData, 1, z);
}
void Ia_man::createEnemiesZone6(EntityManager& em, uint16_t z) {
    createdzone6 = true;
    std::vector<EnemyData> Vec_ShootPlayerData = {
        { {-46.0f, 0.0f, -20.0f},
         {vec3f{},},2,-43.0f,-46.0f,-11.0f,-20.0f,false, ORANGE
        }
    };
    createEnemiesofType(em, Vec_ShootPlayerData, 1, z);
}
void Ia_man::createEnemiesZone11(EntityManager& em, uint16_t z) {
    createdzone11 = true;
    std::vector<EnemyData> Vec_Drake = {
      { {73.0f, 0.f, -87.0f},
       {
           vec3f{73.0f, 0.f, -87.0f},
           { 69.0f, 0.f, -87.0f },
           DrakeComponent::invalid
       },10,0.0f,0.0f,0.0f,0.0f,true, RED}
    };
    createEnemiesofType(em, Vec_Drake, 4, z);
}
void Ia_man::createEnemiesZone12(EntityManager& em, uint16_t z) {
    createdzone12 = true;
    std::vector<EnemyData> Vec_Diagonal = {
        {getRandomPosinRange(74.0f,92.0f,-77.0f,-65.0f),{},1,74.0f,92.0f,-77.0f,-65.0f,true, PURPLE},
        {getRandomPosinRange(74.0f,92.0f,-77.0f,-65.0f),{},1,74.0f,92.0f,-77.0f,-65.0f,true, PURPLE},
        {getRandomPosinRange(74.0f,92.0f,-77.0f,-65.0f),{},1,74.0f,92.0f,-77.0f,-65.0f,true, PURPLE},
        {getRandomPosinRange(74.0f,92.0f,-77.0f,-65.0f),{},1,74.0f,92.0f,-77.0f,-65.0f,true, PURPLE},
        {getRandomPosinRange(74.0f,92.0f,-77.0f,-65.0f),{},1,74.0f,92.0f,-77.0f,-65.0f,true, PURPLE}
    };
    createEnemiesofType(em, Vec_Diagonal, 3, z);
}
// //----------------------------------------------------------------------------------------------------------------

// //Funcion principal que llama a las funciones de creación de enemigos dado una zona
void Ia_man::createEnemiesZone(EntityManager& em, uint16_t zone) {
    switch (zone)
    {
    case 2: this->createEnemiesZone2(em, zone);
        break;
    case 3: this->createEnemiesZone3(em, zone);
        break;
    case 4:this->createEnemiesZone4(em, zone);
        break;
    case 5: this->createEnemiesZone5(em, zone);
        break;
    case 6: this->createEnemiesZone6(em, zone);
        break;
    case 11: this->createEnemiesZone11(em, zone);
        break;
    case 12: this->createEnemiesZone12(em, zone);
        break;
    default:
        break;
    };
}