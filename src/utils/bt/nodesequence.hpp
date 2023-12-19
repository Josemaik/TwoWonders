#pragma once
#include "node.hpp"
#include <utils/types.hpp>
#include <vector>
#include <initializer_list>


struct BTNodeSequence_t : BTNode_t{
    using value_type = BTNode_t*;
    using container_type = std::vector<value_type>;
    using init_list = std::initializer_list<value_type>;
    using iterator  = std::vector<BTNode_t*>::iterator;

    BTNodeSequence_t(init_list l) : nodes{l} {}

    void resetSequence() { currentNodeIt = nodes.begin(); }

    BTNodeStatus_t run(EntityContext_t& ectx) noexcept final { // final es como override sin dejar sobreescribir
        //Si el iterator esta al final de la sequencia de nodos, reseteamos
        if( currentNodeIt == nodes.end() )
            resetSequence();
        //Ejecutamos nodo al que apunta el iterador y obtenemos el estado
        auto status = (*currentNodeIt)->run(ectx);
        switch (status)
        {
            case BTNodeStatus_t::fail:    { resetSequence(); return BTNodeStatus_t::fail;    }
                break;
            case BTNodeStatus_t::running: { return BTNodeStatus_t::running; }
                break;  
            case BTNodeStatus_t::success: {
                ++currentNodeIt;
                if ( currentNodeIt == nodes.end() ) { resetSequence(); return BTNodeStatus_t::success; }
            }
                break;
            default:
                break;
        }
        return BTNodeStatus_t::running;
    }
    
private:
    container_type nodes {};
    iterator currentNodeIt { nodes.begin() };
};