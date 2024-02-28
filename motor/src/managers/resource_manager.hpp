#pragma once
#include "../components/resource.hpp"

#include <string>
#include <memory>
#include <iostream>
#include <unordered_map>

struct ResourceManager{
public:

    inline static std::size_t nextID{ 0 };

    template<typename T> T& getResource(const std::size_t& id){
        // resource is null -> loadResource
        return *static_cast<T*>(m_resources[id].get());
    }

    template<typename T, typename... Args> std::shared_ptr<T> loadResource(Args&&... args){
        nextID++;
        auto resource = std::make_shared<T>(nextID, std::forward<Args>(args)...);
        if(resource->load()){
            m_resources[nextID] = resource;
            return resource;
        }
        else
            throw std::runtime_error("Error loading resource");
    }

    void unloadResource(const std::size_t& id){
        auto it = m_resources.find(id);
        if(it != m_resources.end()){
            it->second->unload();
            m_resources.erase(it);
        }
    }

    void unloadAllResources(){
        for (auto& pair : m_resources)
            pair.second->unload();
        m_resources.clear();
    }

private:
    std::unordered_map<std::size_t, std::shared_ptr<Resource>> m_resources;  
};