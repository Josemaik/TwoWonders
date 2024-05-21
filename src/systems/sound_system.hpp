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
    void initEvents();
    void createEventInstance();
    void playMusicMenu();
    void playAmbient();
    void sonido_amb_bosque();
    void sonido_mazmorra();
    void seleccion_menu();
    void recoger_mana();
    void sonido_mov();
    void sonido_salir();

    FMOD_STUDIO_EVENTINSTANCE* createNewEventInstance();

   
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
    void sonido_pasos_pradera();       
    void sonido_pasos_prision();        
    void sonido_piedras_golpe();
    void sonido_rebote();             
    void sonido_destello();
    void sonido_recoger_vida();        

    void sonido_llave();                
    void sonido_h_pompa();              
    void sonido_checkpoint();           
    void sonido_palanca();              
    void sonido_recibir_danyo();         
    void sonido_aum_vida_max();         



    void sonido_music_mazmorra();   
    //dialogos
    void sonido_DInvestigador();    
    void sonido_DPlayer();          
    void sonido_DCalabaza();        
    void sonido_DEsquimal();        //por integrar
    void sonido_DPescador();        //por integrar
    void sonido_DSacerdotisa();     //por integrar
    void sonido_DEspiritu();        //por integrar
    void sonido_DGuerrera();        //por integrar    
    void sonido_DMaestro();         //por integrar

   //sonidos de zona volcan

    void sonido_music_volcan();     
    void sonido_h_dash();                  
    void sonido_h_bola_fuego();     
    void sonido_amb_volcan();       
    void sonido_pasos_volcan();     
    void sonido_golem_mov( FMOD_STUDIO_EVENTINSTANCE*& sonido);         
    void sonido_golem_ataque();     
    void sonido_golem_danyo();      
    void sonido_golem_muere();      
    void sonido_munyeco_mov(FMOD_STUDIO_EVENTINSTANCE*& sonido);      //por cambiar segun el tipo de munyeco
    void sonido_munyeco_ataque();   
    void sonido_munyeco_danyo();    
    void sonido_munyeco_muere();    
    void sonido_abrir_puerta_magica();  
    void sonido_recoger_pieza();    
    void sonido_equipar_botas();    
    void sonido_equipar_gorro();    



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

   void sonido_amb_pradera();           
   void sonido_music_pradera();         //por integrar
   void sonido_h_escudo();              //por comprobar
   void sonido_slime_ataque();          //por integrar
   void sonido_slime_danyo();           
   void sonido_slime_mov(FMOD_STUDIO_EVENTINSTANCE*& sonido);             
   void sonido_slime_muere();           //por cambiar dpendiendo del tipo se dlime
   void sonido_slime_curar();           //por integrar    
   void sonido_crear_pociones();        //por integrar    
   void sonido_campamento_derrotado1(); //por integrar          
   void sonido_equipar_lanza();         //por integrar    


   //sonidos de zona biblioteca

    void sonido_aranya_ataque();        //por integrar
    void sonido_aranya_danyo();         //por integrar
    void sonido_aranya_mov();           //por integrar
    void sonido_aranya_muere();         //por integrar
    void sonido_aranya_telaranya();     //por integrar
    void sonido_music_boss_final();     //por integrar
    void sonido_escaleras_biblio();     //por integrar
    void sonido_h_meteorito();          //por integrar
    void sonido_pasos_madera();         //por integrar
    void sonido_amb_biblioteca1();      //por integrar
    void sonido_amb_biblioteca2();      //por integrar

    void sonido_boss_mov();             //por integrar
    void sonido_subdito_mov();          //por integrar  
    void sonido_boss_resucitar();       //por integrar
    void sonido_boss_escudo();          //por integrar
    void sonido_boss_danyo();           //por integrar
    void sonido_boss_subdito_danyo();   //por integrar
    void sonido_subdito_muere();        //por integrar
    void sonido_boss_muere();           //por integrar
    void sonido_inicio_batalla();       //por integrar
    void sonido_cambio_fase();          //por integrar
    void sonido_win();                  //por integrar
    void sonido_jugador_muere();        
    void sonido_no_mana();
    void sonido_puerta_prision();





    void ambiente_parameter_lava(float lava);

   

    void play();
    void play_pasos();
    void play_music();
    void play_music_level();
    void music_stop();
    void music_stop_level();
    void ambient_stop();
    void SFX_pasos_stop();
    void stop_enemigo_mov(FMOD_STUDIO_EVENTINSTANCE* sonido);


    void sonido_pause(int zona);
    void sonido_unpause(int zona);


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
    void getVolumeMov();
    void setVolumeMov(float volumen);
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
    FMOD_STUDIO_EVENTDESCRIPTION* eventDescription_Musica_volcan;
    FMOD_STUDIO_EVENTDESCRIPTION* eventDescription_Musica_mazmorra;
    FMOD_STUDIO_EVENTDESCRIPTION* eventDescription_Musica_pradera;
    FMOD_STUDIO_EVENTDESCRIPTION* eventDescription_Musica_monte;
    FMOD_STUDIO_EVENTDESCRIPTION* eventDescription_Musica_boss_final;
    FMOD_STUDIO_EVENTDESCRIPTION* eventDescription_pasos;
    FMOD_STUDIO_EVENTDESCRIPTION* eventDescription_munyeco_mov;   
    FMOD_STUDIO_EVENTDESCRIPTION* eventDescription_golem_mov; 
    FMOD_STUDIO_EVENTDESCRIPTION* eventDescription_slime_mov; 
    FMOD_STUDIO_EVENTINSTANCE* eventInstance;
    FMOD_STUDIO_EVENTINSTANCE* eventInstance_Ambiente;
    FMOD_STUDIO_EVENTINSTANCE* eventInstance_Ambiente_volcan;
    FMOD_STUDIO_EVENTINSTANCE* eventInstance_Musica;
    FMOD_STUDIO_EVENTINSTANCE* eventInstance_Musica_Level;
    FMOD_STUDIO_EVENTINSTANCE* eventInstance_SFX_pasos;
    FMOD_STUDIO_EVENTINSTANCE* eventInstance_mov_golem;
    FMOD_STUDIO_EVENTINSTANCE* eventInstance_mov_munyeco;

    FMOD_STUDIO_BUS* bus;
    FMOD_STUDIO_BUS* sfxBus;
    FMOD_STUDIO_BUS* AmbientBus;
    FMOD_STUDIO_BUS* MusicBus;
    FMOD_STUDIO_BUS* MovBus;
    
    FMOD_CHANNELGROUP *masterGroup {};
    FMOD_CHANNELGROUP *ambientGroup {};
    FMOD_CHANNELGROUP *musicGroup {};
    FMOD_CHANNELGROUP *sfxGroup {};
    
    
    
    
    //Variables para guardar el volumen, en caso de querer mutear el juego
    float musicVolume;
    float sfxVolume;
    float ambientVolume;
    float generalVolume;

    //Variable para guardar el volumen de los SFX de los pasos del movimiento de los enemigos
    //Se utilizará para cuando se abra el menú de pausa almacenar el volumen anterior
    float movVolume {0};


    //Variables para comprobaciones de pausa
    FMOD_BOOL ambiente {true};
    FMOD_BOOL ambiente_volcan {true};
    FMOD_BOOL ambiente_pradera {true};
    FMOD_BOOL ambiente_biblioteca {true};
    FMOD_BOOL musica_suena {true};




};