#include "Octree.hpp"

// Función para insertar una entidad en el octree o en sus hijos
void Octree::insert(Entity& entity, ColliderComponent& collider)
{
    //std::size_t s = countEntities();
    if (!divided && entities.size() < max_ent)
    {
        entities.push_back({ &entity, &collider });
    }
    else if (depth < MAX_DEPTH)
    {
        if (!divided)
        {
            subdivide();
            divided = true;
        }

        for (auto& octant : octants)
        {
            if (octant->bounds.intersects(collider.boundingBox))
            {
                octant->insert(entity, collider);
            }
        }

    }
}

// Cuando el octree excede su capacidad de entidades, se divide en 8 octantes
void Octree::subdivide()
{
    vec3f size = bounds.size() / 2.0f;
    vec3f center = bounds.center();

    std::array<vec3f, 8> offsets =
    {
        vec3f(-0.5f, -0.5f, -0.5f),
        vec3f(0.5f, -0.5f, -0.5f),
        vec3f(-0.5f, 0.5f, -0.5f),
        vec3f(0.5f, 0.5f, -0.5f),
        vec3f(-0.5f, -0.5f, 0.5f),
        vec3f(0.5f, -0.5f, 0.5f),
        vec3f(-0.5f, 0.5f, 0.5f),
        vec3f(0.5f, 0.5f, 0.5f)
    };

    for (int i = 0; i < 8; ++i)
    {
        vec3f octantCenter = center + offsets[i] * size;
        BBox octantBounds(octantCenter, size);
        octants[i] = std::make_unique<Octree>(depth + 1, octantBounds, this);
    }

    // // Redistribute existing entities to new octants
    OctVector toRemove;
    for (auto& entity : entities)
    {
        for (auto& octant : octants)
        {
            if (octant->bounds.intersects(entity.second->boundingBox))
            {
                octant->insert(*entity.first, *entity.second);
                toRemove.push_back(entity);
                break;
            }
        }
    }

    // Remove entities that have been redistributed
    for (auto& entity : toRemove)
    {
        remove(entity);
    }

    divided = true;
}

// Función para obtener los vecinos con los que una entidad interacciona fuera de su octante
std::vector<Octree*> Octree::getNeighbors(ColliderComponent const& collider)
{
    std::vector<Octree*> neighbors;
    std::set<std::size_t> removeFromNeighbors;

    // Check the parent node and its ancestors
    getParentsRecursive(parent, collider, neighbors);

    // Check the children of the neighboring nodes
    std::vector<Octree*> newNeighbors = neighbors;
    for (std::size_t i = 0; i < newNeighbors.size(); ++i)
    {
        getChildrenRecursive(newNeighbors[i], collider, newNeighbors, removeFromNeighbors, i);
    }

    // Add new neighbors to the neighbors vector
    neighbors = newNeighbors;

    // Remove octants that have no entities
    for (auto i = removeFromNeighbors.rbegin(); i != removeFromNeighbors.rend(); ++i)
    {
        neighbors.erase(neighbors.begin() + *i);
    }

    return neighbors;
}

// Función para buscar los hijos de un nodo específico recursivamente
void Octree::getChildrenRecursive(Octree* node, ColliderComponent const& collider, std::vector<Octree*>& neighbors, std::set<std::size_t>& removeFromNeighbors, std::size_t position)
{
    if (node->divided)
    {
        removeFromNeighbors.insert(position);

        for (const auto& octant : node->octants)
        {
            if (octant->bounds.intersects(collider.boundingBox) && octant.get() != this)
            {
                neighbors.push_back(octant.get());
                getChildrenRecursive(octant.get(), collider, neighbors, removeFromNeighbors, neighbors.size() - 1);
            }
        }
    }
    else if (node->entities.empty())
    {
        removeFromNeighbors.insert(position);
    }
}

// Función para buscar los padres del nodo padre recursivamente
void Octree::getParentsRecursive(Octree* node, ColliderComponent const& collider, std::vector<Octree*>& neighbors)
{
    if (node != nullptr)
    {
        neighbors.push_back(node);

        if (node->parent != nullptr)
        {
            for (const auto& octant : node->parent->octants)
            {
                if (octant->bounds.intersects(collider.boundingBox) && octant.get() != node)
                {
                    neighbors.push_back(octant.get());
                }
            }
        }

        getParentsRecursive(node->parent, collider, neighbors);
    }
}

// Función defectuosa
Octree::OctVector Octree::query(const BBox& box)
{
    Octree::OctVector found{};

    // Revisar si la entidad se encuentra en el nodo actual
    if (!bounds.intersects(box))
        return found;

    // Si está dividido, buscar en los octantes
    if (divided)
    {
        for (const auto& octant : octants)
        {
            auto octantFound = octant->query(box);
            found.insert(found.end(), octantFound.begin(), octantFound.end());
        }
    }
    else
    {
        // Si no está dividido, buscar en las entidades
        for (const auto& entity : entities)
        {
            if (box.intersects(entity.second->boundingBox))
            {
                found.push_back(entity);
            }
        }
    }

    return found;
}

// Contamos las entidades que hay en el octante y en sus hijos
std::size_t Octree::countEntities() const
{
    std::size_t count = entities.size();

    if (divided)
    {
        for (const auto& octant : octants)
        {
            count += octant->countEntities();
        }
    }

    return count;
}

// Función para recombinar hijos del cotree - no usada
void Octree::recombine()
{
    if (divided)
    {
        for (auto& octant : octants)
        {
            auto& octantEntities = octant->entities;
            entities.insert(entities.end(), octantEntities.begin(), octantEntities.end());
            octant.reset();
        }

        divided = false;
    }
}

// Función para eliminar una entidad del octree
void Octree::remove(std::pair<Entity*, ColliderComponent*>& entity)
{
    // Check if the entity is in the current Octree node
    for (auto it = entities.begin(); it != entities.end(); ++it)
    {
        if (it->first == entity.first && it->second == entity.second)
        {
            entities.erase(it);
            return;
        }
    }
}