#pragma once
#ifndef ENTITY_MANAGER
#define ENTITY_MANAGER
#include <array>
#include <cstdint>
#include <span>
#include <cassert>
#include <iostream>
#include <vector>
#include "component_storage.hpp"
#include "../utils/slotmap.hpp"
#include "../utils/meta_program.hpp"

template <typename CMPTAGLIST>
struct cmp_tag_traits {
    static_assert(CMPTAGLIST::size() <= 64, "Component tag list is too large");
    using mask_type = MP::smallest_type<CMPTAGLIST>;

    consteval static uint8_t size() noexcept { return CMPTAGLIST::size(); }

    template <typename CMPTAG>
    consteval static uint8_t id() noexcept
    {
        static_assert(CMPTAGLIST::template contains<CMPTAG>(), "Component tag not found");
        return CMPTAGLIST::template pos<CMPTAG>();
    }

    template <typename... LIST>
    consteval static mask_type mask() noexcept
    {
        return (0 | ... | (1 << id<LIST>()));
    }
};

template <typename TAGS>
struct tag_traits : cmp_tag_traits<TAGS> {};

template <typename CMPS>
struct cmp_traits : tag_traits<CMPS> {};

namespace ETMG {

    template <typename CMPList, typename TAGList, std::size_t SlotCapacity = 5>
    struct EntityManager
    {
        // Forward declarations
        struct Entity;

        static constexpr std::size_t MAX_ENTITIES{ 5 };
        using TypeProcessFunc = void(*)(Entity&);
        using cmp_type = std::uint8_t;
        using tag_info = tag_traits<TAGList>;
        using cmp_info = cmp_traits<CMPList>;
        template <typename List>
        using tuple_replace = MP::replace_t<std::tuple, List>;
        template<typename Type>
        using to_slotmap = Slotmap<Type, SlotCapacity>;
        using storage_type = tuple_replace<MP::forall_insert_template_t<to_slotmap, CMPList>>;
        template <typename CMPType>
        using to_keytype = typename Slotmap<CMPType, SlotCapacity>::key_type;

        template <typename CMP>
        [[nodiscard]] constexpr auto& getCMPStorage() noexcept
        {
            constexpr auto id{ cmp_info::template id < CMP>() };
            return std::get< id >(CMPTuple_);
        }

        struct Entity
        {
            using keytype_list = MP::forall_insert_template_t<to_keytype, CMPList>;
            using key_storage_type = tuple_replace<keytype_list>;

            template <typename CMP>
            void addComponent(to_keytype<CMP> key)
            {
                cmp_mask |= cmp_info::template mask<CMP>();
                std::get< to_keytype<CMP> >(cmp_keys_) = key;
            }

            template <typename CMP>
            bool hasComponent() const noexcept
            {
                return cmp_mask & cmp_info::template mask<CMP>();
            }

            template <typename CMP>
            to_keytype<CMP> getComponentKey() const
            {
                assert(hasComponent<CMP>());
                return std::get< to_keytype<CMP> >(cmp_keys_);
            }

            template <typename TAG>
            void addTag()
            {
                tag_mask |= tag_info::template mask<TAG>();
            }

            template <typename TAG>
            bool hasTag() const noexcept
            {
                return tag_mask & tag_info::template mask<TAG>();
            }

        private:
            std::size_t id{};

            typename cmp_info::mask_type cmp_mask{};
            typename cmp_info::mask_type tag_mask{};
            key_storage_type cmp_keys_{};
            inline static std::size_t nextID{ 1 };
        };

        template <typename TAG>
        void addTag(Entity& e)
        {
            e.template addTag<TAG>();
        }

        template <typename CMP, typename... InitTypes>
        CMP& addComponent(Entity& e, InitTypes&&... args)
        {
            std::cout << "Adding component" << std::endl;
            // Check if component is already present
            if (e.template hasComponent<CMP>())
            {
                std::cout << "Component already present" << std::endl;
                return getComponent<CMP>(e);
            }

            // Create new component
            return createComponent<CMP>(e, std::forward<InitTypes>(args)...);
        }

        Entity& newEntity() noexcept
        {
            assert(alive_ < MAX_ENTITIES);
            alive_ += 1;
            return entities_[alive_ - 1];
        }

        void destroyEntity(size_t index)
        {
            assert(index < alive_);
            assert(alive_ > 0);
            entities_[index] = entities_[alive_ - 1];
            alive_ -= 1;
        }

        template <typename CMP>
        CMP& getComponent(Entity& e)
        {
            auto key = e.template getComponentKey<CMP>();
            auto& storage = getCMPStorage<CMP>();
            return storage[key];
        }

        template <typename CMP>
        CMP const& getComponent(Entity const& e)
        {
            auto key = e.template getComponentKey<CMP>();
            auto& storage = getCMPStorage<CMP>();
            return storage[key];
        }

        template <typename T>
        void forAll(T&& func)
        {
            for (Entity& e : getEntities())
            {
                func(e);
            }
        }

        template <typename CMPs, typename TAGs>
        void forEach(auto&& func)
        {
            forEachImpl(func, CMPs{}, TAGs{});
        }

        std::size_t freeEntities() const noexcept
        {
            return MAX_ENTITIES - alive_;
        }

        std::span<const Entity> getEntities() const
        {
            return std::span{ entities_.begin(), entities_.begin() + alive_ };
        }

        std::span<Entity> getEntities()
        {
            return std::span{ entities_.begin(), entities_.begin() + alive_ };
        }

    private:
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


        template <typename CMP, typename... InitTypes>
        CMP& createComponent(Entity& e, InitTypes&&... args)
        {
            // Create new component
            auto& storage = getCMPStorage<CMP>();
            auto key = storage.push_back(CMP{ std::forward<InitTypes>(args)... });
            e.template addComponent<CMP>(key);

            return storage[key];
        }


        std::size_t alive_{};
        std::array<Entity, MAX_ENTITIES> entities_{};

        storage_type CMPTuple_{};
    };
#endif // !ENTITY_MANAGER

} // namespace ETMG