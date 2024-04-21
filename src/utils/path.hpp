#pragma once
#include "vec3D.hpp"
#include <cstdio>

template<std::size_t N>
struct Path_t{
    struct iterator {
        vec3d &operator*() {
            if( data == nullptr ) return zero;
            return (*data)[i];
        };
        iterator &operator++() {
            if ( ++i >= N) i = 0;
            return *this;
        }
        // iterator &operator--() {
        //     if( --i < 0) ++i;
        //     return *this;
        // }
        bool operator!=(iterator const& oit) {
            return i != oit.i;
        }
        inline static vec3d zero{};
        std::size_t i{};
        std::array<vec3d,N>* data {};
    };

    iterator begin() { return { 0 , &m_waypoints }; }
    iterator end()   { return {N-1, &m_waypoints }; }

    std::array<vec3d,N> m_waypoints;
};
