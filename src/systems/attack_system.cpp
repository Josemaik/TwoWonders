#include "attack_system.hpp"

void AttackSystem::update(EntityManager& em, AttackManager& am) {

    auto& frti = em.getSingleton<FrustumInfo>();
    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, AttackerComponent& att)
    {
        if (!frti.inFrustum(e.getID()))
            return;

        if (att.createAttack)
        {
            // Para asegurarme que no usa ninguna velocidad anterior
            if (att.vel == vec3d::zero())
                am.createAttackType(em, e, att.type);
            else
                am.createAttackType(em, e, att.type, att.vel);

            att.createAttack = false;
            att.type = AttackType::None;
        }

        att.decreaseCountdown(timeStep, att.elapsed);
    });

    using SYSCMPs = MP::TypeList<AttackComponent>;
    em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, AttackComponent& att)
    {
        if (att.lifeTime <= 0)
        {
            auto& li = em.getSingleton<LevelInfo>();
            li.insertDeath(e.getID());

            if (att.atkType & AttackType::HealSpellSetup)
                att.doEffect = true;
        }
        else
            att.lifeTime -= timeStep;

        if (att.doEffect)
        {
            attackMap[att.atkType](em, e, att, am);
            att.doEffect = false;
        }

        if (e.hasComponent<ProjectileComponent>())
        {
            auto& li = em.getSingleton<LevelInfo>();
            auto& pro = em.getComponent<ProjectileComponent>(e);
            if (pro.checkRange(timeStep)) {

                if (pro.startedFalling)
                {
                    li.insertDeath(e.getID());
                    return;
                }

                if (e.hasComponent<PhysicsComponent>())
                {
                    auto& phy = em.getComponent<PhysicsComponent>(e);
                    phy.gravity = .1f;
                    pro.startedFalling = true;
                    pro.range = 2.0f;
                    pro.elapsed = 0.0f;
                }
            }
        }
    });
}


void AttackSystem::resolvePlayerDirection(PhysicsComponent& playerPhy, PhysicsComponent& enemyPhy, bool)
{
    auto& pos = playerPhy.position;
    auto& otherPos = enemyPhy.position;
    auto& enemyVel = enemyPhy.velocity;
    vec3d dir = { pos.x() - otherPos.x(), 0, pos.z() - otherPos.z() };
    int multiplier = 7;

    dir.normalize();

    // if (isEnemy)
    // {
        // Si la dirección del enemigo y la del jugador tienen menos de 20 grados de diferencia, el jugador se le suman 45º en la dirección contraria
    vec3d dirEnemy = { otherPos.x() - enemyVel.x(), 0, otherPos.z() - enemyVel.z() };
    dirEnemy.normalize();

    // Calcular el ángulo entre las dos direcciones
    double dot = dir.x() * dirEnemy.x() + dir.z() * dirEnemy.z(); // producto punto
    double det = dir.x() * dirEnemy.z() - dir.z() * dirEnemy.x(); // determinante
    double angle = atan2(det, dot); // atan2(y, x) o atan2(sin, cos)

    // Convertir el ángulo a grados
    double angleDeg = angle * 180 / K_PI;

    // Si el ángulo es menor de 20 grados, ajustar la dirección del jugador
    if (std::abs(angleDeg) < 20)
    {
        // Rotar la dirección del jugador 45 grados en la dirección contraria
        double angleRad = -135 * K_PI / 180; // Convertir a radianes
        double cosAngle = cos(angleRad);
        double sinAngle = sin(angleRad);
        vec3d newDir = { dir.x() * cosAngle - dir.z() * sinAngle, 0.0, dir.x() * sinAngle + dir.z() * cosAngle };

        dir = newDir;
    }
    // }
    // else
    //     playerPhy.gravity = playerPhy.gravity / 1.5;

    playerPhy.velocity = dir * multiplier;
    playerPhy.stopped = true;
}