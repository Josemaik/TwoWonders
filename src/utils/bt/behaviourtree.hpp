#pragma once
#include<vector>
#include <memory>
#include <cstddef>
#include "node.hpp"

struct BehaviourTree_t{
    //Types
    using value_type = std::unique_ptr<BTNode_t,BTNode_t::Deleter>;
    using NodeStorage_t = std::vector<value_type>; 
    using MemoryStorage_t = std::unique_ptr<std::byte[]>;
    //Constructor
    explicit BehaviourTree_t() {}
    void run() noexcept{
        if ( nodes.size() > 0 ){
            nodes.back()->run();
        }
    }
    template <typename NodeType, typename...ParamTypes>
    NodeType& createNode(ParamTypes&&... params){
        //Reservar memoria
        ptr_reserved -= sizeof(NodeType);
        if ( ptr_reserved < mem.get() ){
            ptr_reserved += sizeof(NodeType);
            throw std::bad_alloc{};
        }

        //Create Node
        auto* pnode = new (ptr_reserved) NodeType{ std::forward<ParamTypes>(params)... };
        nodes.emplace_back( pnode );
        //Return node
        return *pnode;
    }
private:
    std::size_t     mem_size    { 1024 };
    MemoryStorage_t mem         { std::make_unique<std::byte[]>(mem_size) };
    std::byte*      ptr_reserved{ mem.get() + mem_size };
    NodeStorage_t   nodes       {};
};