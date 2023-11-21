#pragma once
#ifndef ENTITY_MANAGER
#define ENTITY_MANAGER
#include <array>
#include <cstdint>
#include <span>
#include <cassert>
#include <iostream>
#include <tuple>
#include <vector>

template <typename CMP0, typename CMP1, typename T>
struct EntityManager
{
    static constexpr std::size_t MAX_ENTITIES{ 5 };
    using TypeProcessFunc = void(*)(T&);
    using value_type = T;
    using cmp_type = std::uint32_t;

    constexpr cmp_type getCmpId(CMP0) { return 0; } // 0b0000
    constexpr cmp_type getCmpId(CMP1) { return 1; } // 0b0001
    constexpr cmp_type getCMPMask(std::uint32_t const id)
    {
        return 1 << id;
    }

    template <typename CMP>
    constexpr std::vector<CMP>& getCMP() noexcept
    {
        constexpr auto id = getCmpId(CMP{});
        return std::get<id>(t);
    }

    value_type& newEntity() noexcept
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

    void forAll(TypeProcessFunc f)
    {
        for (value_type& e : getEntities())
        {
            f(e);
        }
    }

    std::size_t freeEntities() const noexcept
    {
        return MAX_ENTITIES - alive_;
    }

    std::span<value_type const> getEntities() const
    {
        return std::span{ entities_.begin(), entities_.begin() + alive_ };
    }

    std::span<value_type> getEntities()
    {
        return std::span{ entities_.begin(), entities_.begin() + alive_ };
    }

private:
    std::size_t alive_{};
    std::array<value_type, MAX_ENTITIES> entities_{};
    std::tuple<std::vector<CMP0>, std::vector<CMP1>> t;
};
#endif // !ENTITY_MANAGER