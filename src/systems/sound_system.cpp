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

//Constructor
SoundSystem::SoundSystem(){
    //Inalización FMOD
    ERRCHECK(FMOD::Studio::System::create(&soundSystem));
    ERRCHECK(soundSystem->getCoreSystem(&coreSystem));
    ERRCHECK(coreSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0));
    ERRCHECK(coreSystem->setOutput(FMOD_OUTPUTTYPE_AUTODETECT));
    ERRCHECK(soundSystem->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0));
}

//inicialitza el Master, string i UI bank (els primers que s'han de carregar en memoria)
void SoundSystem::initBanks(const std::string& master_bank_location, const std::string& master_string_location, std::string const& ui_bank_location, std::string const& music_bank_location)
{
    ERRCHECK(soundSystem->loadBankFile(master_bank_location.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &master_bank));
    ERRCHECK(soundSystem->loadBankFile(master_string_location.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &strings_bank));
    ERRCHECK(soundSystem->loadBankFile(ui_bank_location.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &ui_bank));
    ERRCHECK(soundSystem->loadBankFile(music_bank_location.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &music_bank));
    //ERRCHECK(master_bank->getLoadingState(&loadingState));
    //ERRCHECK(strings_bank->getLoadingState(&loadingState));
    ERRCHECK(ui_bank->getLoadingState(&loadingState));

    std::cout<< loadingState << std::endl; //dona 3 = FMOD_STUDIO_LOADING_STATE_LOADING (loading in progress)  
}

//crear instancies d'events
void SoundSystem::createEventInstance(){
    ERRCHECK(soundSystem->getEvent("event:/Menús/main_select", &eventDescription) );
    ERRCHECK(soundSystem->getEvent("event:/Música/Menu/menu_music", &eventDescription_Musica) );
    
    ERRCHECK(eventDescription->createInstance(&eventInstance));
    ERRCHECK(eventDescription_Musica->createInstance(&eventInstance_Musica));
    //açó fa que sone el evento que haja assignat a eventInstance
    eventInstance->start(); //sona!!!
    eventInstance_Musica->start();
    //FMOD_Studio_EventInstance_Start(&eventInstance);
}

//play event
void SoundSystem::play(){
    FMOD_RESULT FMOD_Studio_EventInstance_Start();
    //FMOD_RESULT Studio::EventInstance::start();
}

void SoundSystem::update (){
    ERRCHECK(soundSystem->update());
}

void SoundSystem::liberar(){
    //ERRCHECK(eventDescription->releaseAllInstances());
    ERRCHECK(master_bank->unload());
    ERRCHECK(strings_bank->unload());
    ERRCHECK(soundSystem->unloadAll());
}