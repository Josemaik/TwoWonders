#pragma once

namespace MP
{
    // Define constant value for compile-time
    template <typename T, T VAL>
    struct constant { static constexpr T value{ VAL }; };
    struct true_type : constant<bool, true> {};
    struct false_type : constant<bool, false> {};


    // Forward declaration del Typelist
    template <typename...Ts>
    struct TypeList;


    // is_same - Check if two types are the same
    template <typename T, typename U>
    struct is_same : false_type {};

    template <typename T>
    struct is_same<T, T> : true_type {};

    template <typename T, typename U>
    constexpr bool is_same_v{ is_same<T, U>::value };


    // type_id - Get the type of a type
    template <typename T>
    struct type_id { using type = T; };


    // nth_type - Get the nth type of a typelist
    template <std::size_t N, typename... Ts>
    struct nth_type { static_assert(sizeof...(Ts) > 0, "TypeList is empty"); };

    template <std::size_t N, typename...Ts>
    using nth_type_t = typename nth_type<N, TypeList< Ts... >>::type;

    template <typename T, typename... Ts>
    struct nth_type<0, T, Ts...> : type_id< T > {};

    template <std::size_t N, typename T, typename... Ts>
    struct nth_type<N, T, Ts...> : type_id< nth_type_t<N - 1, Ts...>> {};


    // pos - Get the position of a type in a typelist
    template <typename T, typename... Ts>
    struct pos { static_assert(sizeof...(Ts) > 0, "TypeList is empty"); };

    template <typename T, typename... Ts>
    constexpr std::size_t pos_v{ pos<T, Ts...>::value };

    template <typename T, typename... Ts>
    struct pos<T, T, Ts...> : constant<std::size_t, 0> {};

    template <typename T, typename U, typename... Ts>
    struct pos<T, U, Ts...> : constant<std::size_t, 1 + pos_v<T, Ts... >> {};


    // IF Type or Another Type
    template <bool Condition, typename T, typename F>
    struct IFT : type_id < F > {};

    template <typename T, typename F>
    struct IFT<true, T, F> : type_id < T > {};

    template <bool Condition, typename T, typename F>
    using IFT_t = typename IFT<Condition, T, F>::type;


    // TypeList type
    template <typename...Ts>
    struct TypeList {
        consteval static std::size_t size() noexcept { return sizeof...(Ts); }

        template <typename T>
        consteval static bool contains() noexcept { return (false || ... || is_same_v<T, Ts>); }

        template <typename T>
        consteval static std::size_t pos() noexcept
        {
            static_assert(contains<T>(), "Type not found");
            return pos_v<T, Ts...>;
        }
    };

    // Replace type with another
    template <template <typename...> class New, typename List>
    struct replace {};

    template <template <typename...> class New, typename... Ts>
    struct replace<New, TypeList<Ts...>> : type_id < New<Ts...> > {};

    template <template <typename...> class New, typename List>
    using replace_t = typename replace<New, List>::type;


    // For all insert template
    template <template <typename> class New, typename List>
    struct forall_insert_template {};

    template <template <typename> class New, typename... Ts>
    struct forall_insert_template<New, TypeList<Ts...>> : type_id<TypeList<New<Ts>...>> {};

    template <template <typename> class New, typename List>
    using forall_insert_template_t = typename forall_insert_template<New, List>::type;


    // Get the smallest type
    template <typename LIST>
    using smallest_type = IFT_t<LIST::size() <= 8, std::uint8_t,
        IFT_t<LIST::size() <= 16, std::uint16_t,
        IFT_t<LIST::size() <= 32, std::uint32_t,
        std::uint64_t>>>;

} // namespace MP