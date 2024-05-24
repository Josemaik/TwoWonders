#include "Octree.hpp"

std::unordered_set<Octree*> Octree::octreePool_{};

// Función para insertar una entidad en el octree o en sus hijos
void Octree::insert(Entity& entity, ColliderComponent& collider)
{
    if (!divided_)
    {
        if (octEntities_.size() < max_ent_)
        {
            octEntities_.push_back({ &entity, &collider });
            octreePool_.insert(this);
        }
        else
        {
            // Si el octree no está dividido, lo dividimos
            subdivide();

            // Insertamos las entidades en los octantes antes de borrarlas de este nodo
            for (auto& ent : octEntities_)
                insert(*ent.first, *ent.second);
            insert(entity, collider);

            // Borramos las entidades de este nodo
            octEntities_.clear();
        }
    }
    else
    {
        for (auto& octant : octants_)
        {
            if (octant->bounds_.intersects(collider.bbox))
                octant->insert(entity, collider);
        }
    }
}

// Cuando el octree excede su capacidad de entidades, se divide en 8 octantes
void Octree::subdivide()
{
    vec3d size = bounds_.size() / 2.0;
    vec3d center = bounds_.center();
    vec3d octantCenter{};

    for (std::size_t i = 0; i < 8; ++i)
    {
        octantCenter = center + offsets[i] * size;
        BBox octantBounds(octantCenter, size);

        octants_[i] = std::make_unique<Octree>(depth_ + 1, octantBounds, this);
    }

    octreePool_.erase(this);
    divided_ = true;
}

void Octree::clear()
{
    if (divided_)
    {
        for (auto& octant : octants_)
        {
            if (octant)
            {
                octant->clear();
                octant.reset();
            }
        }

        divided_ = false;
    }
    else
        octEntities_.clear();
}

// bool Octree::isBalanced()
// {
//     double base = 8.0;
//     std::size_t entities = countEntities();
//     uint8_t expected = static_cast<uint8_t>(std::ceil(std::log(entities / max_ent_) / std::log(base)));

//     if (std::abs(static_cast<int>(depth_) - expected) > 1)
//         return false;

//     if (divided_)
//     {
//         for (const auto& octant : octants_)
//         {
//             if (!octant->isBalanced())
//                 return false;
//         }
//     }

//     return true;
// }

// void Octree::rebalance()
// {
//     if (!isBalanced())
//     {
//         // Store all entities before clearing the Octree
//         std::vector<std::pair<Entity*, ColliderComponent*>> entities(octEntities_.begin(), octEntities_.end());

//         clear();

//         // Reinsert all entities
//         for (auto& ent : entities)
//             insert(*ent.first, *ent.second);

//         octEntities_.clear();
//     }

//     if (divided_)
//     {
//         for (auto& octant : octants_)
//         {
//             // Only rebalance the octant if it's not balanced
//             if (!octant->isBalanced())
//                 octant->rebalance();
//         }
//     }
// }

// std::size_t Octree::countEntities() const
// {
//     std::size_t count = octEntities_.size();

//     if (divided_)
//     {
//         for (const auto& octant : octants_)
//         {
//             if (octant)
//                 count += octant->countEntities();
//         }
//     }

//     return count;
// }

// Funciones en desuso
//
//
// Función para obtener los vecinos con los que una entidad interacciona fuera de su octante
// std::unordered_set<Octree*>& Octree::getNeighbors(Entity const&, ColliderComponent const&)
// {
//     neighbors.clear();
//     neighbors.insert(this);

//     // Comentar esta parte de la función y luego comentar el break de subdivide, ambos funcionan.
//     // Tal y como lo tenemos el rendimiento es más estable.
//     //
//     // Revisa los hijos del nodo padre y los vecinos del padre y etc etc
//     // if (parent_ != nullptr)
//     //     getParentsRecursive(parent_, entity, collider, neighbors);

//     return neighbors;
// }

// // // Función para buscar los hijos de un nodo específico recursivamente
// void Octree::getChildrenRecursive(Octree* node, Entity const& entity, ColliderComponent const& collider, std::unordered_set<Octree*>& neighbors)
// {
//     for (const auto& octant : node->octants_)
//     {
//         if (octant.get() != this && octant->bounds_.intersects(collider.boundingBox))
//         {
//             if (!octant->octEntities_.empty() && !octant->query(entity, collider))
//                 neighbors.insert(octant.get());
//             else if (octant->divided_)
//                 getChildrenRecursive(octant.get(), entity, collider, neighbors);
//         }
//     }
// }

// // // Función para buscar los padres del nodo padre recursivamente
// void Octree::getParentsRecursive(Octree* node, Entity const& entity, ColliderComponent const& collider, std::unordered_set<Octree*>& neighbors)
// {
//     if (node->parent_ != nullptr)
//     {
//         for (const auto& octant : node->parent_->octants_)
//         {
//             if (octant->bounds_.intersects(collider.boundingBox) && octant.get() != node)
//             {
//                 if (!octant->octEntities_.empty() && !octant->query(entity, collider))
//                     neighbors.insert(octant.get());
//             }
//         }
//         getParentsRecursive(node->parent_, entity, collider, neighbors);
//     }
//     getChildrenRecursive(node, entity, collider, neighbors);
// }

// bool Octree::query(Entity const& entity, ColliderComponent const& collider)
// {
//     if (octEntities_.find({ const_cast<Entity*>(&entity), const_cast<ColliderComponent*>(&collider) }) != octEntities_.end())
//         return true;

//     return false;
// }

// // Contamos las entidades que hay en el octante y en sus hijos
// std::size_t Octree::countEntities() const
// {
//     std::size_t count = octEntities_.size();

//     if (divided_)
//     {
//         for (const auto& octant : octants_)
//         {
//             count += octant->countEntities();
//         }
//     }

//     return count;
// }

// // Función para eliminar una entidad del octree
// void Octree::remove(std::pair<Entity*, ColliderComponent*> const& entity)
// {
//     // Check if the entity is in the current Octree node
//     auto it = octEntities_.find(entity);
//     if (it != octEntities_.end())
//     {
//         octEntities_.erase(it);
//     }
// }
