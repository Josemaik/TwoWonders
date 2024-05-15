#include "DSphereTree.hpp"

void DSphereTree::insert(Entity& entity, ColliderComponent& collider) {
    if (!divided_) {
        if (sphereEntities_.size() < max_ent_)
            sphereEntities_.push_back({ &entity, &collider });
        else
            subdivide(entity, collider);
    }
    else if (depth_ < MAX_DEPTH) {
        for (auto& sphere : spheres_) {
            if (sphere && sphere->bounds_.intersects(collider.bbox)) {
                sphere->insert(entity, collider);
            }
        }
    }
}

void DSphereTree::subdivide(Entity& entity, ColliderComponent& collider) {
    sphereEntities_.push_back({ &entity, &collider });

    // Calcula el centroide de los datos
    vec3d centroid{};
    for (const auto& entity : sphereEntities_) {
        centroid += entity.second->bbox.center();
    }
    centroid /= static_cast<double>(sphereEntities_.size());

    // Calcula la varianza a lo largo de cada eje
    vec3d variance{};
    for (const auto& entity : sphereEntities_) {
        vec3d diff = entity.second->bbox.center() - centroid;
        variance += diff * diff;
    }

    // Encuentra el eje de mayor varianza
    std::size_t axis = std::max_element(variance.begin(), variance.end()) - variance.begin();

    // Ordena los datos a lo largo de ese eje
    std::sort(sphereEntities_.begin(), sphereEntities_.end(),
        [axis](const auto& a, const auto& b) {
        return a.second->bbox.center()[axis] < b.second->bbox.center()[axis];
    });

    // Divide los datos en dos a lo largo de ese eje
    std::size_t mid = sphereEntities_.size() / 2;
    SphereMap left(sphereEntities_.begin(), sphereEntities_.begin() + mid);
    SphereMap right(sphereEntities_.begin() + mid, sphereEntities_.end());

    std::vector<vec3d> leftCenters{}, rightCenters{};
    for (const auto& entity : left) {
        leftCenters.push_back(entity.second->bbox.center());
    }
    for (const auto& entity : right) {
        rightCenters.push_back(entity.second->bbox.center());
    }

    // Liberamos el espacio del nodo padre
    sphereEntities_.clear();
    divided_ = true;

    // Crea las nuevas esferas
    spheres_[0] = std::make_unique<DSphereTree>(depth_ + 1, Sphere::boundingSphere(leftCenters), this);
    spheres_[1] = std::make_unique<DSphereTree>(depth_ + 1, Sphere::boundingSphere(rightCenters), this);

    // Inserta las entidades en las nuevas esferas
    for (const auto& entity : left) {
        spheres_[0]->insert(*entity.first, *entity.second);
    }
    for (const auto& entity : right) {
        spheres_[1]->insert(*entity.first, *entity.second);
    }
}

void DSphereTree::clear() {
    if (divided_) {
        for (auto& sphere : spheres_) {
            if (sphere)
                sphere->clear();
        }

        divided_ = false;
    }
    else
        sphereEntities_.clear();
}