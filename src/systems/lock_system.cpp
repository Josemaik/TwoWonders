#include "lock_system.hpp"

void LockSystem::update(EntityManager& em)
{
    auto& li = em.getSingleton<LevelInfo>();
    auto& player = *em.getEntityByID(li.playerID);

    if (player.hasComponent<PhysicsComponent>())
    {
        auto& playerPhy = em.getComponent<PhysicsComponent>(player);
        auto& playerPos = playerPhy.position;
        enemies.clear();

        em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, PhysicsComponent& phy)
        {
            auto& pos = phy.position;

            // Calcula la distancia entre la posición del jugador y la posición del enemigo
            double distance = std::sqrt(std::pow(playerPos.x() - pos.x(), 2) + std::pow(playerPos.y() - pos.y(), 2) + std::pow(playerPos.z() - pos.z(), 2));

            // Si el enemigo se encuentra a menos de 10 unidades de distancia del jugador se inserta en el set
            if (distance < 10.0)
                enemies.push_back({ e.getID(), distance });

        });

        // Ordenar de menor a mayor
        std::sort(enemies.begin(), enemies.end(), [](const auto& a, const auto& b) {
            return a.second < b.second;
        });

        if (li.lockInput)
        {
            if (enemies.empty())
            {
                li.lockedEnemy = li.max;
                li.closestEnemy = li.max;
                li.lockInput = false;
            }
            else
            {
                if (li.lockedEnemy == li.max)
                {
                    li.lockedEnemy = enemies[0].first;
                    li.closestEnemy = li.max;
                }

                auto& enemy = *em.getEntityByID(li.lockedEnemy);

                if (!enemy.hasTag<EnemyTag>())
                {
                    li.lockedEnemy = li.max;
                    li.lockInput = false;
                    return;
                }

                auto& lockedEnemyPos = em.getComponent<PhysicsComponent>(enemy).position;
                playerPhy.orientation = std::atan2(lockedEnemyPos.x() - playerPos.x(), lockedEnemyPos.z() - playerPos.z());
            }
        }
        else
        {
            li.closestEnemy = enemies.empty() ? li.max : enemies[0].first;

            if (li.lockedEnemy != li.max)
                li.lockedEnemy = li.max;
        }
    }
}

// if (li.lockedEnemy != nullptr && !enemies.empty())
// {
//     for (size_t i = 0; i < enemies.size(); ++i)
//     {
//         if (em.getEntityByID(enemies[i].first) == li.lockedEnemy)
//         {
//             // Si es el último enemigo, vuelve al inicio. Si no, avanza una posición.
//             li.lockedEnemy = em.getEntityByID(enemies[(i + 1) % enemies.size()].first);
//             break;
//         }
//     }
// }