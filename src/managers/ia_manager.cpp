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
    // auto& li = em.getSingleton<LevelInfo>();
    // Vacíamos enemigos de la zona anterior del vector
    // li.enemiesID.clear();
    //Creamos enemigos
    for (const auto& data : vec)
    {
        auto& enemy{ em.newEntity() };
        em.addTag<EnemyTag>(enemy);
        auto& r = em.addComponent<RenderComponent>(enemy, RenderComponent{ .position = data.position, .scale = { 1.0f, 1.0f, 1.0f }, .color = data.color ,.visible = data.visible });
        auto& p = em.addComponent<PhysicsComponent>(enemy, PhysicsComponent{ .position = r.position , .velocity = { .2, .0, .0 }, .scale = r.scale, });
        em.addComponent<LifeComponent>(enemy, LifeComponent{ .life = data.num_lifes });
        em.addComponent<ColliderComponent>(enemy, ColliderComponent{ p.position, r.scale, BehaviorType::ENEMY });

        switch (data.currentType)
        {
        case TypeEnemies::Patrol: em.addComponent<AIComponent>(enemy, AIComponent{ .patrol = data.route ,.behaviourTree = &tree });
            break;
        case TypeEnemies::PlayerShoot:em.addComponent<AIComponent>(enemy, AIComponent{ .detectplayer = true,.ghost = true,.Xmin = data.Xmin,.Xmax = data.Xmax,.Zmin = data.Zmin,.Zmax = data.Zmax,.countdown_shoot = 4.0,.behaviourTree = &tree });
            em.addComponent<AttackComponent>(enemy, AttackComponent{ .countdown = 4.5 });
            break;
        case TypeEnemies::RanndomShoot:
            em.addComponent<AIComponent>(enemy, AIComponent{ .Xmin = data.Xmin,.Xmax = data.Xmax,.Zmin = data.Zmin,.Zmax = data.Zmax,.countdown_stop = getRandomStop(),.countdown_shoot = 3.0,.behaviourTree = &tree });
            em.addComponent<AttackComponent>(enemy, AttackComponent{ .countdown = 1.5 });
            break;
        case TypeEnemies::Bat:  em.addComponent<AIComponent>(enemy, AIComponent{ .Xmin = data.Xmin,.Xmax = data.Xmax,.Zmin = data.Zmin,.Zmax = data.Zmax,.countdown_stop = getRandomStop(),.behaviourTree = &tree });
            break;
        case TypeEnemies::Drake: em.addComponent<AIComponent>(enemy, AIComponent{ .patrol = data.route,.countdown_stop = 2.5,.countdown_shoot = 0.0,.behaviourTree = &tree });
            em.addComponent<AttackComponent>(enemy, AttackComponent{ .countdown = 0.0 });
            r.scale = { 2.0,2.5,2.5 };
            break;
        default:
            break;
        }
        //Insertamos enemigos en el vector
        // li.enemiesID.insert(enemy.getID());
    }
}
/////////////////////////////////////////////////////////////
//Funciones para crear los enemigos de cada Zona ////////////
/////////////////////////////////////////////////////////////--------------------------------------------------------
//Creamos behaviour tree
// BehaviourTree_t tree;
// //Creamos los enemigos de la Zona 2
// void Ia_man::createEnemiesZone2(EntityManager& em) {
//     createdzone2 = true;
//     //Creamos Patrol Enemies
//     std::vector<EnemyData> Vec_patrolData = {
//        {.currentType = TypeEnemies::Patrol,.position = {-9.0, 0., -14.0},
//         .route = {
//             vec3d{-9.0, 0., -14.0},
//             { -9.0, 0., -10.0 },
//             { 9.0, 0., -10.0 },
//             { 9.0, 0., -14.0 },
//             AIComponent::invalid
//         }, .num_lifes = 1,.Xmin = 0.0,.Xmax = 0.0,.Zmin = 0.0,.Zmax = 0.0,.visible = true,.color = ORANGE}
//     };
//     //Creamos los nodos del behaviour tree
//     tree.createNode<BTAction_Patrol>();
//     createEnemiesofType(em, Vec_patrolData, tree);
// }
// void Ia_man::createEnemiesZone3(EntityManager& em) {
//     createdzone3 = true;
//     //Crearemos IA random
//     std::vector<EnemyData> Vec_RandomShoot_3 = {
//        {.currentType = TypeEnemies::RanndomShoot,.position = getRandomPosinRange(-31.0,-13.0,-6.0,-3.0),.route = {},.num_lifes = 1,.Xmin = -31.0,.Xmax = -11.0,.Zmin = -7.0,.Zmax = 8.0, .visible = true,.color = ORANGE},
//        {.currentType = TypeEnemies::RanndomShoot,.position = getRandomPosinRange(-31.0,-13.0,-3.0,-1.0),.route = {},.num_lifes = 1,.Xmin = -31.0,.Xmax = -11.0,.Zmin = -7.0,.Zmax = 8.0, .visible = true,.color = ORANGE},
//        {.currentType = TypeEnemies::RanndomShoot,.position = getRandomPosinRange(-31.0,-13.0,-1.0,2.0), .route = {},.num_lifes = 1,.Xmin = -31.0,.Xmax = -11.0,.Zmin = -7.0,.Zmax = 8.0, .visible = true,.color = ORANGE},
//        {.currentType = TypeEnemies::RanndomShoot,.position = getRandomPosinRange(-31.0,-13.0,2.0,4.0),  .route = {},.num_lifes = 1,.Xmin = -31.0,.Xmax = -11.0,.Zmin = -7.0,.Zmax = 8.0, .visible = true,.color = ORANGE},
//        {.currentType = TypeEnemies::RanndomShoot,.position = getRandomPosinRange(-31.0,-13.0,4.0,6.0),  .route = {},.num_lifes = 1,.Xmin = -31.0,.Xmax = -11.0,.Zmin = -7.0,.Zmax = 8.0, .visible = true,.color = ORANGE}
//     };
//     tree.createNode<BTNodeSequence_t>(
//         &tree.createNode<BTActionRandomMovement>(),
//         &tree.createNode<BTActionShoot>(AIComponent::TypeShoot::OneShootonDir)
//     );
//     createEnemiesofType(em, Vec_RandomShoot_3, tree);
// }
// void Ia_man::createEnemiesZone4(EntityManager& em) {
//     createdzone4 = true;
//     //Creamos Random enemies
//     std::vector<EnemyData> Vec_RandomShoot = {
//         {.currentType = TypeEnemies::RanndomShoot,.position = getRandomPosinRange(-32.0,-13.0,-22.0,-10.0),.route = {},.num_lifes = 1,.Xmin = -32.0,.Xmax = -11.0,.Zmin = -24.0,.Zmax = -9.0,.visible = true,.color = ORANGE},
//         {.currentType = TypeEnemies::RanndomShoot,.position = getRandomPosinRange(-32.0,-13.0,-22.0,-10.0),.route = {},.num_lifes = 1,.Xmin = -32.0,.Xmax = -11.0,.Zmin = -24.0,.Zmax = -9.0,.visible = true,.color = ORANGE},
//         {.currentType = TypeEnemies::RanndomShoot,.position = getRandomPosinRange(-32.0,-13.0,-22.0,-10.0),.route = {},.num_lifes = 1,.Xmin = -32.0,.Xmax = -11.0,.Zmin = -24.0,.Zmax = -9.0,.visible = true,.color = ORANGE},
//         {.currentType = TypeEnemies::RanndomShoot,.position = getRandomPosinRange(-32.0,-13.0,-22.0,-10.0),.route = {},.num_lifes = 1,.Xmin = -32.0,.Xmax = -11.0,.Zmin = -24.0,.Zmax = -9.0,.visible = true,.color = ORANGE},
//         {.currentType = TypeEnemies::RanndomShoot,.position = getRandomPosinRange(-32.0,-13.0,-22.0,-10.0),.route = {},.num_lifes = 1,.Xmin = -32.0,.Xmax = -11.0,.Zmin = -24.0,.Zmax = -9.0,.visible = true,.color = ORANGE}
//     };
//     tree.createNode<BTNodeSequence_t>(
//         &tree.createNode<BTActionRandomMovement>(),
//         &tree.createNode<BTActionShoot>(AIComponent::TypeShoot::OneShootonDir)
//     );
//     createEnemiesofType(em, Vec_RandomShoot, tree);
// }
// //Creamos murcielgaos
// void Ia_man::createEnemiesZone5(EntityManager& em) {
//     createdzone5 = true;
//     std::vector<EnemyData> Vec_ShootPlayerData = {
//         {.currentType = TypeEnemies::PlayerShoot,.position = {-45.0, 0.0, 4.0},
//          .route = {vec3d{}},.num_lifes = 2,.Xmin = -43.0,.Xmax = -46.0,.Zmin = 3.0,.Zmax = -4.0,.visible = false,.color = ORANGE
//         }
//     };
//     tree.createNode<BTNodeSequence_t>(
//         &tree.createNode<BTActionChangePosition>(),
//         &tree.createNode<BTDecisionPlayerDetected>(),
//         &tree.createNode<BTActionShoot>(AIComponent::TypeShoot::OneShoottoPlayer)
//     );
//     createEnemiesofType(em, Vec_ShootPlayerData, tree);
// }
// //creamos enemigos edel río
// void Ia_man::createEnemiesZone6(EntityManager& em) {
//     createdzone6 = true;
//     std::vector<EnemyData> Vec_ShootPlayerData = {
//         {.currentType = TypeEnemies::PlayerShoot,.position = {-46.0, 0.0, -20.0},
//          .route = {vec3d{},},.num_lifes = 2,.Xmin = -43.0,.Xmax = -46.0,.Zmin = -11.0,.Zmax = -20.0,.visible = false,.color = ORANGE
//         }
//     };
//     // tree4.createNode<BTActionShootPlayer>();
//     tree.createNode<BTNodeSequence_t>(
//         &tree.createNode<BTActionChangePosition>(),
//         &tree.createNode<BTDecisionPlayerDetected>(),
//         &tree.createNode<BTActionShoot>(AIComponent::TypeShoot::OneShoottoPlayer)
//     );
//     createEnemiesofType(em, Vec_ShootPlayerData, tree);
// }
// void Ia_man::createEnemiesZone11(EntityManager& em) {
//     createdzone11 = true;
//     std::vector<EnemyData> Vec_Drake = {
//       {.currentType = TypeEnemies::Drake, .position = {73.0, 0., -87.0},
//        .route = {
//            vec3d{73.0, 0., -87.0},
//            { 69.0, 0., -87.0 },
//            AIComponent::invalid
//        },.num_lifes = 10,.Xmin = 0.0,.Xmax = 0.0,.Zmin = 0.0,.Zmax = 0.0,.visible = true,.color = RED}
//     };
//     tree.createNode<BTNodeSequence_t>(
//         &tree.createNode<BTAction_Patrol>(),
//         &tree.createNode<BTActionShoot>(AIComponent::TypeShoot::TripleShoot)
//     );
//     createEnemiesofType(em, Vec_Drake, tree);
// }
// void Ia_man::createEnemiesZone12(EntityManager& em) {
//     createdzone12 = true;
//     std::vector<EnemyData> Vec_Diagonal = {
//         {.currentType = TypeEnemies::Bat,.position = getRandomPosinRange(74.0,92.0,-77.0,-65.0),.route = {},.num_lifes = 1,.Xmin = 74.0,.Xmax = 92.0,.Zmin = -77.0,.Zmax = -65.0,.visible = true,.color = PURPLE},
//         {.currentType = TypeEnemies::Bat,.position = getRandomPosinRange(74.0,92.0,-77.0,-65.0),.route = {},.num_lifes = 1,.Xmin = 74.0,.Xmax = 92.0,.Zmin = -77.0,.Zmax = -65.0,.visible = true,.color = PURPLE},
//         {.currentType = TypeEnemies::Bat,.position = getRandomPosinRange(74.0,92.0,-77.0,-65.0),.route = {},.num_lifes = 1,.Xmin = 74.0,.Xmax = 92.0,.Zmin = -77.0,.Zmax = -65.0,.visible = true,.color = PURPLE},
//         {.currentType = TypeEnemies::Bat,.position = getRandomPosinRange(74.0,92.0,-77.0,-65.0),.route = {},.num_lifes = 1,.Xmin = 74.0,.Xmax = 92.0,.Zmin = -77.0,.Zmax = -65.0,.visible = true,.color = PURPLE},
//         {.currentType = TypeEnemies::Bat,.position = getRandomPosinRange(74.0,92.0,-77.0,-65.0),.route = {},.num_lifes = 1,.Xmin = 74.0,.Xmax = 92.0,.Zmin = -77.0,.Zmax = -65.0,.visible = true,.color = PURPLE}
//     };
//     tree.createNode<BTActionDiagonalMovement>();
//     createEnemiesofType(em, Vec_Diagonal, tree);
// }
// // //----------------------------------------------------------------------------------------------------------------

// // //Funcion principal que llama a las funciones de creación de enemigos dado una zona
// void Ia_man::createEnemiesZone(EntityManager& em, uint16_t zone) {
//     (void)em;
//     switch (zone)
//     {
//     case 2: //this->createEnemiesZone2(em);
//         break;
//     case 3: //this->createEnemiesZone3(em);
//         break;
//     case 4://this->createEnemiesZone4(em);
//         break;
//     case 5: //this->createEnemiesZone5(em);
//         break;
//     case 6: //this->createEnemiesZone6(em);
//         break;
//     case 11: //this->createEnemiesZone11(em);
//         break;
//     case 12: //this->createEnemiesZone12(em);
//         break;
//     default:
//         break;
//     };
// }

void Ia_man::createEnemies(EntityManager&) {

    {
        //Enemigo Dragón
        // auto& e7{ em.newEntity() };
        // em.addTag<EnemyTag>(e7);
        // auto& wr7 = em.addComponent<RenderComponent>(e7, RenderComponent{ .position = vec3d{73.0, 0., -87.0}, .scale = vec3d{ 2.0, 2.0, 2.0 }, .color = BLUE });
        // auto& wp7 = em.addComponent<PhysicsComponent>(e7, PhysicsComponent{ .position = vec3d(wr7.position),.gravity = 2.0, .max_speed = 0.55 });
        // em.addComponent<ColliderComponent>(e7, ColliderComponent{ wp7.position, wr7.scale, BehaviorType::ENEMY });
        // em.addComponent<LifeComponent>(e7, LifeComponent{ .life = 15 });
        // em.addComponent<TypeComponent>(e7, TypeComponent{ .type = ElementalType::Fuego });

        // auto* patrol_7 = &tree7.createNode<BTAction_Patrol>();
        // auto* ready_7 = &tree7.createNode<BTDecisionReadyforAttack>();
        // auto* atack_7 = &tree.createNode<BTActionShoot>(AIComponent::TypeShoot::TripleShoot);
        // [[maybe_unused]] auto* sequence7_3 = &tree7.createNode<BTNodeSequence_t>(patrol_7, ready_7, atack_7);


        // em.addComponent<AIComponent>(e7, AIComponent{ .arrival_radius = 0.7,.detect_radius = 15.0,.attack_radius = 10.0,.ready_attack = false,.tx = 0.0,.tz = 0.0,.time2arrive = 7.0,.tactive = true,.perceptionTime = 0.2f,
        // .path = { vec3d{73.0, 0., -87.0},{ 69.0, 0., -87.0 }, {73.0, 0., -87.0},{ 69.0, 0., -87.0 } },.countdown_stop = 0.0,.countdown_perception = 0.5,.behaviourTree = &tree7 });
        // em.addComponent<AttackComponent>(e7, AttackComponent{ .scale_to_respawn_attack = 7.0 });

        //Enemigo Golem
        // auto& e1{ em.newEntity() };
        // em.addTag<EnemyTag>(e1);
        // auto& wr1 = em.addComponent<RenderComponent>(e1, RenderComponent{ .position = vec3d{ -33.0, 2.0, -15.0 }, .scale = vec3d{ 2.0, 2.0, 2.0 }, .color = BLUE });
        // auto& wp1 = em.addComponent<PhysicsComponent>(e1, PhysicsComponent{ .position = vec3d(wr1.position), .max_speed = 0.55 });
        // em.addComponent<ColliderComponent>(e1, ColliderComponent{ wp1.position, wr1.scale, BehaviorType::ENEMY });
        // em.addComponent<LifeComponent>(e1, LifeComponent{ .life = 10 });
        // em.addComponent<TypeComponent>(e1, TypeComponent{ .type = ElementalType::Fuego });

        // auto* d_a_1 = &tree1.createNode<BTDecisionReadyforAttack>();
        // auto* a_a_1 = &tree1.createNode<BTActionShoot>(AIComponent::TypeShoot::Melee); // fail si disparo succes si no disparo
        // auto* d_r_1 = &tree1.createNode<BTDecisionOnAttackRadius>();
        // auto* sequence1_1 = &tree1.createNode<BTNodeSequence_t>(d_a_1, a_a_1, d_r_1);

        // auto* d_1_1 = &tree1.createNode<BTDecisionPlayerDetected>();
        // auto* a_s_1 = &tree1.createNode<BTAction_Pursue>();
        // auto* sequence1_2 = &tree1.createNode<BTNodeSequence_t>(d_1_1, a_s_1);


        // auto* patrol_1 = &tree1.createNode<BTAction_Patrol>();
        // auto* sequence1_3 = &tree1.createNode<BTNodeSequence_t>(patrol_1);

        // tree1.createNode<BTNodeSelector_t>(sequence1_1, sequence1_2, sequence1_3);

        // em.addComponent<AIComponent>(e1, AIComponent{ .arrival_radius = 0.1,.detect_radius = 7.0,.attack_radius = 2.5,.tx = 0.0,.tz = 0.0,.time2arrive = 1.0,.tactive = true,.perceptionTime = 0.2f,
        // .path = { vec3d{-35.0,0.0,-16.0},{-35,0.0,-14.0}, {-35.0,0.0,-16.0},{-35,0.0,-14.0} },.countdown_perception = 0.5,.behaviourTree = &tree1 });
        // em.addComponent<AttackComponent>(e1, AttackComponent{ .scale_to_respawn_attack = 7.0 });

        //Enemigo Muñeco
        // auto& e2{ em.newEntity() };
        // em.addTag<EnemyTag>(e2);
        // em.addTag<SnowmanTag>(e2);
        // auto& wr2 = em.addComponent<RenderComponent>(e2, RenderComponent{ .position = vec3d{-2.0,0.0,-21.0}, .scale = vec3d{1.0,2.0,1.0}, .color = BLUE });
        // auto& wp2 = em.addComponent<PhysicsComponent>(e2, PhysicsComponent{ .position = vec3d(wr2.position),.gravity = 2.0 });
        // em.addComponent<ColliderComponent>(e2, ColliderComponent{ wp2.position, wr2.scale, BehaviorType::ENEMY });
        // em.addComponent<LifeComponent>(e2, LifeComponent{ .life = 6 });
        // em.addComponent<TypeComponent>(e2, TypeComponent{ .type = ElementalType::Hielo });

        // auto* d_a_2 = &tree2.createNode<BTDecisionReadyforAttack>();
        // auto* a_a_2 = &tree2.createNode<BTActionShoot>(AIComponent::TypeShoot::OneShoottoPlayer); // fail si disparo succes si no disparo
        // auto* d_r_2 = &tree2.createNode<BTDecisionOnAttackRadius>();
        // auto* sequence2_1 = &tree2.createNode<BTNodeSequence_t>(d_a_2, a_a_2, d_r_2);

        // auto* d_1_2 = &tree2.createNode<BTDecisionPlayerDetected>();
        // auto* a_s_2 = &tree2.createNode<BTAction_Seek>();
        // auto* sequence2_2 = &tree2.createNode<BTNodeSequence_t>(d_1_2, a_s_2);


        // auto* patrol_2 = &tree2.createNode<BTAction_Patrol>();
        // auto* sequence2_3 = &tree2.createNode<BTNodeSequence_t>(patrol_2);

        // tree2.createNode<BTNodeSelector_t>(sequence2_1, sequence2_2, sequence2_3);

        // em.addComponent<AIComponent>(e2, AIComponent{ .arrival_radius = 0.1,.detect_radius = 9.0,.tx = 0.0,.tz = 0.0,.time2arrive = 1.0,.tactive = true,.perceptionTime = 0.3f,
        // .path = {vec3d{-7.0,0.0,-20.0},{-3.0,0.0,-20.0},{-7.0,0.0,-20.0},{-3.0,0.0,-20.0}},.countdown_perception = 2.0,.behaviourTree = &tree2 });
        // em.addComponent<AttackComponent>(e2, AttackComponent{});
        // //Enemigo muñeco 2
        // auto& e5{ em.newEntity() };
        // em.addTag<EnemyTag>(e5);
        // em.addTag<SnowmanTag>(e5);
        // auto& wr5 = em.addComponent<RenderComponent>(e5, RenderComponent{ .position = vec3d{-31.0,0.0,-4.0}, .scale = vec3d{1.0,2.0,1.0}, .color = BLUE });
        // auto& wp5 = em.addComponent<PhysicsComponent>(e5, PhysicsComponent{ .position = vec3d(wr5.position),.gravity = 2.0 });
        // em.addComponent<ColliderComponent>(e5, ColliderComponent{ wp5.position, wr5.scale, BehaviorType::ENEMY });
        // em.addComponent<LifeComponent>(e5, LifeComponent{ .life = 6 });
        // em.addComponent<TypeComponent>(e5, TypeComponent{ .type = ElementalType::Fuego });

        // auto* d_a_5 = &tree5.createNode<BTDecisionReadyforAttack>();
        // auto* a_a_5 = &tree5.createNode<BTActionShoot>(AIComponent::TypeShoot::OneShoottoPlayer); // fail si disparo succes si no disparo
        // auto* d_r_5 = &tree5.createNode<BTDecisionOnAttackRadius>();
        // auto* sequence5_1 = &tree5.createNode<BTNodeSequence_t>(d_a_5, a_a_5, d_r_5);

        // auto* d_1_5 = &tree5.createNode<BTDecisionPlayerDetected>();
        // auto* a_s_5 = &tree5.createNode<BTAction_Seek>();
        // auto* sequence5_2 = &tree5.createNode<BTNodeSequence_t>(d_1_5, a_s_5);


        // auto* patrol_5 = &tree5.createNode<BTAction_Patrol>();
        // auto* sequence5_3 = &tree5.createNode<BTNodeSequence_t>(patrol_5);

        // tree5.createNode<BTNodeSelector_t>(sequence5_1, sequence5_2, sequence5_3);

        // em.addComponent<AIComponent>(e5, AIComponent{ .arrival_radius = 0.1,.detect_radius = 9.0,.tx = 0.0,.tz = 0.0,.time2arrive = 1.0,.tactive = true,.perceptionTime = 0.1f,
        // .path = {vec3d{-31.0,0.0,-4.0},{-25.0,0.0,-4.0},{-25.0,0.0,-2.0},{-31.0,0.0,-2.0}},.countdown_perception = 2.5,.behaviourTree = &tree5 });
        // em.addComponent<AttackComponent>(e5, AttackComponent{});

        // //Enemigo muñeco 3
        // auto& e6{ em.newEntity() };
        // em.addTag<EnemyTag>(e6);
        // em.addTag<SnowmanTag>(e6);
        // auto& wr6 = em.addComponent<RenderComponent>(e6, RenderComponent{ .position = vec3d{-31.0,0.0,-8.0}, .scale = vec3d{1.0,2.0,1.0}, .color = BLUE });
        // auto& wp6 = em.addComponent<PhysicsComponent>(e6, PhysicsComponent{ .position = vec3d(wr6.position) });
        // em.addComponent<ColliderComponent>(e6, ColliderComponent{ wp6.position, wr6.scale, BehaviorType::ENEMY });
        // em.addComponent<LifeComponent>(e6, LifeComponent{ .life = 6 });
        // em.addComponent<TypeComponent>(e6, TypeComponent{ .type = ElementalType::Agua });

        // auto* d_a_6 = &tree6.createNode<BTDecisionReadyforAttack>();
        // auto* a_a_6 = &tree6.createNode<BTActionShoot>(AIComponent::TypeShoot::OneShoottoPlayer); // fail si disparo succes si no disparo
        // auto* d_r_6 = &tree6.createNode<BTDecisionOnAttackRadius>();
        // auto* sequence6_1 = &tree6.createNode<BTNodeSequence_t>(d_a_6, a_a_6, d_r_6);

        // auto* d_1_6 = &tree6.createNode<BTDecisionPlayerDetected>();
        // auto* a_s_6 = &tree6.createNode<BTAction_Seek>();
        // auto* sequence6_2 = &tree6.createNode<BTNodeSequence_t>(d_1_6, a_s_6);


        // auto* patrol_6 = &tree6.createNode<BTAction_Patrol>();
        // auto* sequence6_3 = &tree6.createNode<BTNodeSequence_t>(patrol_6);

        // tree6.createNode<BTNodeSelector_t>(sequence6_1, sequence6_2, sequence6_3);

        // em.addComponent<AIComponent>(e6, AIComponent{ .arrival_radius = 0.1,.detect_radius = 12.0,.tx = 0.0,.tz = 0.0,.time2arrive = 1.0,.tactive = true,.perceptionTime = 0.1f,
        // .path = {vec3d{-31.0,0.0,-4.0},{-25.0,0.0,-4.0},{-25.0,0.0,-2.0},{-31.0,0.0,-2.0}},.countdown_perception = 6.1,.behaviourTree = &tree6 });
        // em.addComponent<AttackComponent>(e6, AttackComponent{});


        // //Enemigo Slime
        // auto& e3{ em.newEntity() };
        // em.addTag<SlimeTag>(e3);
        // em.addTag<EnemyTag>(e3);
        // auto& wr3 = em.addComponent<RenderComponent>(e3, RenderComponent{ .position = vec3d{85.0,0.0,-75.0}, .scale = vec3d{1.0,1.0,1.0}, .color = BLUE });
        // auto& wp3 = em.addComponent<PhysicsComponent>(e3, PhysicsComponent{ .position = vec3d(wr3.position),.gravity = 2.0 });
        // em.addComponent<ColliderComponent>(e3, ColliderComponent{ wp3.position, wr3.scale, BehaviorType::ENEMY });
        // em.addComponent<LifeComponent>(e3, LifeComponent{ .life = 4 });
        // em.addComponent<TypeComponent>(e3, TypeComponent{ .type = ElementalType::Fuego });

        // // checklifes < 3 ( si me queda 1 vida , decido de forma aleatoria si huyo/me curo o curo compañero sacrificandome)
        // // decision de si huyo o me curo ( aleatorio )
        // // ( si curar compañero ) miro blackboard (tuplas que tengan posicion y vida) y cmpruebo si tienen < 3 vidas y si esta dentro de radio de cura
        // // ( si falla el curar a otro compañero , huyo y me curo)
        // //hacer estructura
        // auto* d_cl_3 = &tree3.createNode<BTDecisionCheckLifes>();

        // auto* d_foc3 = &tree3.createNode<BTDecisionFleeorCurePartner>();
        // auto* d_rh3 = &tree3.createNode<BTDecisionReadyforHeal>();
        // auto* a_hm3 = &tree3.createNode<BTAction_HealMate>();
        // auto* sequence3_0_0 = &tree3.createNode<BTNodeSequence_t>(d_foc3, d_rh3, a_hm3);

        // auto* a_f_3 = &tree3.createNode<BTAction_Flee>();
        // auto* a_h_3 = &tree3.createNode<BTAction_Healing>();
        // auto* sequence3_0_1 = &tree3.createNode<BTNodeSequence_t>(a_f_3, a_h_3);

        // auto* selector3_0 = &tree3.createNode<BTNodeSelector_t>(sequence3_0_0, sequence3_0_1);

        // auto* sequence3_0 = &tree3.createNode<BTNodeSequence_t>(d_cl_3, selector3_0);

        // auto* d_a_3 = &tree3.createNode<BTDecisionReadyforAttack>();
        // auto* a_j_3 = &tree3.createNode<BTAction_JumptoPlayer>();
        // //auto* a_a_3 = &tree3.createNode<BTActionShoot>(AIComponent::TypeShoot::Melee); // fail si disparo succes si no disparo
        // auto* d_r_3 = &tree3.createNode<BTDecisionOnAttackRadius>();
        // auto* sequence3_2 = &tree3.createNode<BTNodeSequence_t>(d_a_3, a_j_3, d_r_3);

        // auto* d_1_3 = &tree3.createNode<BTDecisionPlayerDetected>();
        // auto* a_s_3 = &tree3.createNode<BTAction_Seek>();
        // auto* sequence3_3 = &tree3.createNode<BTNodeSequence_t>(d_1_3, a_s_3);


        // auto* patrol_3 = &tree3.createNode<BTAction_Patrol>();
        // auto* sequence3_4 = &tree3.createNode<BTNodeSequence_t>(patrol_3);

        // tree3.createNode<BTNodeSelector_t>(sequence3_0, sequence3_2, sequence3_3, sequence3_4);

        // em.addComponent<AIComponent>(e3, AIComponent{ .arrival_radius = 0.1,.detect_radius = 8.0,.attack_radius = 6,.tx = 0.0,.tz = 0.0,.time2arrive = 1.0,.tactive = true,.perceptionTime = 0.1f,
        // .path = { vec3d{85.0, 0.0, -75.0} , {90.0,0.0,-75.0} , {90.0,0.0,-66.0}, {85.0,0.0,-66.0}},.countdown_stop = 2.0,.countdown_shoot = 0.0,.countdown_perception = 1.5,.behaviourTree = &tree3 });
        // em.addComponent<AttackComponent>(e3, AttackComponent{});
        // //Slime 2
        // auto& e4{ em.newEntity() };
        // em.addTag<SlimeTag>(e4);
        // em.addTag<EnemyTag>(e4);
        // auto& wr4 = em.addComponent<RenderComponent>(e4, RenderComponent{ .position = vec3d{75.0,0.0,-74.0}, .scale = vec3d{1.0,1.0,1.0}, .color = BLUE });
        // auto& wp4 = em.addComponent<PhysicsComponent>(e4, PhysicsComponent{ .position = vec3d(wr4.position),.gravity = 2.0 });
        // em.addComponent<ColliderComponent>(e4, ColliderComponent{ wp4.position, wr4.scale, BehaviorType::ENEMY });
        // em.addComponent<LifeComponent>(e4, LifeComponent{ .life = 4 });
        // em.addComponent<TypeComponent>(e4, TypeComponent{ .type = ElementalType::Neutral });

        // auto* d_cl_4 = &tree4.createNode<BTDecisionCheckLifes>();

        // auto* d_foc4 = &tree4.createNode<BTDecisionFleeorCurePartner>();
        // auto* d_rh4 = &tree4.createNode<BTDecisionReadyforHeal>();
        // auto* a_hm4 = &tree4.createNode<BTAction_HealMate>();
        // auto* sequence4_0_0 = &tree4.createNode<BTNodeSequence_t>(d_foc4, d_rh4, a_hm4);

        // auto* a_f_4 = &tree4.createNode<BTAction_Flee>();
        // auto* a_h_4 = &tree4.createNode<BTAction_Healing>();
        // auto* sequence4_0_1 = &tree4.createNode<BTNodeSequence_t>(a_f_4, a_h_4);

        // auto* selector4_0 = &tree4.createNode<BTNodeSelector_t>(sequence4_0_0, sequence4_0_1);

        // auto* sequence4_0 = &tree4.createNode<BTNodeSequence_t>(d_cl_4, selector4_0);

        // auto* d_a_4 = &tree4.createNode<BTDecisionReadyforAttack>();
        // auto* a_j_4 = &tree4.createNode<BTAction_JumptoPlayer>();
        // //auto* a_a_4 = &tree4.createNode<BTActionShoot>(AIComponent::TypeShoot::Melee); // fail si disparo succes si no disparo
        // auto* d_r_4 = &tree4.createNode<BTDecisionOnAttackRadius>();
        // auto* sequence4_2 = &tree4.createNode<BTNodeSequence_t>(d_a_4, a_j_4, d_r_4);

        // auto* d_1_4 = &tree4.createNode<BTDecisionPlayerDetected>();
        // auto* a_s_4 = &tree4.createNode<BTAction_Seek>();
        // auto* sequence4_3 = &tree4.createNode<BTNodeSequence_t>(d_1_4, a_s_4);


        // auto* patrol_4 = &tree4.createNode<BTAction_Patrol>();
        // auto* sequence4_4 = &tree4.createNode<BTNodeSequence_t>(patrol_4);

        // tree4.createNode<BTNodeSelector_t>(sequence4_0, sequence4_2, sequence4_3, sequence4_4);

        // em.addComponent<AIComponent>(e4, AIComponent{ .arrival_radius = 0.1,.detect_radius = 8.0,.attack_radius = 6,.tx = 0.0,.tz = 0.0,.time2arrive = 1.0,.tactive = true,.perceptionTime = 0.5f,
        // .path = { vec3d{75.0, 0.0, -74.0} , {75.0,0.0,-66.0} , {81.0,0.0,-66.0}, {80.0,0.0,-74.0}},.countdown_stop = 2.0,.countdown_shoot = 0.0,.countdown_perception = 1.0,.behaviourTree = &tree4 });
        // em.addComponent<AttackComponent>(e4, AttackComponent{});

    }
}

void Ia_man::createEnemy(EntityManager& em, jsonType json)
{
    // Extraemos los datos del json
    vec3d position = { json["position"][0].GetDouble(), json["position"][1].GetDouble(), json["position"][2].GetDouble() };
    vec3d scale = { json["scale"][0].GetDouble(), json["scale"][1].GetDouble(), json["scale"][2].GetDouble() };
    Color color = { static_cast<u_char>(json["color"][0].GetUint()), static_cast<u_char>(json["color"][1].GetUint()), static_cast<u_char>(json["color"][2].GetUint()), static_cast<u_char>(json["color"][3].GetUint()) };
    double max_speed = json["max_speed"].GetDouble();
    int life = json["life"].GetInt();
    int type = json["type"].GetInt();
    ElementalType element = static_cast<ElementalType>(json["element"].GetDouble());
    double arrival_radius = json["arrival_radius"].GetDouble();
    double detect_radius = json["detect_radius"].GetDouble();
    double attack_radius = json["attack_radius"].GetDouble();
    double tx = json["tx"].GetDouble();
    double tz = json["tz"].GetDouble();
    double time2arrive = json["time2arrive"].GetDouble();
    bool tactive = json["tactive"].GetBool();
    double perceptionTime = json["perceptionTime"].GetDouble();
    Path_t<4> path = { vec3d{json["path"][0][0].GetDouble(), json["path"][0][1].GetDouble(), json["path"][0][2].GetDouble()},
                       vec3d{json["path"][1][0].GetDouble(), json["path"][1][1].GetDouble(), json["path"][1][2].GetDouble()},
                       vec3d{json["path"][2][0].GetDouble(), json["path"][2][1].GetDouble(), json["path"][2][2].GetDouble()},
                       vec3d{json["path"][3][0].GetDouble(), json["path"][3][1].GetDouble(), json["path"][3][2].GetDouble()} };
    // int i = 0;
    // for (const auto& point : json["path"]) {
    //     path[i] = vec3d{ point[0], point[1], point[2] };
    //     i++;
    // }
    double countdown_stop = json["countdown_stop"].GetDouble();
    double countdown_shoot = json["countdown_shoot"].GetDouble();
    double countdown_perception = json["countdown_perception"].GetDouble();
    double scale_to_respawn_attack = json["scale_to_respawn_attack"].GetDouble();

    // Creamos el enemigo
    auto& e{ em.newEntity() };
    em.addTag<EnemyTag>(e);

    auto& wr = em.addComponent<RenderComponent>(e, RenderComponent{ .position = position, .scale = scale, .color = color });
    auto& wp = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position = wr.position, .scale = wr.scale, .max_speed = max_speed });
    em.addComponent<ColliderComponent>(e, ColliderComponent{ wp.position, wr.scale, BehaviorType::ENEMY });
    em.addComponent<LifeComponent>(e, LifeComponent{ .life = life });
    em.addComponent<TypeComponent>(e, TypeComponent{ .type = element });

    // Creamos el arbol de comportamiento
    vec_t.push_back(std::make_unique<BehaviourTree_t>());
    auto& tree = *vec_t.back();

    switch (type)
    {
    case 0:
    {
        em.addTag<GolemTag>(e);

        // auto* d_a_1 = &tree.createNode<BTDecisionReadyforAttack>();
        // auto* a_a_1 = &tree.createNode<BTActionShoot>(AIComponent::TypeShoot::Melee); // fail si disparo succes si no disparo
        // auto* d_r_1 = &tree.createNode<BTDecisionOnAttackRadius>();
        // auto* sequence1_1 = &tree.createNode<BTNodeSequence_t>(d_a_1, a_a_1, d_r_1);

        // auto* d_1_1 = &tree.createNode<BTDecisionPlayerDetected>();
        // auto* a_s_1 = &tree.createNode<BTAction_Pursue>();
        // auto* sequence1_2 = &tree.createNode<BTNodeSequence_t>(d_1_1, a_s_1);


        // auto* patrol_1 = &tree.createNode<BTAction_Patrol>();
        // auto* sequence1_3 = &tree.createNode<BTNodeSequence_t>(patrol_1);

        // tree.createNode<BTNodeSelector_t>(sequence1_1, sequence1_2, sequence1_3);

        break;
    }
    case 1:
    {
        em.addTag<SnowmanTag>(e);

        // auto* d_a_6 = &tree.createNode<BTDecisionReadyforAttack>();
        // auto* a_a_6 = &tree.createNode<BTActionShoot>(AIComponent::TypeShoot::OneShoottoPlayer); // fail si disparo succes si no disparo
        // auto* d_r_6 = &tree.createNode<BTDecisionOnAttackRadius>();
        // auto* sequence6_1 = &tree.createNode<BTNodeSequence_t>(d_a_6, a_a_6, d_r_6);




        // auto* patrol_6 = &tree.createNode<BTAction_Patrol>();
        // auto* sequence6_3 = &tree.createNode<BTNodeSequence_t>(patrol_6);

        // tree.createNode<BTNodeSelector_t>(sequence6_1, sequence6_2, sequence6_3);

        tree.createNode<BTAction_Patrol>();

        break;
    }

    case 2:
    {
        em.addTag<SlimeTag>(e);

        // auto* d_cl_3 = &tree.createNode<BTDecisionCheckLifes>();

        // auto* d_foc3 = &tree.createNode<BTDecisionFleeorCurePartner>();
        // auto* d_rh3 = &tree.createNode<BTDecisionReadyforHeal>();
        // auto* a_hm3 = &tree.createNode<BTAction_HealMate>();
        // auto* sequence3_0_0 = &tree.createNode<BTNodeSequence_t>(d_foc3, d_rh3, a_hm3);

        // auto* a_f_3 = &tree.createNode<BTAction_Flee>();
        // auto* a_h_3 = &tree.createNode<BTAction_Healing>();
        // auto* sequence3_0_1 = &tree.createNode<BTNodeSequence_t>(a_f_3, a_h_3);

        // auto* selector3_0 = &tree.createNode<BTNodeSelector_t>(sequence3_0_0, sequence3_0_1);

        // auto* sequence3_0 = &tree.createNode<BTNodeSequence_t>(d_cl_3, selector3_0);

        // auto* d_a_3 = &tree.createNode<BTDecisionReadyforAttack>();
        // auto* a_j_3 = &tree.createNode<BTAction_JumptoPlayer>();
        // //auto* a_a_3 = &tree.createNode<BTActionShoot>(AIComponent::TypeShoot::Melee); // fail si disparo succes si no disparo
        // auto* d_r_3 = &tree.createNode<BTDecisionOnAttackRadius>();
        // auto* sequence3_2 = &tree.createNode<BTNodeSequence_t>(d_a_3, a_j_3, d_r_3);

        // auto* d_1_3 = &tree.createNode<BTDecisionPlayerDetected>();
        // auto* a_s_3 = &tree.createNode<BTAction_Seek>();
        // auto* sequence3_3 = &tree.createNode<BTNodeSequence_t>(d_1_3, a_s_3);


        // auto* patrol_3 = &tree.createNode<BTAction_Patrol>();
        // auto* sequence3_4 = &tree.createNode<BTNodeSequence_t>(patrol_3);


        tree.createNode<BTAction_Patrol>();

        //tree.createNode<BTNodeSelector_t>(sequence3_0, sequence3_2, sequence3_3, sequence3_4);
        break;
    }

    case 3:
    {
        em.addTag<BossFinalTag>(e);

        auto* patrol = &tree.createNode<BTAction_Patrol>();

        auto* d_pd = &tree.createNode<BTDecisionPlayerDetected>();
        auto* d_gs = &tree.createNode<BTDecisionSubditosAlreadyGenerated>();
        auto* a_gs = &tree.createNode<BTDAction_GenerateSubditos>();
        auto* sequence = &tree.createNode<BTNodeSequence_t>(d_pd, d_gs, a_gs);

        auto* d_h = &tree.createNode<BTDecisionReadyforHeal>();
        auto* a_hm = &tree.createNode<BTAction_HealMate>();
        auto* sequence1 = &tree.createNode<BTNodeSequence_t>(d_h, a_hm);

        auto* shield = &tree.createNode<BTDecisionActivateorQuit_Shield>();

        auto* d_pd2 = &tree.createNode<BTDecisionPlayerDetected>();
        auto* d_raa = &tree.createNode<BTDecisionReadyforAirAttack>();
        auto* d_as = &tree.createNode<BTActionShoot>(AIComponent::TypeShoot::Air_attack);
        auto* sequence2 = &tree.createNode<BTNodeSequence_t>(d_pd2, d_raa, d_as);

        auto& bb = em.getSingleton<BlackBoard_t>();
        if (bb.boss_fase == 1)
            tree.createNode<BTNodeSelector_t>(sequence, sequence1, patrol);
        else {
            tree.createNode<BTNodeSelector_t>(sequence, sequence1, sequence2, shield, patrol);
        }
        break;
    }
    case 4:
    {
        em.addTag<SpiderTag>(e);
        // auto* d_p_h = &tree.createNode<BTDecisionPlayerHunted>();
        // auto* d_s_1 = &tree.createNode<BTAction_Seek>();
        // auto* a_a_6 = &tree.createNode<BTActionShoot>(AIComponent::TypeShoot::Melee);
        // auto* sequence0 = &tree.createNode<BTNodeSequence_t>(d_p_h, d_s_1, a_a_6);

        // auto* d_ra = &tree.createNode<BTDecisionReadyforAttack>();
        // auto* a_as = &tree.createNode<BTActionShoot>(AIComponent::TypeShoot::OneShoottoPlayer); // fail si disparo succes si no disparo
        // auto* d_or = &tree.createNode<BTDecisionOnAttackRadius>();
        // auto* sequence1 = &tree.createNode<BTNodeSequence_t>(d_ra, a_as, d_or);

        auto* d_1_6 = &tree.createNode<BTDecisionPlayerDetected>();
        auto* a_s_6 = &tree.createNode<BTAction_Seek>();
        // auto* sequence2 = &tree.createNode<BTNodeSequence_t>(d_1_6, a_s_6);
        tree.createNode<BTNodeSequence_t>(d_1_6, a_s_6);

        // auto* patrol_6 = &tree.createNode<BTAction_Patrol>();
        // auto* sequence3 = &tree.createNode<BTNodeSequence_t>(patrol_6);

       // tree.createNode<BTNodeSelector_t>(sequence0, sequence1, sequence2, sequence3);
        //Escribir arbol de araña
    }
    break;
    default:
        break;
    }


    // Creamos el componente IA
    auto& ai = em.addComponent<AIComponent>(e, AIComponent{ .arrival_radius = arrival_radius, .detect_radius = detect_radius, .attack_radius = attack_radius, .tx = tx, .tz = tz,.time2arrive = time2arrive, .tactive = tactive, .perceptionTime = static_cast<float>(perceptionTime),
        .path = path, .countdown_stop = countdown_stop, .countdown_shoot = countdown_shoot, .countdown_perception = countdown_perception, .behaviourTree = &tree });

    em.addComponent<AttackComponent>(e, AttackComponent{ .scale_to_respawn_attack = scale_to_respawn_attack });

    // Revisamos si está en el mapa tutorial para hacer que los enemigos no hagan daño
    auto& li = em.getSingleton<LevelInfo>();
    if (li.mapID == 0)
        em.addTag<NoDamageTag>(e);

    auto& bb = em.getSingleton<BlackBoard_t>();
    if (bb.boss_fase == 2) {
        ai.couldown_spawning = 0.35;
        ai.countdown_heal = 1.0;
        ai.countdown_shield = 0.6;
        ai.countdown_air_attack = 0.85;
    }
}

void Ia_man::resetVec()
{
    vec_t.clear();
}

void Ia_man::createBossFinalFase2(EntityManager& em, const mapType& map) {
    const rapidjson::Value& underworld = map["underworld"];
    const rapidjson::Value& bossfase2 = underworld["bossfinalfase2"];
    for (rapidjson::SizeType i = 0; i < bossfase2.Size(); i++)
    {
        const rapidjson::Value& enemy = bossfase2[i];
        createEnemy(em, enemy);
    }
}


//Generación de subditos
vec3d Ia_man::getRandomPosAroundBoss(double radio, const vec3d& spawnerPos) {
    // Generar un ángulo aleatorio en radianes
    double angle = ((double)rand() / RAND_MAX) * 2 * M_PI;
    // Generar una distancia aleatoria dentro del radio
    double distance = ((double)rand() / RAND_MAX) * radio;
    // Calcular las coordenadas x e y a partir del ángulo y la distancia
    double random_x = cos(angle) * distance;
    double random_z = sin(angle) * distance;
    // Desplazar las coordenadas alrededor del spawner
    double spawn_x = spawnerPos.x();
    double spawn_z = spawnerPos.z();

    // Crear un objeto vec3d con las coordenadas generadas
    vec3d posicion;
    posicion.setX(spawn_x + random_x);
    posicion.setZ(spawn_z + random_z);

    return posicion;
}
void Ia_man::createSubdito(EntityManager& em, double generate_radius) {
    //get boss final position
    auto& bb = em.getSingleton<BlackBoard_t>();
    vec3d const boss_pos = bb.boss_position;
    //create subditos and spawn around boss
    auto& e{ em.newEntity() };
    em.addTag<SubjectTag>(e);
    em.addTag<EnemyTag>(e);

    auto& wr = em.addComponent<RenderComponent>(e, RenderComponent{ .position = getRandomPosAroundBoss(generate_radius,boss_pos), .scale = vec3d{ 1.0,2.0,1.0 }, .color = GRAY });
    auto& wp = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position = wr.position, .max_speed = 0.4 });
    em.addComponent<ColliderComponent>(e, ColliderComponent{ wp.position, wr.scale, BehaviorType::ENEMY });
    em.addComponent<LifeComponent>(e, LifeComponent{ .life = 2 });


    // Creamos el arbol de comportamiento
    vec_t.push_back(std::make_unique<BehaviourTree_t>());
    auto& tree = *vec_t.back();
    // Semilla
    std::srand(static_cast<unsigned int>(std::time(0)));
    // Genera un número aleatorio entre 0 y 99
    int randomNum = std::rand() % 100;
    //aqui el bt
    auto* d_a_1 = &tree.createNode<BTDecisionReadyforAttack>();

    ElementalType type_ele{};
    AIComponent::TypeShoot type_attk{};
    if (randomNum <= 50) {
        type_attk = AIComponent::TypeShoot::Melee;
        type_ele = ElementalType::Fire;
        wp.max_speed = 0.5;
    }
    else {
        type_attk = AIComponent::TypeShoot::OneShoottoPlayer;
        type_ele = ElementalType::Ice;
        wp.max_speed = 0.4;
    }
    auto* a_a_1 = &tree.createNode<BTActionShoot>(type_attk); // fail si disparo succes si no disparo
    auto* d_r_1 = &tree.createNode<BTDecisionOnAttackRadius>();
    auto* sequence1_1 = &tree.createNode<BTNodeSequence_t>(d_a_1, a_a_1, d_r_1);

    auto* d_1_1 = &tree.createNode<BTDecisionPlayerDetected>();


    double attack_radius_p, countdown_shoot_p, countdown_stop_p{};
    if (type_attk == AIComponent::TypeShoot::Melee) {
        auto* a_s_1{ &tree.createNode<BTAction_Pursue>() };
        attack_radius_p = 2.5;
        if (bb.boss_fase == 1) {
            countdown_shoot_p = 0.4;
        }
        else {
            countdown_shoot_p = 0.7;
        }
        countdown_stop_p = 0.8;
        auto* sequence1_2 = &tree.createNode<BTNodeSequence_t>(d_1_1, a_s_1);
        tree.createNode<BTNodeSelector_t>(sequence1_1, sequence1_2);
    }
    else {
        auto* a_s_1{ &tree.createNode<BTAction_Seek>() };
        attack_radius_p = 4.5;
        countdown_shoot_p = 0.2;
        countdown_stop_p = 0.5;
        auto* sequence1_2 = &tree.createNode<BTNodeSequence_t>(d_1_1, a_s_1);
        tree.createNode<BTNodeSelector_t>(sequence1_1, sequence1_2);
    }


    em.addComponent<TypeComponent>(e, TypeComponent{ .type = type_ele });
    em.addComponent<SubjectComponent>(e, SubjectComponent{});

    em.addComponent<AIComponent>(e, AIComponent{ .arrival_radius = 0.1, .detect_radius = 18.0, .attack_radius = attack_radius_p, .tx = 0.0, .tz = 0.0,.time2arrive = 1.0, .tactive = true, .perceptionTime = static_cast<float>(0.2),
    .path = vec3d{}, .countdown_stop = countdown_stop_p, .countdown_shoot = countdown_shoot_p, .countdown_perception = 0.2, .behaviourTree = &tree });

    em.addComponent<AttackComponent>(e, AttackComponent{ .scale_to_respawn_attack = 5.0 });
}
