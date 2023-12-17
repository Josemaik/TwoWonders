#pragma once
#include <cstdint>

enum class BTNodeStatus_t : uint8_t{
      success
    , fail
    , running
};

struct BTNode_t{
    //Custom deleter
    struct Deleter { void operator()(BTNode_t* n) {n->~BTNode_t(); } };
    //Constructor por defecto
    BTNode_t() = default;
    virtual ~BTNode_t() = default;
    //No permitir constructores copia ni asignación
    BTNode_t(const BTNode_t&)            = delete;
    BTNode_t(BTNode_t&&)                 = delete;
    BTNode_t& operator=(const BTNode_t&) = delete;
    BTNode_t& operator=(BTNode_t&&)      = delete;
    //Constructor y destructor virtual
    virtual void run() noexcept = 0;
};