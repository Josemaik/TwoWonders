#pragma once
#include <queue>
#include <string>

struct TextInfo
{
    void addText(const std::string& text)
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

    const std::queue<std::string>& getTextQueue() const
    {
        return textQueue;
    }

private:
    // Cola para los textos
    std::queue<std::string> textQueue{};
};