#pragma once
#include <vector>
#include <chrono>
#include <cereal/types/vector.hpp>
#include <cereal/types/chrono.hpp>
#include <cereal/archives/json.hpp>
#include "../vec3D.hpp"

struct InputEvent
{
    enum class Type
    {
        None,
        AttackKeyDown,
        InteractKeyPressed,
        InteractKeyReleased,
        EnterReleased,
        EscapeReleased,
        InventoryReleased,
        LockInReleased,
        ChangeSpellReleased,
    };

    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(type, timeStamp);
    }

    Type type{};
    std::chrono::duration<double> timeStamp{};
};

struct MovementEvent
{
    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(vel, timeStamp);
    }

    vec3d vel{};
    std::chrono::duration<double> timeStamp{};
};

struct GameData
{
    void addInputEvent(InputEvent::Type type, std::chrono::duration<double> timestamp)
    {
        inputEvents.push_back({ type, timestamp });
    }

    void addMovementEvent(vec3d& vel, std::chrono::duration<double> timeStamp)
    {
        if (!movementEvents.empty() && movementEvents.back().vel != vel)
            movementEvents.push_back({ vel, timeStamp });
        else if (movementEvents.empty())
            movementEvents.push_back({ vel, timeStamp });
    }

    void setRandomSeed(unsigned int seed)
    {
        randomSeed = seed;
    }

    void setStartTime(std::chrono::_V2::system_clock::time_point& time)
    {
        startTime = time;
    }

    void setFinalTime(std::chrono::duration<float, std::milli>& time)
    {
        finalTime = time;
    }

    std::chrono::duration<float, std::milli> getTime()
    {
        return std::chrono::duration<float, std::milli>(std::chrono::system_clock::now() - startTime);
    }

    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(randomSeed, inputEvents, movementEvents, finalTime);
    }

    unsigned int getRandomSeed() const
    {
        return randomSeed;
    }

    const std::vector<InputEvent>& getInputEvents() const
    {
        return inputEvents;
    }

    const std::vector<MovementEvent>& getMovementEvents() const
    {
        return movementEvents;
    }

    const InputEvent& getCurrentInput() const
    {
        return currentInput;
    }

    const MovementEvent& getCurrentMovement() const
    {
        return currentMovement;
    }

    void update()
    {
        updateCurrentInput();
        updateCurrentMovement();
    }

    bool isKeyDown(int key)
    {
        auto time = getTime();
        if (currentInput.timeStamp < time && time < finalTime)
            switch (currentInput.type)
            {
            case InputEvent::Type::AttackKeyDown:
            {
                if (key == 32 || key == 7)
                    return true;
                break;
            }
            default:
                return false;
            }

        return false;
    }

    bool isKeyPressed(int key)
    {
        auto time = getTime();
        if (currentInput.timeStamp < time && time < finalTime)
            switch (currentInput.type)
            {
            case InputEvent::Type::InteractKeyPressed:
            {
                if (key == 69 || key == 7)
                    return true;
                break;
            }
            default:
                return false;
            }

        return false;
    }

    bool isKeyReleased(int key)
    {
        auto time = getTime();
        if (currentInput.timeStamp < time && time < finalTime)
            switch (currentInput.type)
            {
            case InputEvent::Type::EnterReleased:
            {
                if (key == 257)
                    return true;
                break;
            }
            case InputEvent::Type::EscapeReleased:
            {
                if (key == 256 || key == 15)
                    return true;
                break;
            }
            case InputEvent::Type::InventoryReleased:
            {
                if (key == 73 || key == 14)
                    return true;
                break;
            }
            case InputEvent::Type::LockInReleased:
            {
                if (key == 70 || key == 6)
                    return true;
                break;
            }
            case InputEvent::Type::ChangeSpellReleased:
            {
                if (key == 81)
                    return true;
                break;
            }
            default:
                return false;
                break;
            }

        return false;
    }

    void setVel(vec3d& vel)
    {
        vel = currentMovement.vel;
    }

private:
    void updateCurrentInput()
    {
        auto time = getTime();
        if (input_it < inputEvents.size() && time < finalTime)
        {
            if (inputEvents[input_it].timeStamp < time)
            {
                currentInput = inputEvents[input_it];
                input_it += 1;
            }
        }
    }

    void updateCurrentMovement()
    {
        auto time = getTime();
        if (movement_it < movementEvents.size() && time < finalTime)
        {
            if (movementEvents[movement_it].timeStamp < time)
            {
                currentMovement = movementEvents[movement_it];
                movement_it += 1;
            }
        }
    }

    unsigned int randomSeed{};
    std::vector<InputEvent> inputEvents{};
    std::vector<MovementEvent> movementEvents{};
    std::chrono::_V2::system_clock::time_point startTime{};
    std::chrono::duration<float, std::milli> finalTime{};
    InputEvent currentInput{};
    MovementEvent currentMovement{};
    std::size_t input_it{ 0 }, movement_it{ 0 };
};