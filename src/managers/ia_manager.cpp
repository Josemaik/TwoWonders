#include "ia_manager.hpp"


void Ia_man::createEnemy(EntityManager& em, jsonType json)
{
    // Extraemos los datos del .kaiwa
    vec3d position = { json["position"][0].GetDouble(), json["position"][1].GetDouble(), json["position"][2].GetDouble() };
    vec3d rotationVec{ json["rotVector"][1].GetDouble(), json["rotVector"][2].GetDouble(), json["rotVector"][0].GetDouble() };
    double orientation{ json["rotation"].GetDouble() };
    vec3d scale = { json["scale"][0].GetDouble(), json["scale"][1].GetDouble(), json["scale"][2].GetDouble() };
    Color color = { static_cast<unsigned char>(json["color"][0].GetUint()), static_cast<unsigned char>(json["color"][1].GetUint()), static_cast<unsigned char>(json["color"][2].GetUint()), static_cast<unsigned char>(json["color"][3].GetUint()) };
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

    //Comprobamos si la IA patrulla o no
    bool followpatrol{ true };
    vec3d firstpathpos = { json["path"][0][0].GetDouble(), json["path"][0][1].GetDouble(), json["path"][0][2].GetDouble() };
    if (firstpathpos.x() == position.x() && firstpathpos.z() == position.z()) {
        followpatrol = false;
    }


    double countdown_stop = json["countdown_stop"].GetDouble();
    double countdown_shoot = json["countdown_shoot"].GetDouble();
    double countdown_perception = json["countdown_perception"].GetDouble();

    double rot = orientation * DEGTORAD;

    // Creamos el enemigo
    auto& e{ em.newEntity() };
    em.addTag<EnemyTag>(e);
    em.addTag<LockableTag>(e);

    auto& wr = em.addComponent<RenderComponent>(e, RenderComponent{ .position = position, .scale = scale, .color = color,.orientation = rot,.rotationVec = rotationVec });
    auto& wp = em.addComponent<PhysicsComponent>(e, PhysicsComponent{ .position = wr.position, .scale = wr.scale,.orientation = rot,.orientationonrespawn = rot,.rotationVec = rotationVec, .max_speed = max_speed });
    em.addComponent<ColliderComponent>(e, ColliderComponent{ wp.position, wr.scale, BehaviorType::ENEMY });
    auto& wl = em.addComponent<LifeComponent>(e, LifeComponent{ .life = life });
    em.addComponent<TypeComponent>(e, TypeComponent{ .type = element });
    em.addComponent<SoundComponent>(e);

    // Creamos el arbol de comportamiento para cada tipo de IA
    vec_t.push_back(std::make_unique<BehaviourTree_t>());
    auto& tree = *vec_t.back();

    switch (type)
    {
    case 0:
    {
        em.addTag<GolemTag>(e);

        auto* d_a_1 = &tree.createNode<BTDecisionReadyforAttack>();
        auto* a_a_1 = &tree.createNode<BTActionShoot>(AIComponent::TypeShoot::Melee, vec3d{}); // fail si disparo succes si no disparo
        auto* d_r_1 = &tree.createNode<BTDecisionOnAttackRadius>();
        auto* sequence1_1 = &tree.createNode<BTNodeSequence_t>(d_a_1, a_a_1, d_r_1);

        auto* d_1_1 = &tree.createNode<BTDecisionPlayerDetected>();
        auto* a_s_1 = &tree.createNode<BTAction_Seek>();
        auto* sequence1_2 = &tree.createNode<BTNodeSequence_t>(d_1_1, a_s_1);


        auto* patrol_1 = &tree.createNode<BTAction_Patrol>();
        auto* sequence1_3 = &tree.createNode<BTNodeSequence_t>(patrol_1);

        tree.createNode<BTNodeSelector_t>(sequence1_1, sequence1_2, sequence1_3);

        break;
    }
    case 1:
    {
        em.addTag<SnowmanTag>(e);
        em.addComponent<AnimationComponent>(e);
        auto* a_e_1 = &tree.createNode<BTAction_Evade>();
        auto* d_a_6 = &tree.createNode<BTDecisionReadyforAttack>();
        auto* a_a_6 = &tree.createNode<BTActionShoot>(AIComponent::TypeShoot::OneShoottoPlayer, vec3d{}); // fail si disparo succes si no disparo
        auto* d_r_6 = &tree.createNode<BTDecisionOnAttackRadius>();
        auto* sequence6_1 = &tree.createNode<BTNodeSequence_t>(a_e_1, d_a_6, a_a_6, d_r_6);

        auto* d_1_2 = &tree.createNode<BTDecisionPlayerDetected>();
        auto* a_s_2 = &tree.createNode<BTAction_Seek>();
        auto* sequence6_2 = &tree.createNode<BTNodeSequence_t>(d_1_2, a_s_2);

        auto* patrol_6 = &tree.createNode<BTAction_Patrol>();
        auto* sequence6_3 = &tree.createNode<BTNodeSequence_t>(patrol_6);

        tree.createNode<BTNodeSelector_t>(sequence6_1, sequence6_2, sequence6_3);
        break;
    }

    case 2:
    {
        em.addTag<SlimeTag>(e);

        auto* d_cl_3 = &tree.createNode<BTDecisionCheckLifes>();

        auto* d_foc3 = &tree.createNode<BTDecisionFleeorCurePartner>();
        auto* d_rh3 = &tree.createNode<BTDecisionReadyforHeal>();
        auto* a_hm3 = &tree.createNode<BTAction_HealMate>();
        auto* sequence3_0_0 = &tree.createNode<BTNodeSequence_t>(d_foc3, d_rh3, a_hm3);

        auto* a_f_3 = &tree.createNode<BTAction_Flee>();
        auto* a_h_3 = &tree.createNode<BTAction_Healing>();
        auto* sequence3_0_1 = &tree.createNode<BTNodeSequence_t>(a_f_3, a_h_3);

        auto* selector3_0 = &tree.createNode<BTNodeSelector_t>(sequence3_0_0, sequence3_0_1);

        auto* sequence3_0 = &tree.createNode<BTNodeSequence_t>(d_cl_3, selector3_0);

        auto* d_a_3 = &tree.createNode<BTDecisionReadyforAttack>();
        auto* a_j_3 = &tree.createNode<BTAction_JumptoPlayer>();
        auto* a_a_3 = &tree.createNode<BTActionShoot>(AIComponent::TypeShoot::Melee, vec3d{}); // fail si disparo succes si no disparo
        auto* d_r_3 = &tree.createNode<BTDecisionOnAttackRadius>();
        auto* sequence3_2 = &tree.createNode<BTNodeSequence_t>(d_a_3, a_j_3, a_a_3, d_r_3);

        auto* d_1_3 = &tree.createNode<BTDecisionPlayerDetected>();
        auto* a_s_3 = &tree.createNode<BTAction_Seek>();
        auto* sequence3_3 = &tree.createNode<BTNodeSequence_t>(d_1_3, a_s_3);


        auto* patrol_3 = &tree.createNode<BTAction_Patrol>();
        auto* sequence3_4 = &tree.createNode<BTNodeSequence_t>(patrol_3);


        tree.createNode<BTNodeSelector_t>(sequence3_0, sequence3_2, sequence3_3, sequence3_4);
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
        auto* d_as = &tree.createNode<BTActionShoot>(AIComponent::TypeShoot::Air_attack, vec3d{});
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
        auto* d_p_h = &tree.createNode<BTDecisionPlayerHunted>();
        auto* d_s_1 = &tree.createNode<BTAction_Seek>();
        auto* d_rax = &tree.createNode<BTDecisionReadyforAttack>();
        auto* a_a_6 = &tree.createNode<BTActionShoot>(AIComponent::TypeShoot::Melee, vec3d{});
        auto* sequence0 = &tree.createNode<BTNodeSequence_t>(d_p_h, d_s_1, d_rax, a_a_6);

        auto* d_1_7 = &tree.createNode<BTDecisionPlayerDetected>();
        auto* d_ra = &tree.createNode<BTDecisionReadyforAttack>();
        auto* a_as = &tree.createNode<BTActionShoot>(AIComponent::TypeShoot::OneShoottoPlayer, vec3d{}); // fail si disparo succes si no disparo
        auto* d_or = &tree.createNode<BTDecisionOnAttackRadius>();
        auto* sequence1 = &tree.createNode<BTNodeSequence_t>(d_1_7, d_ra, a_as, d_or);

        auto* d_1_6 = &tree.createNode<BTDecisionPlayerDetected>();
        auto* a_s_6 = &tree.createNode<BTAction_Seek>();
        auto* sequence2 = &tree.createNode<BTNodeSequence_t>(d_1_6, a_s_6);
        tree.createNode<BTNodeSequence_t>(d_1_6, a_s_6);

        auto* patrol_6 = &tree.createNode<BTAction_Patrol>();
        auto* sequence3 = &tree.createNode<BTNodeSequence_t>(patrol_6);

        tree.createNode<BTNodeSelector_t>(sequence0, sequence1, sequence2, sequence3);
    }
    break;
    case 5: {
        em.addTag<AngryBushTag>(e);
        em.destroyTag<LockableTag>(e);
        auto& abc = em.addComponent<AngryBushComponent>(e);
        em.addComponent<AnimationComponent>(e);
        abc.max_speed = wp.max_speed;
        wl.invulnerable = true;
        tree.createNode<BTAction_Pendulum>();
    }
          break;
    case 6:
        em.addTag<DummyTag>(e);
        break;
    case 7:
        em.addTag<AngryBushTag2>(e);
        em.destroyTag<LockableTag>(e);
        em.addComponent<AnimationComponent>(e);
        tree.createNode<BTAction_Patrol>();
        break;
    case 8: {
        em.addTag<CrusherTag>(e);
        em.addComponent<AnimationComponent>(e);
        auto* pdc = &tree.createNode<BTDecisionPlayerDetected>();
        auto* rfca = &tree.createNode<BTDecisionReadyforCrusherAttk>();
        auto* ash = &tree.createNode<BTActionShoot>(AIComponent::TypeShoot::CrusherAttack, wp.position);
        tree.createNode<BTNodeSequence_t>(pdc, rfca, ash);
    }
          break;
    default:
        break;
    }


    // Creamos el componente IA
    auto& ai = em.addComponent<AIComponent>(e, AIComponent{ .arrival_radius = arrival_radius, .detect_radius = detect_radius, .attack_radius = attack_radius, .tx = tx, .tz = tz,.time2arrive = time2arrive, .tactive = tactive, .perceptionTime = static_cast<float>(perceptionTime),
        .path = path,.followpatrol = followpatrol ,.countdown_stop = countdown_stop, .countdown_shoot = countdown_shoot, .countdown_perception = countdown_perception, .initialPos = position, .behaviourTree = &tree });

    em.addComponent<AttackerComponent>(e);

    // Revisamos si está en el mapa tutorial para hacer que los enemigos no hagan daño
    auto& li = em.getSingleton<LevelInfo>();
    if (li.mapID == 0)
        em.addTag<NoDamageTag>(e);
    //Si estamos en la fase 2 del boss final especificar countdowns
    auto& bb = em.getSingleton<BlackBoard_t>();
    if (bb.boss_fase == 2) {
        ai.couldown_spawning = 0.35;
        ai.countdown_heal = 1.0;
        ai.countdown_shield = 0.6;
        ai.countdown_air_attack = 0.85;
    }
}

//Borrar vector de bt
void Ia_man::resetVec()
{
    vec_t.clear();
}

//Crear fase 2 del boss final
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
    double angle = ((double)std::rand() / RAND_MAX) * 2 * K_PI;
    // Generar una distancia aleatoria dentro del radio
    double distance = ((double)std::rand() / RAND_MAX) * radio;
    // Calcular las coordenadas x e y a partir del ángulo y la distancia
    double random_x = std::cos(angle) * distance;
    double random_z = std::sin(angle) * distance;
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

    auto& wr = em.addComponent<RenderComponent>(e, RenderComponent{ .position = getRandomPosAroundBoss(generate_radius,boss_pos), .scale = vec3d{ 1.0,2.0,1.0 }, .color = D_GRAY });
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
    auto* a_a_1 = &tree.createNode<BTActionShoot>(type_attk, vec3d{}); // fail si disparo succes si no disparo
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

    em.addComponent<AttackerComponent>(e);
}
