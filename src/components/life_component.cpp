#include "life_component.hpp"

void LifeComponent::decreaseLife() {
    if ((elapsed >= countdown) && (life > 0)) {
        life -= 1;
        elapsed = 0;
    }
}

void LifeComponent::decreaseLife(int minusLife) {
    if ((elapsed >= countdown) && (life > 0)) {
        life -= minusLife;
        elapsed = 0;
        if (life < 0)
            life = 0;
    }
}

void LifeComponent::increaseLife(int i) {
    if (life < maxLife) {
        life += i;
        if (life > maxLife)
            life = maxLife;
    }
}

void LifeComponent::increaseMaxLife() {
    maxLife += 2;
}
