#pragma once
#ifndef ENTITY_MANAGER
#define ENTITY_MANAGER
#include <array>
#include <cstdint>
#include <span>
#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>
#include <typeinfo>
#include <set>
#include <functional>
#include "../utils/slotmap.hpp"
#include "../utils/meta_program.hpp"

// Plantilla para obtener la información de los tags
template <typename TAGS>
struct tag_traits : MP::cmp_tag_traits<TAGS> {};

// Plantilla para obtener la información de los componentes
template <typename CMPS>
struct cmp_traits : tag_traits<CMPS> {};

namespace ETMG {

    template <typename CMPList, typename SNGCMPLIST, typename TAGList, std::size_t SlotCapacity = 200>

    struct EntityManager
    {
        // Forward declarations - Se declara la clase Entity antes de que se defina
        // así se puede usar en algunos lados sin que de errores antes de definirlo
        struct Entity;

        // CONSTANTES
        //
        static constexpr std::size_t MAX_ENTITIES{ 200 };

        // VARIABLES ESTÁTICAS
        //
        inline static std::size_t nextID{ 0 }; // ID de la siguiente entidad a crear

        // ALIAS
        //
        using deathSet = std::set<std::size_t, std::greater<std::size_t>>;
        using TypeProcessFunc = void(*)(Entity&); // Alias para un puntero a función (lambda)
        using tag_info = tag_traits<TAGList>; // Alias para la información de los tags
        using cmp_info = cmp_traits<CMPList>; // Alias para la información de los componentes

        // Alias para reemplazar directamente un TypeList por una tupla
        template <typename List>
        using tuple_replace = MP::replace_t<std::tuple, List>;
        // tipo para Tupla de componentes singlenton
        using singlestorage_t = MP::replace_t<std::tuple, SNGCMPLIST>;
        // Alias para convertir un TypeList<T, U, V, ...> a TypeList<Slotmap<T, 10>, Slotmap<U, 10>, Slotmap<V, 10>, ...>
        template<typename Type>
        using to_slotmap = Slotmap<Type, SlotCapacity>;

        // Alias para sacar el tipo tupla donde guardaremos los componentes
        using storage_type = tuple_replace<MP::forall_insert_template_t<to_slotmap, CMPList>>;

        // Alias para sacar el tipo de dato de la key de cualquier Slotmap
        template <typename CMPType>
        using to_keytype = typename Slotmap<CMPType, SlotCapacity>::key_type;

        // Clase Entity
        struct Entity
        {
            friend struct EntityManager<CMPList, SNGCMPLIST, TAGList, SlotCapacity>;
            // Sacamos TypeList para asociar cada componente con su key
            using keytype_list = MP::forall_insert_template_t<to_keytype, CMPList>;
            // Tupla de los tipos de las keys de los componentes
            using key_storage_type = tuple_replace<keytype_list>;

            Entity() { id_ = nextID++; }

            // Plantilla para saber si la entidad tiene un componente del tipo CMP
            template <typename CMP>
            bool hasComponent() const noexcept
            {
                // Devuelve true si la máscara de componentes tiene el bit correspondiente al componente CMP
                return cmp_mask_ & cmp_info::template mask<CMP>();
            }

            // Plantilla para saber si la entidad tiene un tag del tipo TAG
            template <typename TAG>
            bool hasTag() const noexcept
            {
                // Devuelve true si la máscara de tags tiene el bit correspondiente al tag TAG
                return tag_mask_ & tag_info::template mask<TAG>();
            }

            std::size_t  getID() const noexcept { return id_; }

        private:
            // Plantilla para obtener la clave de un componente del tipo CMP
            template <typename CMP>
            to_keytype<CMP> getComponentKey() const
            {
                assert(hasComponent<CMP>());
                return std::get< to_keytype<CMP> >(cmp_keys_);
            }

            // Resetea la entidad
            void reset() noexcept
            {
                cmp_mask_ = 0;
                tag_mask_ = 0;
                cmp_keys_ = {};
            }

            // Asigna un nuevo ID a la entidad
            void setID(std::size_t const id) noexcept { this->id_ = id; }

            // Plantilla para añadir un componente a la entidad
            template <typename CMP>
            void addComponent(to_keytype<CMP> key)
            {
                // Actualiza la máscara de componentes para indicar que esta entidad ahora tiene un componente del tipo CMP
                cmp_mask_ |= cmp_info::template mask<CMP>();

                // Almacena la clave para el componente del tipo CMP
                std::get< to_keytype<CMP> >(cmp_keys_) = key;
            }

            // Plantilla para añadir un tag a la entidad
            template <typename TAG>
            void addTag()
            {
                // Se actualiza la máscara de tags para indicar que esta entidad ahora tiene un tag del tipo TAG
                tag_mask_ |= tag_info::template mask<TAG>();
            }

            std::size_t id_{}; // ID de la entidad
            typename cmp_info::mask_type cmp_mask_{}; // Máscara de componentes
            typename tag_info::mask_type tag_mask_{}; // Máscara de tags
            key_storage_type cmp_keys_{}; // Tupla de las claves de los componentes
        };

        // FUNCIONES
        //
        //
        // Plantilla para añadir un tag a una entidad del EntityManager
        template <typename TAG>
        void addTag(Entity& e)
        {
            if (e.template hasTag<TAG>()) return;

            e.template addTag<TAG>();
        }

        // Plantilla para añadir un componente a una entidad del EntityManager
        template <typename CMP, typename... InitTypes>
        CMP& addComponent(Entity& e, InitTypes&&... args)
        {
            // Revisamos si ya tiene el componente
            if (e.template hasComponent<CMP>())
            {
                return getComponent<CMP>(e);
            }

            // Si no lo tiene, lo creamos
            return createComponent<CMP>(e, std::forward<InitTypes>(args)...);
        }

        // Función para crear una entidad y nos devuelve su referencia por parámetro
        Entity& newEntity() noexcept
        {
            assert(alive_ < MAX_ENTITIES);
            alive_ += 1;
            return entities_[alive_ - 1];
        }

        // Plantilla para recibir un componente específico de una entidad
        template <typename CMP, typename EntityT>
        auto& getComponent(EntityT& e)
        {
            auto key = e.template getComponentKey<CMP>();
            auto& storage = getCMPStorage<CMP>();
            return storage[key];
        }

        // Plantilla para recorrer todas las entidades que tengan los componentes y tags especificados
        template <typename CMPs, typename TAGs>
        void forEach(auto&& func) { forEachImpl(func, CMPs{}, TAGs{}); }

        // Plantilla para recorrer todas las entidades que tengan algunos de los componentes y tags especificados
        template <typename CMPs, typename TAGs>
        void forEachAny(auto&& func) { forEachImplAny(func, CMPs{}, TAGs{}); }

        // Función que nos devuelve el número de entidades libres sin asignar en el EntityManager
        std::size_t freeEntities() const noexcept { return MAX_ENTITIES - alive_; }

        // Función que nos devuelve las entidades vivas
        template<typename T = Entity>
        auto getEntities() -> std::span<std::conditional_t<std::is_const_v<T>, const Entity, Entity>>
        {
            return std::span{ entities_.begin(), entities_.begin() + alive_ };
        }

        // Función que nos devuelve una tupla de componentes singleton
        template<typename SCMP, typename T = SCMP>
        [[nodiscard]] constexpr auto& getSingleton() noexcept
        {
            using ReturnType = std::conditional_t<std::is_const_v<T>, const SCMP, SCMP>;
            return std::get<ReturnType>(singletonCMPTuple_);
        }


        // Obtener entidad por su ID
        Entity* getEntityByID(std::size_t const id) noexcept
        {
            auto it = std::ranges::find_if(
                entities_
                , [=](Entity const& e) { return e.getID() == id; }
            );
            if (it != entities_.end())
                return &(*it); // Devuelve la direccion de memoria a la que apunta el iterador
            return nullptr;
        }

        // Despeja todas las entidades
        void destroyAll() { while (alive_ > 0) destroyEntity(0); }

        // Despeja un conjunto de entidades
        void destroyEntities(deathSet const& set)
        {
            // Destroy the entities
            for (auto const& id : set)
            {
                destroyEntity(id);
            }
        }

        // Plantilla para destruir un componente de una entidad
        template<typename CMP>
        void destroyComponent(Entity& e) {
            if (e.template hasComponent<CMP>()) {
                if constexpr (std::is_same_v<CMP, RenderComponent>)
                    getComponent<RenderComponent>(e).destroyMesh();

                auto key = e.template getComponentKey<CMP>();
                this->template getCMPStorage<CMP>().erase(key);
            }
        }

    private:
        // Plantilla para sacar una lista de componentes de la tupla que los guarda
        template <typename CMP>
        [[nodiscard]] constexpr auto& getCMPStorage() noexcept
        {
            constexpr auto id{ cmp_info::template id < CMP>() };
            return std::get< id >(CMPTuple_);
        }

        // Función para destruir una entidad
        void destroyEntity(size_t index)
        {
            assert(index < alive_);
            assert(alive_ > 0);
            auto& e = entities_[index];

            // Eliminamos los componentes de la entidad
            destroyCMPs(e);

            // Reseteamos la entidad
            e.reset();

            // Eliminamos la entidad
            e = entities_[alive_ - 1];
            entities_[alive_ - 1].reset();
            e.setID(index);

            alive_ -= 1;
        }

        // Plantilla para recorrer todas las entidades que tengan los componentes y tags especificados
        template <typename... CMPs, typename... TAGs>
        void forEachImpl(auto&& func, MP::TypeList<CMPs...>, MP::TypeList<TAGs...>)
        {
            for (Entity& e : getEntities())
            {
                auto hasCMPs = (true && ... && e.template hasComponent<CMPs>());
                auto hasTAGs = (true && ... && e.template hasTag<TAGs>());
                if (hasCMPs && hasTAGs)
                {
                    func(e, getComponent<CMPs>(e)...);
                }
            }
        }

        // Plantilla para recorrer todas las entidades que tengan algunos de los componentes y tags especificados
        template <typename... CMPs, typename... TAGs>
        void forEachImplAny(auto&& func, MP::TypeList<CMPs...>, MP::TypeList<TAGs...>)
        {
            for (Entity& e : getEntities())
            {
                auto hasAnyCMPs = (false || ... || e.template hasComponent<CMPs>());
                auto hasAnyTAGs = (false || ... || e.template hasTag<TAGs>());
                if (hasAnyCMPs || hasAnyTAGs)
                {
                    func(e, getComponent<CMPs>(e)...);
                }
            }
        }

        // Plantilla para tener la creación de Componentes y Tags privada, solo manejada por el EntityManager
        template <typename CMP, typename... InitTypes>
        CMP& createComponent(Entity& e, InitTypes&&... args)
        {
            // Create new component
            auto& storage = getCMPStorage<CMP>();
            auto key = storage.push_back(CMP{ std::forward<InitTypes>(args)... });
            e.template addComponent<CMP>(key);

            return storage[key];
        }

        // Destruye todos los componentes de una entidad
        void destroyCMPs(Entity& e)
        {
            tuple_replace<CMPList> cmps{};
            MP::for_each_in_tuple(cmps, [&](auto cmpType)
            {
                using CMPType = decltype(cmpType);
                destroyComponent<CMPType>(e);
            });
        }

        std::size_t alive_{};
        std::array<Entity, MAX_ENTITIES> entities_{};

        storage_type CMPTuple_{};
        singlestorage_t singletonCMPTuple_{};
    };

#endif // !ENTITY_MANAGER
} // namespace ETMG
