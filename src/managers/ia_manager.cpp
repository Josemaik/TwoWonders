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
enum class TypeEnemies : uint8_t {Patrol,RanndomShoot,PlayerShoot,Bat,Drake};
struct EnemyData {
    TypeEnemies currentType;
    vec3f position;
    std::array<vec3f, 10> route;
    int num_lifes;
    float Xmin{}, Xmax{}, Zmin{}, Zmax{};
    bool visible{};
};
//------------------------------------------------------------------------------------------------
// Función que crea enemigos dado un tipo ,vector de datos y zona //
void createEnemiesofType(EntityManager& em, std::vector<EnemyData>vec, uint16_t z, BehaviourTree_t& tree) {
    // Obtenemos la clase con información del nivel
    auto& li = em.getSingleton<LevelInfo>();
    // Vacíamos enemigos de la zona anterior del vector
    li.enemiesID.clear();
    //Creamos enemigos
    for (const auto& data : vec)
    {
        auto& enemy{ em.newEntity() };
        em.addTag<EnemyTag>(enemy);
        auto& r = em.addComponent<RenderComponent>(enemy, RenderComponent{ .position = data.position, .scale = { 1.0f, 1.0f, 1.0f }, .color = ORANGE ,.visible = data.visible });
        auto& p = em.addComponent<PhysicsComponent>(enemy, PhysicsComponent{ .position = { r.position }, .velocity = { .2f, .0f, .0f } });
        em.addComponent<LifeComponent>(enemy, LifeComponent{ .life = data.num_lifes });
        em.addComponent<ColliderComponent>(enemy, ColliderComponent{ p.position, r.scale, BehaviorType::ENEMY });

        switch (data.currentType)
        {
        case TypeEnemies::Patrol: em.addComponent<PatrolComponent>(enemy, PatrolComponent{ .patrol = data.route ,.behaviourTree=&tree});
            break;
        case TypeEnemies::PlayerShoot:em.addComponent<ShootPlayerComponent>(enemy, ShootPlayerComponent{.Xmin = data.Xmin,.Xmax = data.Xmax,.Zmin = data.Zmin,.Zmax = data.Zmax});
            em.addComponent<AttackComponent>(enemy, AttackComponent{ .countdown = 3.0f });
            break;
        case TypeEnemies::RanndomShoot:
            em.addComponent<RandomShootComponent>(enemy, RandomShootComponent{.countdown_stop = getRandomStop(),.Xmin = data.Xmin,.Xmax = data.Xmax,.Zmin = data.Zmin,.Zmax = data.Zmax});
            em.addComponent<AttackComponent>(enemy, AttackComponent{ .countdown = 0.0f });
            break;
        case TypeEnemies::Bat:  em.addComponent<DiagonalComponent>(enemy, DiagonalComponent{.countdown_stop = getRandomStop(),.Xmin = data.Xmin,.Xmax = data.Xmax,.Zmin = data.Zmin,.Zmax = data.Zmax});
            em.addComponent<ZoneComponent>(enemy, ZoneComponent{ .zone = z });
            break;
        case TypeEnemies::Drake: em.addComponent<DrakeComponent>(enemy, DrakeComponent{ .patrol = data.route });
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
//Creamos behaviour tree
BehaviourTree_t tree;
//Creamos los enemigos de la Zona 2
void Ia_man::createEnemiesZone2(EntityManager& em, uint16_t z) {
    createdzone2 = true;
    //Creamos Patrol Enemies
    std::vector<EnemyData> Vec_patrolData = {
       { .currentType =TypeEnemies::Patrol,.position ={-9.0f, 0.f, -14.0f}, 
        .route = {
            vec3f{-9.0f, 0.f, -14.0f},
            { -9.0f, 0.f, -10.0f },
            { 9.0f, 0.f, -10.0f },
            { 9.0f, 0.f, -14.0f },
            PatrolComponent::invalid
        }, .num_lifes = 1,.Xmin= 0.0f,.Xmax=0.0f,.Zmin=0.0f,.Zmax=0.0f,.visible=true}
    };
    //Creamos los nodos del behaviour tree
    tree.createNode<BTActionArrive_t>( vec3f{100.0f, 50.0f,20.0f} );
    createEnemiesofType(em, Vec_patrolData,z,tree);
}

void Ia_man::createEnemiesZone3(EntityManager& em, uint16_t z) {
    createdzone3 = true;
    //Crearemos IA random
    std::vector<EnemyData> Vec_RandomShoot_3 = {
       {.currentType =TypeEnemies::RanndomShoot,.position=getRandomPosinRange(-31.0f,-13.0f,-6.0f,-3.0f),.route={},.num_lifes=1,.Xmin=-31.0f,.Xmax=-11.0f,.Zmin=-7.0f,.Zmax=8.0f, .visible=true},
       {.currentType =TypeEnemies::RanndomShoot,.position=getRandomPosinRange(-31.0f,-13.0f,-3.0f,-1.0f),.route={},.num_lifes=1,.Xmin=-31.0f,.Xmax=-11.0f,.Zmin=-7.0f,.Zmax=8.0f, .visible=true},
       {.currentType =TypeEnemies::RanndomShoot,.position=getRandomPosinRange(-31.0f,-13.0f,-1.0f,2.0f), .route={},.num_lifes=1,.Xmin=-31.0f,.Xmax=-11.0f,.Zmin=-7.0f,.Zmax=8.0f, .visible=true},
       {.currentType =TypeEnemies::RanndomShoot,.position=getRandomPosinRange(-31.0f,-13.0f,2.0f,4.0f),  .route={},.num_lifes=1,.Xmin=-31.0f,.Xmax=-11.0f,.Zmin=-7.0f,.Zmax=8.0f, .visible=true},
       {.currentType =TypeEnemies::RanndomShoot,.position=getRandomPosinRange(-31.0f,-13.0f,4.0f,6.0f),  .route={},.num_lifes=1,.Xmin=-31.0f,.Xmax=-11.0f,.Zmin=-7.0f,.Zmax=8.0f, .visible=true}
    };
    createEnemiesofType(em, Vec_RandomShoot_3, z,tree);
}
void Ia_man::createEnemiesZone4(EntityManager& em, uint16_t z) {
    createdzone4 = true;
    //Creamos Random enemies
    std::vector<EnemyData> Vec_RandomShoot = {
        {.currentType =TypeEnemies::RanndomShoot,.position=getRandomPosinRange(-32.0f,-13.0f,-22.0f,-10.0f),.route={},.num_lifes=1,.Xmin=-32.0f,.Xmax=-11.0f,.Zmin=-24.0f,.Zmax=-9.0f,.visible=true},
        {.currentType =TypeEnemies::RanndomShoot,.position=getRandomPosinRange(-32.0f,-13.0f,-22.0f,-10.0f),.route={},.num_lifes=1,.Xmin=-32.0f,.Xmax=-11.0f,.Zmin=-24.0f,.Zmax=-9.0f,.visible=true},
        {.currentType =TypeEnemies::RanndomShoot,.position=getRandomPosinRange(-32.0f,-13.0f,-22.0f,-10.0f),.route={},.num_lifes=1,.Xmin=-32.0f,.Xmax=-11.0f,.Zmin=-24.0f,.Zmax=-9.0f,.visible=true},
        {.currentType =TypeEnemies::RanndomShoot,.position=getRandomPosinRange(-32.0f,-13.0f,-22.0f,-10.0f),.route={},.num_lifes=1,.Xmin=-32.0f,.Xmax=-11.0f,.Zmin=-24.0f,.Zmax=-9.0f,.visible=true},
        {.currentType =TypeEnemies::RanndomShoot,.position=getRandomPosinRange(-32.0f,-13.0f,-22.0f,-10.0f),.route={},.num_lifes=1,.Xmin=-32.0f,.Xmax=-11.0f,.Zmin=-24.0f,.Zmax=-9.0f,.visible=true}
    };
    createEnemiesofType(em, Vec_RandomShoot,z,tree);
}
void Ia_man::createEnemiesZone5(EntityManager& em, uint16_t z) {
    createdzone5 = true;
    std::vector<EnemyData> Vec_ShootPlayerData = {
        { .currentType =TypeEnemies::PlayerShoot,.position={-45.0f, 0.0f, 4.0f},
         .route={vec3f{}},.num_lifes=2,.Xmin=-43.0f,.Xmax=-46.0f,.Zmin=3.0f,.Zmax=-4.0f,.visible=false
        }
    };
    createEnemiesofType(em, Vec_ShootPlayerData,z,tree);
}
void Ia_man::createEnemiesZone6(EntityManager& em, uint16_t z) {
    createdzone6 = true;
    std::vector<EnemyData> Vec_ShootPlayerData = {
        {.currentType =TypeEnemies::PlayerShoot,.position= {-46.0f, 0.0f, -20.0f},
         .route={vec3f{},},.num_lifes=2,.Xmin=-43.0f,.Xmax=-46.0f,.Zmin=-11.0f,.Zmax=-20.0f,.visible=false
        }
    };
    createEnemiesofType(em, Vec_ShootPlayerData,z,tree);
}
void Ia_man::createEnemiesZone11(EntityManager& em, uint16_t z) {
    createdzone11 = true;
    std::vector<EnemyData> Vec_Drake = {
      {.currentType =TypeEnemies::Drake, .position={73.0f, 0.f, -87.0f},
       .route={
           vec3f{73.0f, 0.f, -87.0f},
           { 69.0f, 0.f, -87.0f },
           DrakeComponent::invalid
       },.num_lifes=10,.Xmin=0.0f,.Xmax=0.0f,.Zmin=0.0f,.Zmax=0.0f,.visible=true}
    };
    createEnemiesofType(em, Vec_Drake,z,tree);
}
void Ia_man::createEnemiesZone12(EntityManager& em, uint16_t z) {
    createdzone12 = true;
    std::vector<EnemyData> Vec_Diagonal = {
        {.currentType =TypeEnemies::Bat,.position=getRandomPosinRange(74.0f,92.0f,-77.0f,-65.0f),.route={},.num_lifes=1,.Xmin=74.0f,.Xmax=92.0f,.Zmin=-77.0f,.Zmax=-65.0f,.visible=true},
        {.currentType =TypeEnemies::Bat,.position=getRandomPosinRange(74.0f,92.0f,-77.0f,-65.0f),.route={},.num_lifes=1,.Xmin=74.0f,.Xmax=92.0f,.Zmin=-77.0f,.Zmax=-65.0f,.visible=true},
        {.currentType =TypeEnemies::Bat,.position=getRandomPosinRange(74.0f,92.0f,-77.0f,-65.0f),.route={},.num_lifes=1,.Xmin=74.0f,.Xmax=92.0f,.Zmin=-77.0f,.Zmax=-65.0f,.visible=true},
        {.currentType =TypeEnemies::Bat,.position=getRandomPosinRange(74.0f,92.0f,-77.0f,-65.0f),.route={},.num_lifes=1,.Xmin=74.0f,.Xmax=92.0f,.Zmin=-77.0f,.Zmax=-65.0f,.visible=true},
        {.currentType =TypeEnemies::Bat,.position=getRandomPosinRange(74.0f,92.0f,-77.0f,-65.0f),.route={},.num_lifes=1,.Xmin=74.0f,.Xmax=92.0f,.Zmin=-77.0f,.Zmax=-65.0f,.visible=true}
    };
    createEnemiesofType(em, Vec_Diagonal,z,tree);
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