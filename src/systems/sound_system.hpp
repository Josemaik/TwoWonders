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
    void sonido_mazmorra();
    void seleccion_menu();
    void sonido_config();
    void sonido_mov();
    void sonido_salir();
   
    void sonido_abrir_cofre();
    void sonido_abrir_puerta();
    void sonido_apisonadora_ataque();
    void sonido_apisonadora_danyo();   
    void sonido_cerrar_cofre();         
    void sonido_dummy_golpe();          
    void sonido_interaccion_e();
    void sonido_melee();
    void sonido_movimiento_camara();    
    void sonido_muerte_enemigo();
    void sonido_pasos_pradera();        //arreglar metodo, falla
    void sonido_pasos_prision();        //arreglar metodo, falla
    void sonido_piedras_golpe();
    void sonido_rebote();             
    void sonido_destello();
    void sonido_recoger_vida();         //a futuro

    void sonido_llave();                
    void sonido_h_pompa();              //comprobar
    void sonido_checkpoint();           
    void sonido_palanca();              
    void sonido_recibir_danyo();         
    void sonido_aum_vida_max();         

    //poner sonido de golpe dummy a romper las maderas
    //bajar volumen de ambiente
    ///subir SFX
    //bajar interacción

    void play();
    void play_pasos();
    void music_stop();
    void ambient_stop();
    void SFX_stop();

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