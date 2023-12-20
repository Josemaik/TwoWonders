#include "ai_component.hpp"

vec3f AIComponent::getRandomDirection(vec3f dir1,vec3f dir2,vec3f dir3){
        switch (std::rand() % 3) {
            case 0:  return dir3; break;//derecha
            case 1:  return dir2; break;//izquieda
            case 2:  return dir1; break; //Abajo
            default: return dir2; break;
         }
}
vec3f AIComponent::getRandomDirectionWithDiagonals(vec3f dir1,vec3f dir2,vec3f dir3,vec3f dir4,vec3f dir5){
        switch (std::rand() % 5) {
            case 0:  return dir3; break;
            case 1:  return dir5; break;
            case 2:  return dir1; break; 
            case 3:  return dir2; break; 
            case 4:  return dir4; break;
            default: return dir2; break;
         }
}
  // // Devuelve una dirección aleatoria
vec3f AIComponent::getRandomDir() {
        // Genero direccion aleatoria
        switch (std::rand() % 4) {
        case 0:  return right; break;//derecha
        case 1:  return left; break; //izquieda
        case 2:  return down; break; //Abajo
        case 3:  return up; break; //Arriba
        default: return right; break;
        }
}
vec3f AIComponent::getRandomDirwithDiagonals() {
        // Genero direccion aleatoria
        switch (std::rand() % 8) {
        case 0:  return right;  break; //derecha
        case 1:  return left; break; //izquieda
        case 2:  return down;  break; //Abajo
        case 3:  return up; break; //Arriba
        case 4:  return up_left; break; //up-left
        case 5:  return down_right; break; //down-right
        case 6:  return up_right; break; //up-right
        case 7:  return down_left; break; //down-left
        default: return left; break;
        }
}
bool AIComponent::isInDesiredRange(const vec3f& direction, float xmin, float xmax, float zmin, float zmax) {
        return direction.x() >= xmin && direction.x() <= xmax &&
        direction.z() >= zmin && direction.z() <= zmax;
}
vec3f AIComponent::getRandomPosinRange(float xmin, float xmax, float zmin, float zmax) {
        //Semilla para generar numeros aleatorios
        std::random_device rd;
        std::mt19937 gen(rd());
        // creo rangos
        std::uniform_real_distribution<float> rangoX(xmin, xmax);
        std::uniform_real_distribution<float> rangoZ(zmin, zmax);
        // obtengo x y z aleatoria
        float x = rangoX(gen);
        float z;
        do {
            z = rangoZ(gen);
        } while (z >= -18.0f && z <= -13.0f);

        //devuelvo vector
        return vec3f{ x,0.0f,z };
}