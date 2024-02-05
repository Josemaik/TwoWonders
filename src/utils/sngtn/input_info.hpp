#pragma once 

struct InputInfo
{
    bool up{ false };
    bool down{ false };
    bool left{ false };
    bool right{ false };

    bool space{ false };
    bool enter{ false };
    bool esc{ false };

    bool debugPhy{ false };
    bool debugAI1{ false };
    bool debugAI2{ false };

    bool lockOn{ false };
};