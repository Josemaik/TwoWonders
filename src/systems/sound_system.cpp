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

SoundSystem::SoundSystem(){
    //Inalización FMOD
    ERRCHECK(FMOD::Studio::System::create(&soundSystem));
    ERRCHECK(soundSystem->getCoreSystem(&coreSystem));
    ERRCHECK(coreSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0));
    ERRCHECK(coreSystem->setOutput(FMOD_OUTPUTTYPE_AUTODETECT));
    ERRCHECK(soundSystem->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0));
}

//inicialitza el Master, string i UI bank (els primers que s'han de carregar en memoria)
void SoundSystem::initBanks(const std::string& master_bank_location, const std::string& master_string_location, std::string const& ui_bank_location)
{
    //SOUND_TRACE("Initializing FMOD engine in our game...");
    ERRCHECK(soundSystem->loadBankFile(master_bank_location.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &master_bank));
    ERRCHECK(soundSystem->loadBankFile(master_string_location.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &strings_bank));
    //SOUND_INFO("Successfully loaded FMOD banks");

    ERRCHECK(master_bank->getLoadingState(&loadingState));

    std::cout<< loadingState << std::endl; //dona 3 = FMOD_STUDIO_LOADING_STATE_LOADING (loading in progress)  
}

void SoundSystem::update (){
    ERRCHECK(soundSystem->update() );
}

void SoundSystem::liberar(){
    //ERRCHECK(eventDescription->releaseAllInstances());
    ERRCHECK(master_bank->unload());
    ERRCHECK(strings_bank->unload());
    ERRCHECK(soundSystem->unloadAll());
}