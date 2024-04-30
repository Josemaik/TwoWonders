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
    void initBuses();
    void initChannels();
    void createEventInstance();
    void playMusicMenu();
    void playAmbient();
    void sonido_amb_bosque();
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
    void sonido_h_pompa();              
    void sonido_checkpoint();           
    void sonido_palanca();              
    void sonido_recibir_danyo();         
    void sonido_aum_vida_max();         


    //SONIDOS PENDIENTES
    void sonido_music_mazmorra();   //por integrar
    //dialogos
    void sonido_DInvestigador();    //por integrar
    void sonido_DPlayer();          //por integrar
    void sonido_DCalabaza();        //por integrar
    void sonido_DEsquimal();        //por integrar
    void sonido_DPescador();        //por integrar
    void sonido_DSacerdotisa();     //por integrar
    void sonido_DEspiritu();        //por integrar
    void sonido_DGuerrera();        //por integrar    

   //sonidos de zona volcan

    void sonido_music_volcan();     //falla
    void sonido_h_dash();                  
    void sonido_h_bola_fuego();     
    void sonido_amb_volcan();       //por integrar
    void sonido_pasos_volcan();     //por integrar
    void sonido_golem_mov();        //por integrar
    void sonido_golem_ataque();     
    void sonido_golem_danyo();      
    void sonido_golem_muere();      
    void sonido_munyeco_mov();      //por integrar
    void sonido_munyeco_ataque();   
    void sonido_munyeco_danyo();    
    void sonido_munyeco_muere();    
    void sonido_abrir_puerta_magica();  //por integrar
    void sonido_recoger_pieza();    //ahora no suena
    void sonido_equipar_botas();    //por integrar
    void sonido_equipar_gorro();    //por integrar



   //sonidos de zona monte

    void sonido_amb_monte();        //por integrar   
    void sonido_music_monte();      //por integrar
    void sonido_pasos_nieve();      //por integrar
    void sonido_h_estacas();        //por integrar
    void sonido_equipar_escudo();   //por integrar
    void sonido_activar_baliza();   //por integrar
    void sonido_portal();           //por integrar
    void sonido_pesca();            //por integrar

   //sonidos de zona pradera

   void sonido_amb_pradera();           //por integrar
   void sonido_music_pradera();         //por integrar
   void sonido_h_escudo();              //por integrar
   void sonido_slime_ataque();          //por integrar
   void sonido_slime_danyo();           //por integrar
   void sonido_slime_mov();             //por integrar
   void sonido_slime_muere();           //por integrar
   void sonido_slime_curar();           //por integrar    
   void sonido_crear_pociones();        //por integrar    
   void sonido_campamento_derrotado1(); //por integrar          
   void sonido_equipar_lanza();         //por integrar    
   

    void play();
    void play_pasos();
    void play_music();
    void music_stop();
    void ambient_stop();
    void SFX_stop();

    void update();
    void clear();
    float getVolumeMaster();
    float getVolumeSFX();
    float getVolumeMusic();
    float getVolumeAmbient();
    void setVolumeMaster(float volumen);
    void setVolumeSFX(float volumen);
    void setVolumeMusic(float volumen);
    void setVolumeAmbient(float volumen);
    void muteMaster();
    void muteAmbient();
    void muteSFX();
    void muteMusic();
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
    FMOD_STUDIO_BUS* bus;
    FMOD_STUDIO_BUS* sfxBus;
    FMOD_STUDIO_BUS* AmbientBus;
    FMOD_STUDIO_BUS* MusicBus;
    
    FMOD_CHANNELGROUP *masterGroup {};
    FMOD_CHANNELGROUP *ambientGroup {};
    FMOD_CHANNELGROUP *musicGroup {};
    FMOD_CHANNELGROUP *sfxGroup {};
    
    
    
    
    //Variables para guardar el volumen, en caso de querer mutear el juego
    float musicVolume;
    float sfxVolume;
    float ambientVolume;
    float generalVolume;

    //FMOD_RESULT res;
    //float* volume;
    //float* finalvolume;


};