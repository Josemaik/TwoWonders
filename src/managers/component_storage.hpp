#pragma once
#include <tuple>
#include <cstdint>
#include "../utils/slotmap.hpp"


// template <typename CMPList, typename TAGList, std::size_t Capacity = 10>
// struct ComponentStorage
// {
//     using cmp_type = std::uint8_t;
//     using tag_info = tag_traits<TAGList>;
//     using cmp_info = cmp_traits<CMPList>;
//     template <typename List>
//     using tuple_replace = MP::replace_t<std::tuple, List>;
//     template<typename T>
//     using to_slotmap = Slotmap<T, Capacity>;
//     using storage_type = tuple_replace<MP::forall_insert_template_t<to_slotmap, CMPList>>;

//     //static_assert(MP::is_same_v<storage_type, void>);

//     template <typename CMP>
//     [[nodiscard]] constexpr auto& getCMP() noexcept
//     {
//         constexpr auto id{ cmp_info::template id < CMP>() };
//         return std::get< id >(CMPTuple_);
//     }


// private:
//     storage_type CMPTuple_{};
// };