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
    ERRCHECK(FMOD_Studio_System_Initialize(soundSystem, 512, FMOD_STUDIO_INIT_NORMAL | FMOD_STUDIO_INIT_LIVEUPDATE, FMOD_INIT_NORMAL, 0));

  
}

void SoundSystem::initBanks(const char* master_bank_location, const char* master_string_location, const char* ui_bank_location, const char* ambient_bank_location, const char* music_bank_location, const char* SFX_bank_location)
{
    ERRCHECK(FMOD_Studio_System_LoadBankFile(soundSystem, master_bank_location, FMOD_STUDIO_LOAD_BANK_NORMAL, &master_bank));
    ERRCHECK(FMOD_Studio_System_LoadBankFile(soundSystem, master_string_location, FMOD_STUDIO_LOAD_BANK_NORMAL, &strings_bank));
    ERRCHECK(FMOD_Studio_System_LoadBankFile(soundSystem, ui_bank_location, FMOD_STUDIO_LOAD_BANK_NORMAL, &ui_bank));
    ERRCHECK(FMOD_Studio_System_LoadBankFile(soundSystem, music_bank_location, FMOD_STUDIO_LOAD_BANK_NORMAL, &music_bank));
    ERRCHECK(FMOD_Studio_System_LoadBankFile(soundSystem, ambient_bank_location, FMOD_STUDIO_LOAD_BANK_NORMAL, &ambient_bank));
    ERRCHECK(FMOD_Studio_System_LoadBankFile(soundSystem, SFX_bank_location, FMOD_STUDIO_LOAD_BANK_NORMAL, &SFX_bank));
    ERRCHECK(FMOD_Studio_Bank_GetLoadingState(ui_bank, &loadingState));

    initBuses();

    initChannels();

    initEvents();

 

    
}

void SoundSystem::createEventInstance() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Menus/main_select", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    FMOD_Studio_EventInstance_Start(eventInstance);
}

void SoundSystem::initBuses(){
    ERRCHECK(FMOD_Studio_System_GetBus(soundSystem, "bus:/SFX", &sfxBus));
    ERRCHECK(FMOD_Studio_System_GetBus(soundSystem, "bus:/Ambience", &AmbientBus));
    ERRCHECK(FMOD_Studio_System_GetBus(soundSystem, "bus:/Music", &MusicBus));
    ERRCHECK(FMOD_Studio_System_GetBus(soundSystem, "bus:/SFX/Mov_enemigos", &MovBus));
  
}

void SoundSystem::initChannels(){

      ERRCHECK(FMOD_System_GetMasterChannelGroup(coreSystem, &masterGroup));

}
void SoundSystem::initEvents(){

    //MUSICA DE LOS DIFERENTES NIVELES
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Musica/music_volcan", &eventDescription_Musica_volcan));
  
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Musica/Mazmorra/music_mazmorra", &eventDescription_Musica_mazmorra));

    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Musica/music_pradera", &eventDescription_Musica_pradera));

    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Musica/music_monte", &eventDescription_Musica_monte));

    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Musica/music_boss_final", &eventDescription_Musica_boss_final));
    
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Musica/Menu/menu_music", &eventDescription_Musica));

    //Movimiento de los enemigos

    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Enemigos/Munyeco/Munyeco_mov", &eventDescription_munyeco_mov));

    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Enemigos/Golem/Golem_mov", &eventDescription_golem_mov));


}



void SoundSystem::playMusicMenu() {
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription_Musica, &eventInstance_Musica));
    play_music();
}

void SoundSystem::sonido_music_volcan() {
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription_Musica_volcan, &eventInstance_Musica_Level));
    play_music_level();
   

}

void SoundSystem::sonido_music_mazmorra() {
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription_Musica_mazmorra, &eventInstance_Musica_Level));
    play_music_level();


}
void SoundSystem::sonido_music_monte() {
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription_Musica_monte, &eventInstance_Musica_Level));
    play_music_level();
}

void SoundSystem::sonido_music_pradera() {
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription_Musica_pradera, &eventInstance_Musica_Level));
    play_music_level();
}

void SoundSystem::sonido_music_boss_final(){
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription_Musica_boss_final, &eventInstance_Musica_Level));
    play_music_level();

}  


void SoundSystem::seleccion_menu() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Menus/main_select", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    FMOD_Studio_EventInstance_Start(eventInstance);
    update();
}

void SoundSystem::playAmbient() {
    FMOD_Studio_EventInstance_Start(eventInstance_Ambiente);
    //FMOD_Studio_System_Update(soundSystem);
    update();
}

void SoundSystem::sonido_amb_bosque() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Ambientes/amb_bosque", &eventDescription_Ambiente));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription_Ambiente, &eventInstance_Ambiente));
    //FMOD_Studio_EventInstance_Start(eventInstance_Ambiente);
    // FMOD_Studio_System_Update(soundSystem);
    playAmbient();
    //update();
}

void SoundSystem::sonido_mazmorra() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Ambientes/amb_mazmorra", &eventDescription_Ambiente));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription_Ambiente, &eventInstance_Ambiente));
    //FMOD_Studio_EventInstance_Start(eventInstance_Ambiente);
    //FMOD_Studio_System_Update(soundSystem);
    playAmbient();
    //update();
}

void SoundSystem::sonido_amb_volcan() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Ambientes/amb_volcan", &eventDescription_Ambiente));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription_Ambiente, &eventInstance_Ambiente_volcan));
    FMOD_Studio_EventInstance_Start(eventInstance_Ambiente_volcan);
    FMOD_Studio_System_Update(soundSystem);
    update();
    ambient_started = true;
}

void SoundSystem::ambiente_parameter_lava(float lava){
    FMOD_BOOL boolean= true;
    if( lava < 3){
    ERRCHECK( FMOD_Studio_EventInstance_SetParameterByName(eventInstance_Ambiente_volcan, "Lava", lava, boolean));
    }
    update();
}

void SoundSystem::sonido_amb_monte() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Ambientes/amb_monte", &eventDescription_Ambiente));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription_Ambiente, &eventInstance_Ambiente));
    playAmbient();
}
void SoundSystem::sonido_amb_pradera() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Ambientes/amb_pradera", &eventDescription_Ambiente));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription_Ambiente, &eventInstance_Ambiente));
    playAmbient();
}

void SoundSystem::sonido_amb_biblioteca1(){
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Ambientes/amb_biblioteca1", &eventDescription_Ambiente));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance_Ambiente));
    FMOD_Studio_EventInstance_Start(eventInstance_Ambiente);
    FMOD_Studio_System_Update(soundSystem);
    play();
    update();
}

void SoundSystem::sonido_amb_biblioteca2(){
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Ambientes/amb_biblioteca2", &eventDescription_Ambiente));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance_Ambiente));
    FMOD_Studio_EventInstance_Start(eventInstance_Ambiente);
    FMOD_Studio_System_Update(soundSystem);
    play();
    update();
}




void SoundSystem::recoger_mana() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Interaccion/Recoger_mana", &eventDescription));
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
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Interaccion/Cofre/Llave", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}

void SoundSystem::sonido_h_pompa() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Ataques/H_pompa", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}
void SoundSystem::sonido_checkpoint() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Altar_checkpoint", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}

void SoundSystem::sonido_palanca() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Puerta/Palanca", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}
void SoundSystem::sonido_recibir_danyo() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Jugador_danyo", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}

void SoundSystem::sonido_aum_vida_max() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Interaccion/Aumentar_vida_max", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}

void SoundSystem::sonido_h_dash() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Ataques/H_dash", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}
void SoundSystem::sonido_h_bola_fuego() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Ataques/H_bola", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}







void SoundSystem::sonido_golem_mov( FMOD_STUDIO_EVENTINSTANCE*& sonido) {
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription_golem_mov, &sonido));
    FMOD_Studio_EventInstance_Start(sonido);
    update();
}



void SoundSystem::sonido_golem_ataque() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Enemigos/Golem/Golem_ataque", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}

void SoundSystem::sonido_golem_danyo() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Enemigos/Golem/Golem_danyo", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}
void SoundSystem::sonido_golem_muere() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Enemigos/Golem/Golem_muere", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}

void SoundSystem::sonido_munyeco_mov(FMOD_STUDIO_EVENTINSTANCE*& sonido) {
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription_munyeco_mov, &sonido));
    FMOD_Studio_EventInstance_Start(sonido);
    update();
}

void SoundSystem::sonido_munyeco_ataque() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Enemigos/Munyeco/Munyeco_ataque", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}

void SoundSystem::sonido_munyeco_danyo() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Enemigos/Munyeco/Munyeco_danyo", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}

void SoundSystem::sonido_munyeco_muere() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Enemigos/Munyeco/Munyeco_muere", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}

void SoundSystem::sonido_abrir_puerta_magica() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Puerta/Puerta_agua", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}

void SoundSystem::sonido_recoger_pieza() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Interaccion/Objetos/item_conseguido", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}

void SoundSystem::sonido_equipar_botas() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Interaccion/Objetos/Botas_equipar", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}

void SoundSystem::sonido_equipar_gorro() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Interaccion/Objetos/Gorro_equipar", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}



void SoundSystem::sonido_h_estacas() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Ataques/H_estacas", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}


void SoundSystem::sonido_equipar_escudo() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Interaccion/Objetos/Escudo_equipar", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}


void SoundSystem::sonido_activar_baliza() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Puerta/Baliza", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}

void SoundSystem::sonido_portal() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Puerta/Portal", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}

void SoundSystem::sonido_pesca() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Pesca", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}




void SoundSystem::sonido_h_escudo() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Ataques/H_escudo_hielo", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}


void SoundSystem::sonido_slime_ataque() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Enemigos/Slime/Slime_ataque", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}

void SoundSystem::sonido_slime_danyo() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Enemigos/Slime/Slime_danyo", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}

void SoundSystem::sonido_slime_muere() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Enemigos/Slime/Slime_muere", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}

void SoundSystem::sonido_slime_mov() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Enemigos/Slime/Slime_mov", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}

void SoundSystem::sonido_slime_curar() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Enemigos/Slime/Slime_curar", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}

void SoundSystem::sonido_crear_pociones() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Pocion_crear", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}

void SoundSystem::sonido_campamento_derrotado1() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Enemigos/Campamento_derrota", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}

void SoundSystem::sonido_equipar_lanza() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Interaccion/Objetos/Lanza_equipar", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}


//EVENTOS BIBLIOTECA
    void SoundSystem::sonido_aranya_ataque(){
        ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Enemigos/Aranya/Aranya_ataque", &eventDescription));
        ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
        play();
        update();
    }           
    void SoundSystem::sonido_aranya_danyo(){
        ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Enemigos/Aranya/Aranya_danyo", &eventDescription));
        ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
        play();
        update();

    }         
    void SoundSystem::sonido_aranya_mov(){
        ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Enemigos/Aranya/Aranya_mov", &eventDescription));
        ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
        play();
        update();

    }           
    void SoundSystem::sonido_aranya_muere(){
ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Enemigos/Aranya/Aranya_muere", &eventDescription));
        ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
        play();
        update();
    }         
    void SoundSystem::sonido_aranya_telaranya(){
        ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Enemigos/Aranya/Aranya_telaranya", &eventDescription));
        ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
        play();
        update();

    }     
   
    void SoundSystem::sonido_escaleras_biblio(){
        ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Escalera_biblioteca", &eventDescription));
        ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
        play();
        update();

    }     
    void SoundSystem::sonido_h_meteorito(){
        ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Ataques/H_meteorito", &eventDescription));
        ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
        play();
        update();

    }

    void SoundSystem::sonido_boss_mov(){
        ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Enemigos/Boss/Boss_mov", &eventDescription));
        ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
        play();
        update();
    }            
    void SoundSystem::sonido_subdito_mov(){
        ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Enemigos/Boss/Subdito_mov", &eventDescription));
        ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
        play();
        update();

    }           
    void SoundSystem::sonido_boss_resucitar(){
        ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Enemigos/Boss/Boss_resucitar", &eventDescription));
        ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
        play();
        update();

    }           
    void SoundSystem::sonido_boss_escudo(){
        ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Enemigos/Boss/Boss_escudo", &eventDescription));
        ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
        play();
        update();

    }              
    void SoundSystem::sonido_boss_danyo(){
        ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Enemigos/Boss/Boss_danyo", &eventDescription));
        ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
        play();
        update();

    }              
    void SoundSystem::sonido_boss_subdito_danyo(){
        ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Enemigos/Boss/Boss_Sub_danyo", &eventDescription));
        ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
        play();
        update();

    }
    void SoundSystem::sonido_subdito_muere(){
        ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Enemigos/Boss/Subdito_muere", &eventDescription));
        ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
        play();
        update();

    }        
    void SoundSystem::sonido_boss_muere(){
        ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Enemigos/Boss/Boss_muere", &eventDescription));
        ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
        play();
        update();

    }           
    void SoundSystem::sonido_inicio_batalla(){
        ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Enemigos/Boss/Boss_inicio", &eventDescription));
        ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
        play();
        update();


    }       
    void SoundSystem::sonido_cambio_fase(){
        ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Enemigos/Boss/Boss_fase", &eventDescription));
        ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
        play();
        update();

    }          
    void SoundSystem::sonido_win(){
        ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Feedback/Ganar", &eventDescription));
        ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
        play();
        update();

    }                  
    void SoundSystem::sonido_jugador_muere(){
        ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Jugador_muere", &eventDescription));
        ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
        play();
        update();

    }


    //PASOS

    void SoundSystem::sonido_pasos_pradera() {
        ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Pasos/Pasos_pradera", &eventDescription_pasos));
        ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription_pasos, &eventInstance_SFX_pasos));
     }

    void SoundSystem::sonido_pasos_prision() {
        ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Pasos/Pasos_prision", &eventDescription_pasos));
        ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription_pasos, &eventInstance_SFX_pasos));
    }

    void SoundSystem::sonido_pasos_volcan() {
        ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Pasos/Pasos_volcan", &eventDescription_pasos));
        ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription_pasos, &eventInstance_SFX_pasos));
    }
    void SoundSystem::sonido_pasos_nieve() {
        ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Pasos/Pasos_monte", &eventDescription_pasos));
        ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription_pasos, &eventInstance_SFX_pasos));
    }

    void SoundSystem::sonido_pasos_madera(){
        ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Jugador/Pasos/Pasos_biblioteca", &eventDescription_pasos));
        ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription_pasos, &eventInstance_SFX_pasos));
    }               

  
    
    

//DIALOGOS
void SoundSystem::sonido_DPlayer() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Dialogos/jugador/jugador_voz", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}

void SoundSystem::sonido_DCalabaza() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Dialogos/Calabaza/calabaza_voz", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}

void SoundSystem::sonido_DInvestigador() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Dialogos/Investigador/investigador_voz", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}

void SoundSystem::sonido_DEsquimal() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Dialogos/Esquimal", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}

void SoundSystem::sonido_DPescador() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Dialogos/Pescador/pescador_voz", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}


void SoundSystem::sonido_DSacerdotisa() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Dialogos/Sacerdotisa/sacerdotisa_voz", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}
void SoundSystem::sonido_DEspiritu() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Dialogos/Espiritu/espiritu_voz", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}

void SoundSystem::sonido_DGuerrera() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Dialogos/Guerrera/guerrera_voz", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}

void SoundSystem::sonido_DMaestro() {
    ERRCHECK(FMOD_Studio_System_GetEvent(soundSystem, "event:/Efectos/Dialogos/Maestro", &eventDescription));
    ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDescription, &eventInstance));
    play();
    update();
}








//ACCIONAR O PARAR EVENTOS

    //accionar eventos SFX
void SoundSystem::play() {
    FMOD_Studio_EventInstance_Start(eventInstance);
}

    //accionar evento de pasos
void SoundSystem::play_pasos() {
    FMOD_Studio_EventInstance_Start(eventInstance_SFX_pasos);
    update();
}
    //accionar musica menu
void SoundSystem::play_music() {
    FMOD_Studio_EventInstance_Start(eventInstance_Musica);
    update();
}
    //accionar musica de nivel
void SoundSystem::play_music_level() {
    FMOD_Studio_EventInstance_Start(eventInstance_Musica_Level);
    update();
}


    //parar musica menu
void SoundSystem::music_stop() {
    FMOD_Studio_EventInstance_Stop(eventInstance_Musica, FMOD_STUDIO_STOP_ALLOWFADEOUT);
}
    //parar musica nivel
void SoundSystem::music_stop_level() {
    FMOD_Studio_EventInstance_Stop(eventInstance_Musica_Level, FMOD_STUDIO_STOP_ALLOWFADEOUT);
}

    //parar ambiente
void SoundSystem::ambient_stop() {
    FMOD_Studio_EventInstance_Stop(eventInstance_Ambiente, FMOD_STUDIO_STOP_ALLOWFADEOUT);
    FMOD_Studio_EventInstance_Stop(eventInstance_Ambiente_volcan, FMOD_STUDIO_STOP_ALLOWFADEOUT);
    // update();
}


    //parar pasos
void SoundSystem::SFX_pasos_stop() {
    FMOD_Studio_EventInstance_Stop(eventInstance_SFX_pasos, FMOD_STUDIO_STOP_ALLOWFADEOUT);
    update();
}

    //parar movimiento de enemigos
void SoundSystem::stop_golem_mov( FMOD_STUDIO_EVENTINSTANCE* sonido){
    FMOD_Studio_EventInstance_Stop(sonido, FMOD_STUDIO_STOP_ALLOWFADEOUT);
    update();

}

void SoundSystem::stop_munyeco_mov(FMOD_STUDIO_EVENTINSTANCE*& sonido){
    FMOD_Studio_EventInstance_Stop(sonido, FMOD_STUDIO_STOP_ALLOWFADEOUT);
    update();
}



//FUNCIONES PARA LLAMAR PARA MANEJAR LOS EVENTOS DURANTE EL MENU DE PAUSA
void SoundSystem::sonido_pause(int zona){
    FMOD_BOOL pausa {true};

    SFX_pasos_stop();
    getVolumeMov();
    setVolumeMov(0);
    switch (zona)
    {
        case 0:
            ERRCHECK(FMOD_Studio_EventInstance_SetPaused(eventInstance_Ambiente, pausa));
            break;
        case 2:
            ERRCHECK(FMOD_Studio_EventInstance_SetPaused(eventInstance_Ambiente_volcan, pausa));
            ERRCHECK(FMOD_Studio_EventInstance_SetPaused(eventInstance_Musica_Level, pausa));
            break;
        case 3:
            ERRCHECK(FMOD_Studio_EventInstance_SetPaused(eventInstance_Musica_Level, pausa));
            ERRCHECK(FMOD_Studio_EventInstance_SetPaused(eventInstance_Ambiente, pausa));
            break;
        case 4:
            ERRCHECK(FMOD_Studio_EventInstance_SetPaused(eventInstance_Musica_Level, pausa));
            break;
        default:
            ERRCHECK(FMOD_Studio_EventInstance_SetPaused(eventInstance_Ambiente, pausa));
            ERRCHECK(FMOD_Studio_EventInstance_SetPaused(eventInstance_Musica_Level, pausa));
            break;
    }
    if(!music_started){
        playMusicMenu();
        music_started = true;
    }

}
void SoundSystem::sonido_unpause(int zona){
    FMOD_BOOL despausa {false};
    setVolumeMov(movVolume);
    switch (zona)
    {
        case 0:
            ERRCHECK(FMOD_Studio_EventInstance_SetPaused(eventInstance_Ambiente, despausa));
            break;
        case 2:
            ERRCHECK(FMOD_Studio_EventInstance_SetPaused(eventInstance_Ambiente_volcan, despausa));
            ERRCHECK(FMOD_Studio_EventInstance_SetPaused(eventInstance_Musica_Level, despausa));
            break;
        case 3:
            ERRCHECK(FMOD_Studio_EventInstance_SetPaused(eventInstance_Musica_Level, despausa));
            ERRCHECK(FMOD_Studio_EventInstance_SetPaused(eventInstance_Ambiente, despausa));
            break;
        case 4:
            ERRCHECK(FMOD_Studio_EventInstance_SetPaused(eventInstance_Musica_Level, despausa));
            break;
        default:
            ERRCHECK(FMOD_Studio_EventInstance_SetPaused(eventInstance_Ambiente, despausa));
            ERRCHECK(FMOD_Studio_EventInstance_SetPaused(eventInstance_Musica_Level, despausa));
            break;
    }

     if(music_started){
        music_stop();
        music_started = false;
    }

}



//CONFIGURACION DE VOLUMENES
float SoundSystem::getVolumeMaster() {
    float currentVolume;
    ERRCHECK(FMOD_ChannelGroup_GetVolume(masterGroup, &currentVolume));

    return currentVolume;
}


float SoundSystem::getVolumeSFX() {
    float currentVolume, finalvolume;
    ERRCHECK(FMOD_Studio_Bus_GetVolume(sfxBus, &currentVolume, &finalvolume));
    return currentVolume;
}

float SoundSystem::getVolumeMusic(){
    float currentVolume, finalvolume;
    ERRCHECK(FMOD_Studio_Bus_GetVolume(MusicBus, &currentVolume, &finalvolume));
    return currentVolume;
}
float SoundSystem::getVolumeAmbient(){
    float currentVolume, finalvolume;
    ERRCHECK(FMOD_Studio_Bus_GetVolume(AmbientBus, &currentVolume, &finalvolume));
    return currentVolume;
}

void SoundSystem::setVolumeMaster(float volumen) {
    //REVISAR, ESTO SE DECLARA CREAR EL SYSTEM
    FMOD_System_GetMasterChannelGroup(coreSystem, &masterGroup);


    ERRCHECK(FMOD_ChannelGroup_SetVolume(masterGroup, volumen));
}

void SoundSystem::setVolumeSFX(float volumen){
    ERRCHECK( FMOD_Studio_Bus_SetVolume( sfxBus,volumen ));
}

void SoundSystem::setVolumeMusic(float volumen){
    ERRCHECK( FMOD_Studio_Bus_SetVolume( MusicBus,volumen ));
}

void SoundSystem::setVolumeAmbient(float volumen){
    ERRCHECK( FMOD_Studio_Bus_SetVolume( AmbientBus,volumen ));
}


void SoundSystem::getVolumeMov(){
        float currentVolume, finalvolume;
    ERRCHECK(FMOD_Studio_Bus_GetVolume( sfxBus, &currentVolume, &finalvolume ));
    movVolume = currentVolume;
}
void SoundSystem::setVolumeMov(float volumen){
    //float cambiar_sonido = 0;
    //if(volumen != 0)
    //    cambiar_sonido = movVolume;
    ERRCHECK( FMOD_Studio_Bus_SetVolume( MovBus,volumen ));
}



void SoundSystem::muteMaster(){
    generalVolume = getVolumeMaster();
    setVolumeMaster(0);
}

void SoundSystem::muteAmbient(){
    ambientVolume = getVolumeAmbient();
    setVolumeAmbient(0);

}
void SoundSystem::muteMusic(){
    musicVolume = getVolumeMusic();
    setVolumeMusic(0);

}
void SoundSystem::muteSFX(){
    sfxVolume = getVolumeSFX();
    setVolumeSFX(0);

}


//UPDATE Y DESCARGA DE BANCOS
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