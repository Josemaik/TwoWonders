#include "sound_system.h"
#include "../utils/fmod/fmod_studio.h"
#include "../utils/fmod/fmod_errors.h"
#include <stdio.h>
#include <stdlib.h>

void ERRCHECK_FMOD(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        fprintf(stderr, "%s\n", FMOD_ErrorString(result));
        exit(-1);
    }
}

#define ERRCHECK(_result) ERRCHECK_FMOD(_result)

void SoundSystem_init(SoundSystem* ss) {
    ERRCHECK(FMOD_Studio_System_Create(&ss->soundSystem, FMOD_VERSION));
    ERRCHECK(FMOD_Studio_System_GetCoreSystem(ss->soundSystem, &ss->coreSystem));
    ERRCHECK(FMOD_System_SetSoftwareFormat(ss->coreSystem, 0, FMOD_SPEAKERMODE_5POINT1, 0));
    ERRCHECK(FMOD_System_SetOutput(ss->coreSystem, FMOD_OUTPUTTYPE_AUTODETECT));
    ERRCHECK(FMOD_Studio_System_Initialize(ss->soundSystem, 512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0));
}

void SoundSystem_initBanks(SoundSystem* ss, const char* master_bank_location, const char* master_string_location, const char* ui_bank_location, const char* music_bank_location)
{
    ERRCHECK(FMOD_Studio_System_LoadBankFile(ss->soundSystem, master_bank_location, FMOD_STUDIO_LOAD_BANK_NORMAL, &ss->master_bank));
    ERRCHECK(FMOD_Studio_System_LoadBankFile(ss->soundSystem, master_string_location, FMOD_STUDIO_LOAD_BANK_NORMAL, &ss->strings_bank));
    ERRCHECK(FMOD_Studio_System_LoadBankFile(ss->soundSystem, ui_bank_location, FMOD_STUDIO_LOAD_BANK_NORMAL, &ss->ui_bank));
    ERRCHECK(FMOD_Studio_System_LoadBankFile(ss->soundSystem, music_bank_location, FMOD_STUDIO_LOAD_BANK_NORMAL, &ss->music_bank));
    ERRCHECK(FMOD_Studio_Bank_GetLoadingState(ss->ui_bank, &ss->loadingState));
}

void SoundSystem_createEventInstance(SoundSystem* ss) {
    ERRCHECK(FMOD_Studio_System_GetEvent(ss->soundSystem, "event:/Menús/main_select", &ss->eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(ss->eventDescription, &ss->eventInstance));
    FMOD_Studio_EventInstance_Start(ss->eventInstance);
}

void SoundSystem_playMusicMenu(SoundSystem* ss) {
    ERRCHECK(FMOD_Studio_System_GetEvent(ss->soundSystem, "event:/Música/Menu/menu_music", &ss->eventDescription_Musica));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(ss->eventDescription_Musica, &ss->eventInstance_Musica));
    FMOD_Studio_EventInstance_Start(ss->eventInstance_Musica);
    FMOD_Studio_System_Update(ss->soundSystem);
}


void SoundSystem_seleccion_menu(SoundSystem* ss) {
    ERRCHECK(FMOD_Studio_System_GetEvent(ss->soundSystem, "event:/Menús/main_select", &ss->eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(ss->eventDescription, &ss->eventInstance));
    FMOD_Studio_EventInstance_Start(ss->eventInstance);
    SoundSystem_update(ss);
}

void SoundSystem_sonido_config(SoundSystem* ss) {
    ERRCHECK(FMOD_Studio_System_GetEvent(ss->soundSystem, "event:/Menús/sound_config", &ss->eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(ss->eventDescription, &ss->eventInstance));
    FMOD_Studio_EventInstance_Start(ss->eventInstance);
}

void SoundSystem_sonido_mov(SoundSystem* ss) {
    ERRCHECK(FMOD_Studio_System_GetEvent(ss->soundSystem, "event:/Menús/main_mov", &ss->eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(ss->eventDescription, &ss->eventInstance));
    FMOD_Studio_EventInstance_Start(ss->eventInstance);
    SoundSystem_update(ss);
}

void SoundSystem_play(SoundSystem* ss) {
    FMOD_Studio_EventInstance_Start(ss->eventInstance);
}

void SoundSystem_update(SoundSystem* ss) {
    ERRCHECK(FMOD_Studio_System_Update(ss->soundSystem));
}

void SoundSystem_clear(SoundSystem* ss) {
    FMOD_Studio_Bank_Unload(ss->master_bank);
    FMOD_Studio_Bank_Unload(ss->strings_bank);
    FMOD_Studio_Bank_Unload(ss->ui_bank);
    FMOD_Studio_Bank_Unload(ss->music_bank);
    FMOD_Studio_System_UnloadAll(ss->soundSystem);
}

void SoundSystem_music_stop(SoundSystem* ss) {
    FMOD_Studio_EventInstance_Stop(ss->eventInstance_Musica, FMOD_STUDIO_STOP_ALLOWFADEOUT);
}

/*float SoundSystem::getVolume(){
    eventInstance_Musica->getVolume(volume, finalvolume);
    float volumen = volume;
    return volumen;
}
void SoundSystem::setVolume(float volumen){
    eventInstance_Musica->setVolume(volumen);
}*/
