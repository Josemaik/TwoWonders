#include "life_component.hpp"

void LifeComponent::decreaseLife(){
    if((elapsed >= countdown) && (life > 0)){
        life -= 1;
        elapsed = 0;
    }
}

void LifeComponent::increaseLife(){
    if(life < maxLife)
        life += 1;
}