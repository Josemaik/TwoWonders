
<br />
<p align="center">
  <img width="3840" height="1240" alt="MainKeyArt" src="/assets/Principal_fondo.png" />
  <br />
  <h1 align="center">🧙 Two Wonders 🧙
	<br /><br />
    <p>
      <a href="https://itch.io/your-game-or-page" target="_blank" rel="noopener noreferrer">
  <img src="https://img.shields.io/badge/Find%20us%20on-Itch.io-111111?style=for-the-badge&logo=itchdotio&logoColor=white" alt="Find us on Itch.io" />
    </a>
    </p>
  </h1>
</p>

Two Wonders is a 3D action-adventure game with an isometric view, inspired by games like Tunic or The Legend of Zelda, where exploration is key element.

The game combines strategic puzzle-solving with elemental combat, where players must use different elemental abilities to defeat enemies, each possessing unique elemental strengths and weaknesses.

<!-- <img src="https://github.com/user-attachments/assets/4aee6508-691b-43ef-b264-813fc3abeb55" width="400">
<img src="https://github.com/user-attachments/assets/cec211ad-5ac2-4f94-b322-ba4f90247155" width="400">
<img src="https://github.com/user-attachments/assets/8d223ce6-e1df-46c6-a53f-65ac19cfe8fe" width="400">
<img src="https://github.com/user-attachments/assets/f140a235-c668-46b4-8b40-988defa0bb5a" width="400"> -->

### 🛠️ Contributions - Custom Engine
---
## ECS Engine

I worked on the implementation of the ECS, specifically in the construction of SlotMap matrix structure that would allow us to store the data of the components that each entity could have in binary key system. Then, we implement entity manager which contains functions related to the managment of entities like their Creation, addition of components or tags and their destruction.

[EntityManager](src/managers/event_manager.hpp) | [SlotMap](src/utils/slotmap.hpp) | [MetaProgram](src/utils/meta_program.hpp) | [Components](src/components) | [Systems](src/systems) | [Game](src/managers/game.hpp) | [Singletons](src/utils/sngtn)


## AI Engine

I developed an AI engine from scratch in c++ which would be in charge of processing different entities such as enemies or npcs and giving them behaviors according to their previous design.

[AIComponent](src/components/ai_component.hpp) | [AIManager](src/managers/ia_manager.hpp) | [AISystem](src/systems/ai_system.hpp)

[BehaviorTrees](src/utils/bt) | [Blackboard](src/utils/sngtn/blackboard.hpp) | [PathFinding](src/utils/pf) | [SensorialPerception](src/utils/sp/SensorialPerception.hpp) | Parameter Editor and Debugger [RenderSystem](src/systems/render_system.hpp)

## Skelal Animation System

I collaborated in the implementation of the animation pipeline that uses Assimp to import animation data and a custom runtime system to play and blend animations.

[EntityModel](src/motor/src/components/entity_model.hpp) | [Bone and BoneInfo Class](src/motor/src/utils) | [AnimatorManager](src/motor/src/managers/animator_manager.hpp) | [AnimationComponent](src/components/animation_component.hpp) | [AnimationSystem](src/systems/animation_system.hpp)

## Event Manager and Trigger System

The event system implements a simple queued event dispatcher and a thin system wrapper to perform game-specific reactions like spawning objects, adding text messages, playing sounds, teleporting NPCs, setting level flags, and scheduling further events.  

[EventManager](src/managers/event_manager.hpp) | [ListenerComponent](src/components/listener_component.hpp) | [EventSystem](src/systems/event_system.hpp)

### 🤝 Acknowledgements
---

All the playtesters who gave feedback on the game, from the earliest state to the end.<br />
Everyone at Kaiwa Games who made this game possible.
