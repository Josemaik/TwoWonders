#include "sound_system.hpp"

void ERRCHECK_FMOD(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        fprintf(stderr, "%s\n", FMOD_ErrorString(result));
        exit(-1);
    }
}

#define ERRCHECK(_result) ERRCHECK_FMOD(_result)

SoundSystem::SoundSystem() {
    ERRCHECK(FMOD_Studio_System_Create(&soundSystem, FMOD_VERSION));
    ERRCHECK(FMOD_Studio_System_GetCoreSystem(soundSystem, &coreSystem));
    ERRCHECK(FMOD_System_SetSoftwareFormat(coreSystem, 0, FMOD_SPEAKERMODE_5POINT1, 0));
    ERRCHECK(FMOD_System_SetOutput(coreSystem, FMOD_OUTPUTTYPE_AUTODETECT));
    ERRCHECK(FMOD_Studio_System_Initialize(soundSystem, 512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0));
}

void SoundSystem::initBanks(const char* master_bank_location, const char* master_string_location, const char* ui_bank_location,const char* ambient_bank_location,  const char* music_bank_location, const char* SFX_bank_location)
{
    ERRCHECK(FMOD_Studio_System_LoadBankFile(soundSystem, master_bank_location, FMOD_STUDIO_LOAD_BANK_NORMAL, &master_bank));
    ERRCHECK(FMOD_Studio_System_LoadBankFile(soundSystem, master_string_location, FMOD_STUDIO_LOAD_BANK_NORMAL, &strings_bank));
    ERRCHECK(FMOD_Studio_System_LoadBankFile(soundSystem, ui_bank_location, FMOD_STUDIO_LOAD_BANK_NORMAL, &ui_bank));
    ERRCHECK(FMOD_Studio_System_LoadBankFile(soundSystem, music_bank_location, FMOD_STUDIO_LOAD_BANK_NORMAL, &music_bank));
    ERRCHECK(FMOD_Studio_System_LoadBankFile(soundSystem, ambient_bank_location, FMOD_STUDIO_LOAD_BANK_NORMAL, &ambient_bank));
    ERRCHECK(FMOD_Studio_System_LoadBankFile(soundSystem, SFX_bank_location, FMOD_STUDIO_LOAD_BANK_NORMAL, &SFX_bank));
    ERRCHECK(FMOD_Studio_Bank_GetLoadingState(ui_bank, &loadingState));
}

void SoundSystem::createEventInstance() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Menus/main_select", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    FMOD_Studio_EventInstance_Start(eventInstance);
}

void SoundSystem::playMusicMenu() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Musica/Menu/menu_music", &eventDescription_Musica));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription_Musica, &eventInstance_Musica));
    FMOD_Studio_EventInstance_Start(eventInstance_Musica);
    FMOD_Studio_System_Update(soundSystem);
}


void SoundSystem::seleccion_menu() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Menus/main_select", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    FMOD_Studio_EventInstance_Start(eventInstance);
    update();
}
void SoundSystem::playAmbient(){
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Ambientes/amb_bosque", &eventDescription_Ambiente));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription_Ambiente, &eventInstance_Ambiente));
    FMOD_Studio_EventInstance_Start(eventInstance_Ambiente);
    FMOD_Studio_System_Update(soundSystem);
}

void SoundSystem::sonido_config() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Menus/sound_config", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    FMOD_Studio_EventInstance_Start(eventInstance);
}

void SoundSystem::sonido_mov() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Menus/main_mov", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}
void SoundSystem::sonido_salir() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Menus/salir", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    FMOD_Studio_EventInstance_Start(eventInstance);
    update();
}

void SoundSystem::sonido_abrir_cofre() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Interaccion/Cofre/Abrir-cofre", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}

void SoundSystem::sonido_abrir_puerta() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Puerta/Abrir_puerta", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}

void SoundSystem::sonido_apisonadora_ataque() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Enemigos/Apisonadora/Apisonadora_ataque", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}

void SoundSystem::sonido_apisonadora_danyo() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Enemigos/Apisonadora/Apisonadora_danyo", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}

void SoundSystem::sonido_cerrar_cofre() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Interaccion/Cofre/Cerrar-cofre", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}

void SoundSystem::sonido_dummy_golpe() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Enemigos/Dummy_golpe", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}
void SoundSystem::sonido_interaccion_e() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Interaccion/E", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}

void SoundSystem::sonido_melee() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Ataques/Mele", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}

void SoundSystem::sonido_movimiento_camara() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Feedback/Movimiento_camara", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}

void SoundSystem::sonido_muerte_enemigo() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Enemigos/Muere_cualquier_enemigo", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}
void SoundSystem::sonido_pasos_pradera() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Pasos/Pasos_pradera", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}
void SoundSystem::sonido_pasos_prision() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Pasos/Pasos_prision", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}
void SoundSystem::sonido_piedras_golpe() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Enemigos/Piedras_golpe", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}

void SoundSystem::sonido_rebote() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Interaccion/Rebote", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}

void SoundSystem::sonido_destello() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Interaccion/Recoger_destello", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}

void SoundSystem::sonido_recoger_vida() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Interaccion/Recoger_vida", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}

void SoundSystem::sonido_llave() {
    //ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Interaccion/Recoger_vida", &eventDescription));
    //ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    //play();
    //update();
}

void SoundSystem::sonido_h_pompa() {
    //ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Interaccion/Recoger_vida", &eventDescription));
    //ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    //play();
    //update();
}
void SoundSystem::sonido_checkpoint() {
    //ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Interaccion/Recoger_vida", &eventDescription));
    //ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    //play();
    //update();
}

void SoundSystem::sonido_palanca() {
    //ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Interaccion/Recoger_vida", &eventDescription));
    //ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    //play();
    //update();
}
void SoundSystem::sonido_recibir_danyo() {
    //ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Interaccion/Recoger_vida", &eventDescription));
    //ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    //play();
    //update();
}

void SoundSystem::play() {
    FMOD_Studio_EventInstance_Start(eventInstance);
}

void SoundSystem::update() {
    ERRCHECK(FMOD_Studio_System_Update(soundSystem));
}

void SoundSystem::clear() {
    FMOD_Studio_Bank_Unload(master_bank);
    FMOD_Studio_Bank_Unload(strings_bank);
    FMOD_Studio_Bank_Unload(ui_bank);
    FMOD_Studio_Bank_Unload(music_bank);
    FMOD_Studio_Bank_Unload(ambient_bank);
    FMOD_Studio_Bank_Unload(SFX_bank); 
    FMOD_Studio_System_UnloadAll(soundSystem);
}

void SoundSystem::music_stop() {
    FMOD_Studio_EventInstance_Stop(eventInstance_Musica, FMOD_STUDIO_STOP_ALLOWFADEOUT);
}
void SoundSystem::ambient_stop() {
    FMOD_Studio_EventInstance_Stop(eventInstance_Ambiente, FMOD_STUDIO_STOP_ALLOWFADEOUT);
    update();
}

/*float SoundSystem::getVolume(){
    eventInstance_Musica->getVolume(volume, finalvolume);
    float volumen = volume;
    return volumen;
}
void SoundSystem::setVolume(float volumen){
    eventInstance_Musica->setVolume(volumen);
}*/
