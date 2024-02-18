#pragma once
#include "../components/resource.hpp"
#include <string>
#include <memory>
#include <iostream>
#include <unordered_map>

struct ResourceManager{
public:
    template<typename T> T& getResource(const std::string& id){
        return *static_cast<T*>(m_resources[id].get());
    }

    template<typename T, typename... Args> T& loadResource(const std::string& id, Args&&... args){
        auto resource = std::make_shared<T>(std::forward<Args>(args)...);
        if(resource->load()){
            m_resources[id] = resource;
            return *resource;
        }
        else
            throw std::runtime_error("Error loading resource");
    }

    void unloadResource(const std::string& id){
        auto it = m_resources.find(id);
        if(it != m_resources.end()){
            it->second->unload();
            m_resources.erase(it);
        }
    }

private:
    std::unordered_map<std::string, std::shared_ptr<Resource>> m_resources;  
};