#pragma once
#include <array>
#include <cstdint>
#include <stdexcept>
#include <cassert>
#include <span>

template <typename T, std::size_t Capacity = 10, typename INDEXT = std::uint32_t>
struct Slotmap
{
    using data_type = T;
    using index_type = INDEXT;
    using gen_type = index_type;
    using key_type = struct { index_type id; gen_type gen; };
    template <typename U>
    using content_type = std::array<U, Capacity>;
    using span_type = std::span<data_type>;
    using const_span_type = std::span<const data_type>;

    constexpr explicit Slotmap() noexcept { clear(); }

    // Pillamos el número de componentes ocupados
    [[nodiscard]] constexpr std::size_t getSize() const noexcept { return size_; }
    // Pillamos el tamaño del array
    [[nodiscard]] constexpr std::size_t getCapacity() const noexcept { return Capacity; }

    [[nodiscard]] constexpr key_type push_back(data_type&& newVal)
    {
        auto reservedId = allocate();
        auto& slot = indexes_[reservedId];

        // Asignamos al array de datos
        data_[slot.id] = std::move(newVal);
        erase_[slot.id] = reservedId;

        // Key para el usuario
        auto key{ slot };
        key.id = reservedId;

        return key;
    }

    [[nodiscard]] constexpr key_type push_back(data_type const& newVal)
    {
        return push_back(data_type{ newVal });
    }
    constexpr void clear() noexcept { freelist_init(); }

    constexpr bool erase(key_type key) noexcept
    {
        if (!is_valid(key)) return false;
        free(key);
        return true;
    }

    [[nodiscard]] constexpr bool is_valid(key_type key) const noexcept
    {
        if (key.id >= Capacity || indexes_[key.id].gen != key.gen)
            return false;
        return true;
    }

    [[nodiscard]] constexpr span_type getData() noexcept { return std::span(data_.begin(), size_); }
    [[nodiscard]] constexpr const_span_type getData() const noexcept { return std::span(data_.cbegin(), size_); }

private:
    [[nodiscard]] constexpr index_type allocate()
    {
        if (size_ >= Capacity) throw std::runtime_error("No more space in the slotmap");
        assert(freelist_ < Capacity);

        // Reservamos el slot
        auto slotId = freelist_;
        freelist_ = indexes_[slotId].id; // freelist -> first free slot

        // Inicializamos el slot
        auto& slot = indexes_[slotId];
        slot.id = size_;
        slot.gen = generation_;

        // Actualizamos size_ y generation_
        size_ += 1;
        generation_ += 1;
        return slotId;
    }

    constexpr void free(key_type key) noexcept
    {
        assert(is_valid(key));

        auto& slot = indexes_[key.id];
        auto dataid = slot.id; // Guardamos el id del dato a borrar
        //Actualizar freelist_
        slot.id = freelist_;
        slot.gen = generation_;
        freelist_ = key.id;

        // Actualizar array de datos si hace falta
        if (dataid != size_ - 1)
        {
            data_[dataid] = data_[size_ - 1];
            erase_[dataid] = erase_[size_ - 1];
            indexes_[erase_[dataid]].id = dataid;
        }

        --size_;
        ++generation_;
    }

    constexpr void freelist_init() noexcept
    {
        for (index_type i{}; i < indexes_.size(); ++i)
        {
            indexes_[i].id = i + 1;
        }
        freelist_ = 0;
    }

    index_type size_{};
    index_type freelist_{};
    gen_type generation_{};
    content_type<key_type> indexes_{};
    content_type<data_type> data_{};
    content_type<index_type> erase_{};
};
