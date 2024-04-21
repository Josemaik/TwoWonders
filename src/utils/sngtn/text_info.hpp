#pragma once
#include <queue>
#include <string>

enum struct SpeakerType : uint8_t
{
    NONE = 0,
    PLAYER = 1,
    PLAYER_SAD = 2,
    PLAYER_DANGER = 3,
    NOMAD = 4,
    INVESTIGATOR = 5,
    CAT = 6
};

struct TextInfo
{

    using QueueType = std::queue<std::pair<SpeakerType, std::string>>;

    void addText(const std::pair<SpeakerType, std::string>& text)
    {
        textQueue.push(text);
    }

    bool hasText() const
    {
        return !textQueue.empty();
    }

    void popText()
    {
        if (hasText())
            textQueue.pop();
    }

    const QueueType& getTextQueue() const
    {
        return textQueue;
    }
    bool notPass{ false };
private:
    // Cola para los textos
    QueueType textQueue{};
};