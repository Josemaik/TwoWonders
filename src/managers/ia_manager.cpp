#include "ia_manager.hpp"
////////////////////////////////////////////////////
//Funciones para obtener datos de forma aleatoria///
//////////////////////////////////////////////////////---------------------------------

//DEvuelve un tiempo en segundos random
double getRandomStop() {
    switch (std::rand() % 2) {
    case 0:  return 12.0; break;
    case 1:  return 6.0; break;
    case 2:  return 16.0; break;
    default: return 0.0; break;
    }
}
// //Devuelve una posición random dado un rango
vec3d getRandomPosinRange(double xmin, double xmax, double zmin, double zmax) {
    // Semilla para generar números aleatorios
    std::random_device rd;
    std::mt19937 gen(rd());
    // Creo rangos
    std::uniform_real_distribution<double> rangoX(xmin, xmax);
    std::uniform_real_distribution<double> rangoZ(zmin, zmax);
    // Obtengo x y z aleatoria
    double x = rangoX(gen);
    double z = rangoZ(gen);
    // Devuelvo vector
    return vec3d{ x, 0.0, z };
}
//-----------------------------------------------------------------------------------
/////////////////////////////////////////////////////7
///Clase con los Datos necesarios para los enemigos
///////////////////////////////////////////////////////----------------------------
enum class TypeEnemies : uint8_t { Patrol, RanndomShoot, PlayerShoot, Bat, Drake };
struct EnemyData {
    TypeEnemies currentType;
    vec3d position;
    std::array<vec3d, 10> route;
    int num_lifes;
    double Xmin{}, Xmax{}, Zmin{}, Zmax{};
    bool visible{};
    Color color{};
};
//------------------------------------------------------------------------------------------------
// Función que crea enemigos dado un tipo ,vector de datos y zona //
void createEnemiesofType(EntityManager& em, std::vector<EnemyData>vec, BehaviourTree_t& tree) {
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
        auto& p = em.addComponent<PhysicsComponent>(enemy, PhysicsComponent{ .position = { r.position }, .velocity = { .2, .0, .0 } });
        em.addComponent<LifeComponent>(enemy, LifeComponent{ .life = data.num_lifes });
        em.addComponent<ColliderComponent>(enemy, ColliderComponent{ p.position, r.scale, BehaviorType::ENEMY });

        switch (data.currentType)
        {
        case TypeEnemies::Patrol: em.addComponent<AIComponent>(enemy, AIComponent{.patrol = data.route ,.behaviourTree=&tree});
            break;
        case TypeEnemies::PlayerShoot:em.addComponent<AIComponent>(enemy, AIComponent{.detectplayer=true,.ghost=true,.Xmin = data.Xmin,.Xmax = data.Xmax,.Zmin = data.Zmin,.Zmax = data.Zmax,.countdown_shoot=4.0,.behaviourTree=&tree});
            em.addComponent<AttackComponent>(enemy, AttackComponent{ .countdown = 4.5 });
            break;
        case TypeEnemies::RanndomShoot:
            em.addComponent<AIComponent>(enemy, AIComponent{.Xmin = data.Xmin,.Xmax = data.Xmax,.Zmin = data.Zmin,.Zmax = data.Zmax,.countdown_stop = getRandomStop(),.countdown_shoot=3.0,.behaviourTree=&tree});
            em.addComponent<AttackComponent>(enemy, AttackComponent{ .countdown = 1.5 });
            break;
        case TypeEnemies::Bat:  em.addComponent<AIComponent>(enemy, AIComponent{.Xmin = data.Xmin,.Xmax = data.Xmax,.Zmin = data.Zmin,.Zmax = data.Zmax,.countdown_stop = getRandomStop(),.behaviourTree=&tree});
            break;
        case TypeEnemies::Drake: em.addComponent<AIComponent>(enemy, AIComponent{ .patrol = data.route,.countdown_stop=2.5,.countdown_shoot=0.0,.behaviourTree=&tree });
            em.addComponent<AttackComponent>(enemy, AttackComponent{ .countdown = 0.0 });
            r.scale = { 2.0,2.5,2.5 };
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
void Ia_man::createEnemiesZone2(EntityManager& em) {
    createdzone2 = true;
    //Creamos Patrol Enemies
    std::vector<EnemyData> Vec_patrolData = {
       { .currentType =TypeEnemies::Patrol,.position ={-9.0, 0., -14.0},
        .route = {
            vec3d{-9.0, 0., -14.0},
            { -9.0, 0., -10.0 },
            { 9.0, 0., -10.0 },
            { 9.0, 0., -14.0 },
            AIComponent::invalid
        }, .num_lifes = 1,.Xmin= 0.0,.Xmax=0.0,.Zmin=0.0,.Zmax=0.0,.visible=true,.color=ORANGE}
    };
    //Creamos los nodos del behaviour tree
    tree.createNode<BTActionPatrol>();
    createEnemiesofType(em, Vec_patrolData,tree);
}
void Ia_man::createEnemiesZone3(EntityManager& em) {
    createdzone3 = true;
    //Crearemos IA random
    std::vector<EnemyData> Vec_RandomShoot_3 = {
       {.currentType =TypeEnemies::RanndomShoot,.position=getRandomPosinRange(-31.0,-13.0,-6.0,-3.0),.route={},.num_lifes=1,.Xmin=-31.0,.Xmax=-11.0,.Zmin=-7.0,.Zmax=8.0, .visible=true,.color=ORANGE},
       {.currentType =TypeEnemies::RanndomShoot,.position=getRandomPosinRange(-31.0,-13.0,-3.0,-1.0),.route={},.num_lifes=1,.Xmin=-31.0,.Xmax=-11.0,.Zmin=-7.0,.Zmax=8.0, .visible=true,.color=ORANGE},
       {.currentType =TypeEnemies::RanndomShoot,.position=getRandomPosinRange(-31.0,-13.0,-1.0,2.0), .route={},.num_lifes=1,.Xmin=-31.0,.Xmax=-11.0,.Zmin=-7.0,.Zmax=8.0, .visible=true,.color=ORANGE},
       {.currentType =TypeEnemies::RanndomShoot,.position=getRandomPosinRange(-31.0,-13.0,2.0,4.0),  .route={},.num_lifes=1,.Xmin=-31.0,.Xmax=-11.0,.Zmin=-7.0,.Zmax=8.0, .visible=true,.color=ORANGE},
       {.currentType =TypeEnemies::RanndomShoot,.position=getRandomPosinRange(-31.0,-13.0,4.0,6.0),  .route={},.num_lifes=1,.Xmin=-31.0,.Xmax=-11.0,.Zmin=-7.0,.Zmax=8.0, .visible=true,.color=ORANGE}
    };
    tree.createNode<BTNodeSequence_t>(
         &tree.createNode<BTActionRandomMovement>(),
         &tree.createNode<BTActionShoot>(AIComponent::TypeShoot::OneShootonDir)
    );
    createEnemiesofType(em, Vec_RandomShoot_3,tree);
}
void Ia_man::createEnemiesZone4(EntityManager& em) {
    createdzone4 = true;
    //Creamos Random enemies
    std::vector<EnemyData> Vec_RandomShoot = {
        {.currentType =TypeEnemies::RanndomShoot,.position=getRandomPosinRange(-32.0,-13.0,-22.0,-10.0),.route={},.num_lifes=1,.Xmin=-32.0,.Xmax=-11.0,.Zmin=-24.0,.Zmax=-9.0,.visible=true,.color=ORANGE},
        {.currentType =TypeEnemies::RanndomShoot,.position=getRandomPosinRange(-32.0,-13.0,-22.0,-10.0),.route={},.num_lifes=1,.Xmin=-32.0,.Xmax=-11.0,.Zmin=-24.0,.Zmax=-9.0,.visible=true,.color=ORANGE},
        {.currentType =TypeEnemies::RanndomShoot,.position=getRandomPosinRange(-32.0,-13.0,-22.0,-10.0),.route={},.num_lifes=1,.Xmin=-32.0,.Xmax=-11.0,.Zmin=-24.0,.Zmax=-9.0,.visible=true,.color=ORANGE},
        {.currentType =TypeEnemies::RanndomShoot,.position=getRandomPosinRange(-32.0,-13.0,-22.0,-10.0),.route={},.num_lifes=1,.Xmin=-32.0,.Xmax=-11.0,.Zmin=-24.0,.Zmax=-9.0,.visible=true,.color=ORANGE},
        {.currentType =TypeEnemies::RanndomShoot,.position=getRandomPosinRange(-32.0,-13.0,-22.0,-10.0),.route={},.num_lifes=1,.Xmin=-32.0,.Xmax=-11.0,.Zmin=-24.0,.Zmax=-9.0,.visible=true,.color=ORANGE}
    };
    tree.createNode<BTNodeSequence_t>(
         &tree.createNode<BTActionRandomMovement>(),
         &tree.createNode<BTActionShoot>(AIComponent::TypeShoot::OneShootonDir)
    );
    createEnemiesofType(em, Vec_RandomShoot,tree);
}
//Creamos murcielgaos
void Ia_man::createEnemiesZone5(EntityManager& em) {
    createdzone5 = true;
    std::vector<EnemyData> Vec_ShootPlayerData = {
        { .currentType =TypeEnemies::PlayerShoot,.position={-45.0, 0.0, 4.0},
         .route={vec3d{}},.num_lifes=2,.Xmin=-43.0,.Xmax=-46.0,.Zmin=3.0,.Zmax=-4.0,.visible=false,.color = ORANGE
        }
    };
    tree.createNode<BTNodeSequence_t>(
        &tree.createNode<BTActionChangePosition>(),
        &tree.createNode<BTDecisionPlayerDetected>(),
        &tree.createNode<BTActionShoot>(AIComponent::TypeShoot::OneShoottoPlayer)
    );
    createEnemiesofType(em, Vec_ShootPlayerData,tree);
}
//creamos enemigos edel río
void Ia_man::createEnemiesZone6(EntityManager& em) {
    createdzone6 = true;
    std::vector<EnemyData> Vec_ShootPlayerData = {
        {.currentType =TypeEnemies::PlayerShoot,.position= {-46.0, 0.0, -20.0},
         .route={vec3d{},},.num_lifes=2,.Xmin=-43.0,.Xmax=-46.0,.Zmin=-11.0,.Zmax=-20.0,.visible=false,.color=ORANGE
        }
    };
    // tree4.createNode<BTActionShootPlayer>();
   tree.createNode<BTNodeSequence_t>(
        &tree.createNode<BTActionChangePosition>(),
        &tree.createNode<BTDecisionPlayerDetected>(),
        &tree.createNode<BTActionShoot>(AIComponent::TypeShoot::OneShoottoPlayer)
    );
    createEnemiesofType(em, Vec_ShootPlayerData,tree);
}
void Ia_man::createEnemiesZone11(EntityManager& em) {
    createdzone11 = true;
    std::vector<EnemyData> Vec_Drake = {
      {.currentType =TypeEnemies::Drake, .position={73.0, 0., -87.0},
       .route={
           vec3d{73.0, 0., -87.0},
           { 69.0, 0., -87.0 },
           AIComponent::invalid
       },.num_lifes=10,.Xmin=0.0,.Xmax=0.0,.Zmin=0.0,.Zmax=0.0,.visible=true,.color=RED}
    };
    tree.createNode<BTNodeSequence_t>(
        &tree.createNode<BTActionPatrol>(),
        &tree.createNode<BTActionShoot>(AIComponent::TypeShoot::TripleShoot)
    );
    createEnemiesofType(em, Vec_Drake,tree);
}
void Ia_man::createEnemiesZone12(EntityManager& em) {
    createdzone12 = true;
    std::vector<EnemyData> Vec_Diagonal = {
        {.currentType =TypeEnemies::Bat,.position=getRandomPosinRange(74.0,92.0,-77.0,-65.0),.route={},.num_lifes=1,.Xmin=74.0,.Xmax=92.0,.Zmin=-77.0,.Zmax=-65.0,.visible=true,.color=PURPLE},
        {.currentType =TypeEnemies::Bat,.position=getRandomPosinRange(74.0,92.0,-77.0,-65.0),.route={},.num_lifes=1,.Xmin=74.0,.Xmax=92.0,.Zmin=-77.0,.Zmax=-65.0,.visible=true,.color=PURPLE},
        {.currentType =TypeEnemies::Bat,.position=getRandomPosinRange(74.0,92.0,-77.0,-65.0),.route={},.num_lifes=1,.Xmin=74.0,.Xmax=92.0,.Zmin=-77.0,.Zmax=-65.0,.visible=true,.color=PURPLE},
        {.currentType =TypeEnemies::Bat,.position=getRandomPosinRange(74.0,92.0,-77.0,-65.0),.route={},.num_lifes=1,.Xmin=74.0,.Xmax=92.0,.Zmin=-77.0,.Zmax=-65.0,.visible=true,.color=PURPLE},
        {.currentType =TypeEnemies::Bat,.position=getRandomPosinRange(74.0,92.0,-77.0,-65.0),.route={},.num_lifes=1,.Xmin=74.0,.Xmax=92.0,.Zmin=-77.0,.Zmax=-65.0,.visible=true,.color=PURPLE}
    };
    tree.createNode<BTActionDiagonalMovement>();
    createEnemiesofType(em, Vec_Diagonal,tree);
}
// //----------------------------------------------------------------------------------------------------------------

// //Funcion principal que llama a las funciones de creación de enemigos dado una zona
void Ia_man::createEnemiesZone(EntityManager& em,uint16_t zone) {
    (void)em;
    switch (zone)
    {
    case 2: //this->createEnemiesZone2(em);
        break;
    case 3: //this->createEnemiesZone3(em);
        break;
    case 4://this->createEnemiesZone4(em);
        break;
    case 5: //this->createEnemiesZone5(em);
        break;
    case 6: //this->createEnemiesZone6(em);
        break;
    case 11: //this->createEnemiesZone11(em);
        break;
    case 12: //this->createEnemiesZone12(em);
        break;
    default:
        break;
    };
}
//IA FOR TEST STEERING BEHAVIOUR
BehaviourTree_t tree1;
BehaviourTree_t tree2;
BehaviourTree_t tree3;
void Ia_man::createEnemies(EntityManager& em){
        auto& e1{ em.newEntity() };
        //em.addTag<EnemyTag>(e1);
       // auto& e2{ em.newEntity() };
        //em.addTag<EnemyTag>(e2);
        auto& e3{ em.newEntity() };
        // em.addTag<EnemyTag>(wall);
        auto& wr1 = em.addComponent<RenderComponent>(e1, RenderComponent{ .position = vec3d{6.5,0.0,5.0}, .scale = vec3d{1.0,1.0,1.0}, .color = BLUE });
        auto& wp1 = em.addComponent<PhysicsComponent>(e1, PhysicsComponent{ .position = vec3d(wr1.position),.gravity=2.0});
        em.addComponent<ColliderComponent>(e1, ColliderComponent{ wp1.position, wr1.scale, BehaviorType::ENEMY });
        tree1.createNode<BTAction_Seek>();
        em.addComponent<AIComponent>(e1,AIComponent{.arrival_radius=1.0,.tx=0.0,.tz=0.0,.time2arrive=0.02,.tactive=true,.perceptionTime=1.2f,.behaviourTree=&tree1});

        // auto& wr2 = em.addComponent<RenderComponent>(e2, RenderComponent{ .position = vec3d{-6.5,0.0,5.0}, .scale = vec3d{1.0,1.0,1.0}, .color = BLUE });
        // auto& wp2 = em.addComponent<PhysicsComponent>(e2, PhysicsComponent{ .position = vec3d(wr2.position)});
        // em.addComponent<ColliderComponent>(e2, ColliderComponent{ wp2.position, wr2.scale, BehaviorType::ENEMY });
        // tree2.createNode<BTAction_Arrive>();
        // em.addComponent<AIComponent>(e2,AIComponent{.arrival_radius=1.0,.tx=0.0,.tz=0.0,.time2arrive=2.0,.tactive=true,.behaviourTree=&tree2});

        auto& wr3 = em.addComponent<RenderComponent>(e3, RenderComponent{ .position = vec3d{0.0,0.0,5.0}, .scale = vec3d{1.0,1.0,1.0}, .color = BLUE });
        auto& wp3 = em.addComponent<PhysicsComponent>(e3, PhysicsComponent{ .position = vec3d(wr3.position)});
        em.addComponent<ColliderComponent>(e3, ColliderComponent{ wp3.position, wr3.scale, BehaviorType::ENEMY });
        tree3.createNode<BTAction_Seek>();
        em.addComponent<AIComponent>(e3,AIComponent{.arrival_radius=10.0,.tx=0.0,.tz=0.0,.time2arrive=1.0,.tactive=true,.perceptionTime=0.1f,.behaviourTree=&tree3});
}