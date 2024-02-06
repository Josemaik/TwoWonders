#pragma once

#include "../utils/fmod/fmod_studio.h" // C
#include "../utils/fmod/fmod_studio.hpp" // C ++
#include "../utils/fmod/fmod.hpp" // low - level api
#include "../utils/fmod/fmod_errors.h"
#include <string>
#include <iostream>

struct SoundSystem {

    SoundSystem();

    void initBanks(std::string const& master_bank_location, std::string const& master_string_location, std::string const& ui_bank_location, std::string const& music_bank_location);
    void createEventInstance();
    void playMusicMenu();
    void seleccion_menu();
    void sonido_config();
    void sonido_mov();
    void play();
    void music_stop();

    void update();
    void clear();
    //float getVolume();
    //void setVolume(float volumen);
    bool pushed{ false };
    bool music_started{ false };

private:
    FMOD::System* coreSystem;
    FMOD::Studio::System* soundSystem;
    FMOD::Studio::Bank* master_bank;
    FMOD::Studio::Bank* strings_bank;
    FMOD::Studio::Bank* ui_bank;
    FMOD::Studio::Bank* music_bank;
    FMOD_STUDIO_LOADING_STATE loadingState{};
    FMOD::Studio::EventDescription* eventDescription;
    FMOD::Studio::EventDescription* eventDescription_Ambiente;
    FMOD::Studio::EventDescription* eventDescription_Musica;
    FMOD::Studio::EventInstance* eventInstance;
    FMOD::Studio::EventInstance* eventInstance_Ambiente;
    FMOD::Studio::EventInstance* eventInstance_Musica;
    FMOD_RESULT res;
    float* volume;
    float* finalvolume;



};