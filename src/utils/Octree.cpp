#include "Octree.hpp"

// Función para insertar una entidad en el octree o en sus hijos
void Octree::insert(Entity& entity, ColliderComponent& collider)
{
    if (!divided_ && octEntities_.size() < max_ent_)
    {
        octEntities_.insert({ &entity, &collider });
    }
    else if (depth_ < MAX_DEPTH)
    {
        if (!divided_)
            subdivide(entity, collider);
        else
            for (auto& octant : octants_)
            {
                if (octant->bounds_.intersects(collider.boundingBox))
                {
                    octant->insert(entity, collider);
                }
            }
    }
}

// Cuando el octree excede su capacidad de entidades, se divide en 8 octantes
void Octree::subdivide(Entity& entity, ColliderComponent& collider)
{
    vec3d size = bounds_.size() / 2.0;
    vec3d center = bounds_.center();

    for (std::size_t i = 0; i < 8; ++i)
    {
        vec3d octantCenter = center + offsets[i] * size;
        BBox octantBounds(octantCenter, size);
        octants_[i] = std::make_unique<Octree>(depth_ + 1, octantBounds, this);

        for (auto& entity : octEntities_)
        {
            if (octants_[i]->bounds_.intersects(entity.second->boundingBox))
            {
                octants_[i]->insert(*entity.first, *entity.second);
                // break; // Leer el comentario grande de getNeighbors
            }
        }

        if (octants_[i]->bounds_.intersects(collider.boundingBox))
        {
            octants_[i]->insert(entity, collider);
        }
    }

    // Liberamos el espacio del nodo padre
    octEntities_.clear();

    divided_ = true;
}

void Octree::clear()
{
    octEntities_.clear();

    if (divided_)
    {
        for (auto& octant : octants_)
        {
            if (octant != nullptr || !octant->octEntities_.empty() || octant->divided_)
                octant->clear();
        }
    }

    divided_ = false;
}


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