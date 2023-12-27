#include "ai_component.hpp"

vec3d AIComponent::getRandomDirection(vec3d dir1, vec3d dir2, vec3d dir3) {
    switch (std::rand() % 3) {
        case 0: return dir3; break;  // Derecha
        case 1: return dir2; break;  // Izquierda
        case 2: return dir1; break;  // Abajo
        default: return dir2; break;
    }
}

vec3d AIComponent::getRandomDirectionWithDiagonals(vec3d dir1, vec3d dir2, vec3d dir3, vec3d dir4, vec3d dir5) {
    switch (std::rand() % 5) {
        case 0: return dir3; break;
        case 1: return dir5; break;
        case 2: return dir1; break;
        case 3: return dir2; break;
        case 4: return dir4; break;
        default: return dir2; break;
    }
}

vec3d AIComponent::getRandomDir() {
    switch (std::rand() % 4) {
        case 0: return right; break;   // Derecha
        case 1: return left; break;    // Izquierda
        case 2: return down; break;    // Abajo
        case 3: return up; break;      // Arriba
        default: return right; break;
    }
}

vec3d AIComponent::getRandomDirwithDiagonals() {
    switch (std::rand() % 8) {
        case 0: return right; break;       // Derecha
        case 1: return left; break;        // Izquierda
        case 2: return down; break;        // Abajo
        case 3: return up; break;          // Arriba
        case 4: return up_left; break;     // Up-left
        case 5: return down_right; break;  // Down-right
        case 6: return up_right; break;    // Up-right
        case 7: return down_left; break;   // Down-left
        default: return left; break;
    }
}

bool AIComponent::isInDesiredRange(const vec3d& direction, double xmin, double xmax, double zmin, double zmax) {
    return direction.x() >= xmin && direction.x() <= xmax &&
        direction.z() >= zmin && direction.z() <= zmax;
}

vec3d AIComponent::getRandomPosinRange(double xmin, double xmax, double zmin, double zmax) {
    // Semilla para generar números aleatorios
    std::random_device rd;
    std::mt19937 gen(rd());
    // Creo rangos
    std::uniform_real_distribution<double> rangoX(xmin, xmax);
    std::uniform_real_distribution<double> rangoZ(zmin, zmax);
    // Obtengo x y z aleatoria
    double x = rangoX(gen);
    double z;
    do {
        z = rangoZ(gen);
    } while (z >= -18.0 && z <= -13.0);

    // Devuelvo vector
    return vec3d{ x, 0.0, z };
}
