#include "life_component.hpp"

void LifeComponent::decreaseLife() {
    if ((elapsed >= countdown) && (life > 0)) {
        lifeLost = 1;
        life -= 1;
        elapsed = 0;
    }
}

void LifeComponent::decreaseLife(int minusLife) {
    if ((elapsed >= countdown) && (life > 0)) {
        lifeLost = minusLife;
        life -= minusLife;
        elapsed = 0;
        if (life < 0)
            life = 0;
    }
}

void LifeComponent::increaseLife(int i) {
    if (life < (maxLife + maxLifeAdd)) {
        life += i;
        if (life > (maxLife + maxLifeAdd))
            life = (maxLife + maxLifeAdd);
    }
}

void LifeComponent::increaseMaxLife() {
    maxLife += 2;
}