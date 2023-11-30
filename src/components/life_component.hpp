#pragma once

struct LifeComponent
{
    int life{}, countdown{};

    void decreaseLife(){
        if(countdown < 0){
            life -= 1;
            countdown = 30;
        }
    }

    void decreaseCountdown(){
        countdown -= 1;
    }
};