#pragma once
#include <cstdint>
#include <chrono>
struct GameTimer
{
    using frametype = std::chrono::high_resolution_clock::time_point;
    using deltatype = std::chrono::duration<double>;
    static  const uint8_t current_fps = 60;
    void setcurrentframetime(){
        currentFrameTime = std::chrono::high_resolution_clock::now();
    }
    void setlastframetime(){
        lastFrameTime = std::chrono::high_resolution_clock::now();
    };
    void setdeltatime(){
        deltaTime = std::chrono::duration_cast<std::chrono::duration<double>>(currentFrameTime - lastFrameTime);
    }
    void setElapsedTime(){
        elapsedTime = deltaTime.count();
    }
    frametype& getLastframetime(){
        return lastFrameTime;
    }
    frametype& getcurrentframetime(){
        return currentFrameTime;
    }
    double& getelapsedtime(){
        return elapsedTime;
    }
    const double& getphysicsUpdateTimeStep(){
        return physicsUpdateTimeStep;
    }
private:
    const double physicsUpdateTimeStep = 1.0 / current_fps;
    frametype lastFrameTime;
    frametype currentFrameTime;
    deltatype deltaTime;  
    double elapsedTime;
};
