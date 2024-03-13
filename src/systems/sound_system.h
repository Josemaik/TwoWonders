#ifndef SOUND_SYSTEM_H
#define SOUND_SYSTEM_H

#include "../utils/fmod/fmod.h"
#include "../utils/fmod/fmod_studio.h"
#include "../utils/fmod/fmod_errors.h"
#include "../utils/fmod/fmod_studio_common.h"
#include <string.h>
#include <stdbool.h>

typedef struct {
    FMOD_SYSTEM* coreSystem;
    FMOD_STUDIO_SYSTEM* soundSystem;
    FMOD_STUDIO_BANK* master_bank;
    FMOD_STUDIO_BANK* strings_bank;
    FMOD_STUDIO_BANK* ui_bank;
    FMOD_STUDIO_BANK* music_bank;
    FMOD_STUDIO_LOADING_STATE loadingState;
    FMOD_STUDIO_EVENTDESCRIPTION* eventDescription;
    FMOD_STUDIO_EVENTDESCRIPTION* eventDescription_Ambiente;
    FMOD_STUDIO_EVENTDESCRIPTION* eventDescription_Musica;
    FMOD_STUDIO_EVENTINSTANCE* eventInstance;
    FMOD_STUDIO_EVENTINSTANCE* eventInstance_Ambiente;
    FMOD_STUDIO_EVENTINSTANCE* eventInstance_Musica;
    FMOD_RESULT res;
    float* volume;
    float* finalvolume;
    bool pushed;
    bool music_started;
} SoundSystem;

void SoundSystem_init(SoundSystem* ss);
void SoundSystem_initBanks(SoundSystem* ss, const char* master_bank_location, const char* master_string_location, const char* ui_bank_location, const char* music_bank_location);
void SoundSystem_createEventInstance(SoundSystem* ss);
void SoundSystem_playMusicMenu(SoundSystem* ss);
void SoundSystem_seleccion_menu(SoundSystem* ss);
void SoundSystem_sonido_config(SoundSystem* ss);
void SoundSystem_sonido_mov(SoundSystem* ss);
void SoundSystem_play(SoundSystem* ss);
void SoundSystem_music_stop(SoundSystem* ss);
void SoundSystem_update(SoundSystem* ss);
void SoundSystem_clear(SoundSystem* ss);

#endif // SOUND_SYSTEM_H