#include "sound_system.hpp"

//Para que funcione el ERRCHECK
void ERRCHECK_FMOD (FMOD_RESULT result, const char * file, int line)
{
    if(result != FMOD_OK)
    {
        std::cerr << FMOD_ErrorString(result) << std::endl;
        exit(-1);
    }
}

#define ERRCHECK(_result) ERRCHECK_FMOD(_result, __FILE__, __LINE__)

void SoundSystem::initBanks(const std::string& master_bank_location, const std::string& master_string_location, std::string const& ui_bank_location)
{
    //SOUND_TRACE("Initializing FMOD engine in our game...");
    ERRCHECK(soundSystem->loadBankFile(master_bank_location.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &master_bank));
    ERRCHECK(soundSystem->loadBankFile(master_string_location.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &strings_bank));
    //SOUND_INFO("Successfully loaded FMOD banks");
}