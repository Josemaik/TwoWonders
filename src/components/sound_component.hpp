#pragma once
#include "../utils/fmod/fmod.h"
#include "../utils/fmod/fmod_studio.h"
#include "../utils/fmod/fmod_errors.h"
#include "../utils/fmod/fmod_studio_common.h"
    
struct SoundComponent
{    
    //eventInstance de sonido, se utilizara para el movimiento de los enemigos principalmente
    FMOD_STUDIO_EVENTINSTANCE* sound_mov {nullptr};
};