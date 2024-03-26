#pragma once
#include <vector>
#include <chrono>
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
    unsigned int timeStamp{};
    bool used{ false };
};

struct MovementEvent
{
    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(vel, timeStamp);
    }

    vec3d vel{};
    unsigned int timeStamp{};
};

struct GameData
{
    void addInputEvent(InputEvent::Type type)
    {
        inputEvents.push_back({ type, currentFrame });
    }

    void addMovementEvent(vec3d& vel)
    {
        if (movementEvents.empty() || lastVel != vel)
        {
            movementEvents.push_back({ vel, currentFrame });
            lastVel = vel;
        }
    }

    void setRandomSeed(unsigned int seed)
    {
        randomSeed = seed;
    }

    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(randomSeed, inputEvents, movementEvents);
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
        if (currentInput.timeStamp <= currentFrame && !currentInput.used)
            switch (currentInput.type)
            {
            case InputEvent::Type::AttackKeyDown:
            {
                if (key == 32 || key == 7)
                {
                    currentInput.used = true;
                    return true;
                }
                break;
            }
            default:
                return false;
            }

        return false;
    }

    bool isKeyPressed(int key)
    {
        if (currentInput.timeStamp <= currentFrame && !currentInput.used)
            switch (currentInput.type)
            {
            case InputEvent::Type::InteractKeyPressed:
            {
                if (key == 69 || key == 7)
                {
                    currentInput.used = true;
                    return true;
                }
                break;
            }
            default:
                return false;
            }

        return false;
    }

    bool isKeyReleased(int key)
    {
        if (currentInput.timeStamp <= currentFrame && !currentInput.used)
            switch (currentInput.type)
            {
            case InputEvent::Type::EnterReleased:
            {
                if (key == 257)
                {
                    currentInput.used = true;
                    return true;
                }
                break;
            }
            case InputEvent::Type::EscapeReleased:
            {
                if (key == 256 || key == 15)
                {
                    currentInput.used = true;
                    return true;
                }
                break;
            }
            case InputEvent::Type::InventoryReleased:
            {
                if (key == 73 || key == 14)
                {
                    currentInput.used = true;
                    return true;
                }
                break;
            }
            case InputEvent::Type::LockInReleased:
            {
                if (key == 70 || key == 6)
                {
                    currentInput.used = true;
                    return true;
                }
                break;
            }
            case InputEvent::Type::ChangeSpellReleased:
            {
                if (key == 81)
                {
                    currentInput.used = true;
                    return true;
                }
                break;
            }
            default:
                return false;
                break;
            }

        return false;
    }

    unsigned int getCurrentFrame() const
    {
        return currentFrame;
    }

    void updateFrame()
    {
        currentFrame += 1;
    }

    void updateFrameDown()
    {
        currentFrame -= 1;
    }

    void setVel(vec3d& vel)
    {
        vel = currentMovement.vel;
    }

private:
    void updateCurrentInput()
    {
        if (input_it < inputEvents.size())
        {
            if (currentFrame >= inputEvents[input_it].timeStamp)
            {
                currentInput = inputEvents[input_it];
                input_it += 1;
            }
        }
    }

    void updateCurrentMovement()
    {
        if (movement_it < movementEvents.size())
        {
            if (currentFrame >= movementEvents[movement_it].timeStamp)
            {
                currentMovement = movementEvents[movement_it];
                movement_it += 1;
            }
        }
    }

    unsigned int randomSeed{}, currentFrame{ 0 };
    std::vector<InputEvent> inputEvents{};
    std::vector<MovementEvent> movementEvents{};
    InputEvent currentInput{};
    MovementEvent currentMovement{};
    std::size_t input_it{ 0 }, movement_it{ 0 };
    vec3d lastVel{};
};