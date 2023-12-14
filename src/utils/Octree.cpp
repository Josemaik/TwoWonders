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
        {
            subdivide();
            divided_ = true;
        }

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
void Octree::subdivide()
{
    vec3f size = bounds_.size() / 2.0f;
    vec3f center = bounds_.center();

    for (int i = 0; i < 8; ++i)
    {
        vec3f octantCenter = center + offsets[i] * size;
        BBox octantBounds(octantCenter, size);
        octants_[i] = std::make_unique<Octree>(depth_ + 1, octantBounds, this);
    }

    // Redistribute existing entities to new octants
    OctSet toRemove;
    for (auto& entity : octEntities_)
    {
        for (auto& octant : octants_)
        {
            if (octant->bounds_.intersects(entity.second->boundingBox))
            {
                octant->insert(*entity.first, *entity.second);
                toRemove.insert(entity);
                break;
            }
        }
    }

    // Remove entities that have been redistributed
    for (auto const& entity : toRemove)
    {
        remove(entity);
    }

    divided_ = true;
}

// Función para obtener los vecinos con los que una entidad interacciona fuera de su octante
std::unordered_set<Octree*> Octree::getNeighbors(ColliderComponent const& collider)
{
    std::unordered_set<Octree*> neighbors;

    // Check the parent node and its ancestors
    getParentsRecursive(parent_, collider, neighbors);

    return neighbors;
}

// Función para buscar los hijos de un nodo específico recursivamente
void Octree::getChildrenRecursive(Octree* node, ColliderComponent const& collider, std::unordered_set<Octree*>& neighbors)
{
    for (const auto& octant : node->octants_)
    {
        if (octant.get() != this && octant->bounds_.intersects(collider.boundingBox))
        {
            if (!octant->octEntities_.empty())
                neighbors.insert(octant.get());
            else if (octant->divided_)
                getChildrenRecursive(octant.get(), collider, neighbors);
        }
    }
}

// Función para buscar los padres del nodo padre recursivamente
void Octree::getParentsRecursive(Octree* node, ColliderComponent const& collider, std::unordered_set<Octree*>& neighbors)
{
    if (node != nullptr)
    {
        if (node->parent_ != nullptr)
        {
            for (const auto& octant : node->parent_->octants_)
            {
                if (octant->bounds_.intersects(collider.boundingBox) && octant.get() != node)
                {
                    if (!octant->octEntities_.empty())
                        neighbors.insert(octant.get());
                }
            }
            getParentsRecursive(node->parent_, collider, neighbors);
        }
        getChildrenRecursive(node, collider, neighbors);
    }
}

// Contamos las entidades que hay en el octante y en sus hijos
std::size_t Octree::countEntities() const
{
    std::size_t count = octEntities_.size();

    if (divided_)
    {
        for (const auto& octant : octants_)
        {
            count += octant->countEntities();
        }
    }

    return count;
}

// Función para eliminar una entidad del octree
void Octree::remove(std::pair<Entity*, ColliderComponent*> const& entity)
{
    // Check if the entity is in the current Octree node
    for (auto it = octEntities_.begin(); it != octEntities_.end(); ++it)
    {
        if (it->first == entity.first && it->second == entity.second)
        {
            octEntities_.erase(it);
            return;
        }
    }
}