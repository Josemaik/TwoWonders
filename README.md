
<br />
<p align="center">
  <img width="3840" height="1240" alt="MainKeyArt" src="/assets/Principal_fondo.png" />
  <br />
  <h1 align="center">🧙 Two Wonders 🧙
	<br /><br />
    <p>
      <a href="https://kaiwagames.itch.io/two-wonders" target="_blank" rel="noopener noreferrer">
  <img src="https://img.shields.io/badge/Find%20us%20on-Itch.io-111111?style=for-the-badge&logo=itchdotio&logoColor=white" alt="Find us on Itch.io" />
    </a>
    </p>
  </h1>
</p>

Two Wonders is a 3D action-adventure game with an isometric view, inspired by games like Tunic or The Legend of Zelda, where exploration is key element.

The game combines strategic puzzle-solving with elemental combat, where players must use different elemental abilities to defeat enemies, each possessing unique elemental strengths and weaknesses.













<div align="center" style="display: flex; justify-content: center; gap: 10px; flex-wrap: wrap;">
  <video src="https://github.com/user-attachments/assets/dbb5e6b1-cd3c-48a3-a1ef-ef1b57cb5153" width="100" controls style="border: none; outline: none; border-radius: 10px;"></video>
  <video src="https://github.com/user-attachments/assets/cc0e609c-431a-4a9c-9fee-fec2ccaea5f2" width="400" controls style="border: none; outline: none; border-radius: 10px;"></video>
  <video src="https://github.com/user-attachments/assets/da6d4903-49d3-4f2f-88a9-979b2731c11f" width="400" controls style="border: none; outline: none; border-radius: 10px;"></video>
</div>

### 🛠️ Contributions - Custom Engine
---
## ECS Engine

I worked on the implementation of the ECS, specifically in the construction of SlotMap matrix structure that would allow us to store the data of the components that each entity could have in binary key system. Then, we implement entity manager which contains functions related to the managment of entities like their Creation, addition of components or tags and their destruction.

[EntityManager](src/managers/event_manager.hpp) | [SlotMap](src/utils/slotmap.hpp) | [MetaProgram](src/utils/meta_program.hpp) | [Components](src/components) | [Systems](src/systems) | [Game](src/managers/game.hpp) | [Singletons](src/utils/sngtn)


## AI Engine

I developed an AI engine from scratch in c++ which would be in charge of processing different entities such as enemies or npcs and giving them behaviors according to their previous design.

[AIComponent](src/components/ai_component.hpp) | [AIManager](src/managers/ia_manager.hpp) | [AISystem](src/systems/ai_system.hpp)

[BehaviorTrees](src/utils/bt) | [Blackboard](src/utils/sngtn/blackboard.hpp) | [PathFinding](src/utils/pf) | [SensorialPerception](src/utils/sp/SensorialPerception.hpp) | [Parameter Editor and Debugger - RenderSystem](src/systems/render_system.hpp)

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
