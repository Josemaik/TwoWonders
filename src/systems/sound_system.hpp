#pragma once

#include "../utils/fmod/fmod_studio.h" // C
#include "../utils/fmod/fmod_studio.hpp" // C ++
#include "../utils/fmod/fmod.hpp" // low - level api
#include "../utils/fmod/fmod_errors.h"
#include <string>
#include <iostream>

struct SoundSystem {

    SoundSystem();

    void initBanks(std::string const& master_bank_location, std::string const& master_string_location, std::string const& ui_bank_location);

    void update();
    void liberar();

    private:
        FMOD::System* coreSystem;
        FMOD::Studio::System * soundSystem;
        FMOD::Studio::Bank* master_bank;
        FMOD::Studio::Bank* strings_bank;
        FMOD::Studio::EventDescription* eventDescription;
        FMOD_STUDIO_LOADING_STATE loadingState{};
        //FMOD::Studio::Bank villagesBank;
        //FMOD_RESULT result;

};