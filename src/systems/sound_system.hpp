#pragma once

#include "../utils/fmod/fmod.h"
#include "../utils/fmod/fmod_studio.h"
#include "../utils/fmod/fmod_errors.h"
#include "../utils/fmod/fmod_studio_common.h"
#include <string>
#include <iostream>

struct SoundSystem {

    SoundSystem();

    void initBanks(const char* master_bank_location, const char* master_string_location, const char* ui_bank_location,const char* ambient_bank_location,  const char* music_bank_location, const char* SFX_bank_location);
    void createEventInstance();
    void playMusicMenu();
    void playAmbient();
    void seleccion_menu();
    void sonido_config();
    void sonido_mov();
    void sonido_salir();
    //// sonidos nivel 0
    void abrir_cofre();
    void abrir_puerta();
    void apisonadora_ataque();
    void apisonadora_danyo();
    void cerrar_cofre();
    void dummy_golpe();
    void interaccion_e();
    void mele();
    void movimiento_camara();
    void muerte_enemigo();
    void pasos_pradera();
    void pasos_prision();
    void piedras_golpe();
    void rebote();
    void recoger_vida();


    /// 
    void play();
    void music_stop();
    void ambient_stop();

    void update();
    void clear();
    //float getVolume();
    //void setVolume(float volumen);
    bool pushed{ false };
    bool music_started{ false };
    bool ambient_started{ false};

private:
    FMOD_SYSTEM* coreSystem;
    FMOD_STUDIO_SYSTEM* soundSystem;
    FMOD_STUDIO_BANK* master_bank;
    FMOD_STUDIO_BANK* strings_bank;
    FMOD_STUDIO_BANK* ui_bank;
    FMOD_STUDIO_BANK* music_bank;
    FMOD_STUDIO_BANK* ambient_bank;
    FMOD_STUDIO_BANK* SFX_bank;
    FMOD_STUDIO_LOADING_STATE loadingState{};
    FMOD_STUDIO_EVENTDESCRIPTION* eventDescription;
    FMOD_STUDIO_EVENTDESCRIPTION* eventDescription_Ambiente;
    FMOD_STUDIO_EVENTDESCRIPTION* eventDescription_Musica;
    FMOD_STUDIO_EVENTDESCRIPTION* eventDescription_SFX;  
    FMOD_STUDIO_EVENTINSTANCE* eventInstance;
    FMOD_STUDIO_EVENTINSTANCE* eventInstance_Ambiente;
    FMOD_STUDIO_EVENTINSTANCE* eventInstance_Musica;
    FMOD_STUDIO_EVENTINSTANCE* eventInstance_SFX; 
    FMOD_RESULT res;
    float* volume;
    float* finalvolume;
};