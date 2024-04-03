#pragma once
#include "../resources/resource.hpp"

#include <string>
#include <iostream>

#include <memory>
#include <unordered_map>

struct ResourceManager{
public:

    static ResourceManager& getInstance(){
        static ResourceManager instance;
        return instance;
    }

    ~ResourceManager(){ 
        std::cout << "┌────────┐" << std::endl;
        std::cout << "│ Unload │" << std::endl;
        std::cout << "└────────┘" << std::endl;
        
        unloadAllResources(); 
    };

    template<typename T> 
    T* getResource(const std::size_t& id){
        auto it = m_resources.find(id);
        if(it != m_resources.end())
            return static_cast<T*>(it->second.get());
        
        return nullptr;
    }

    template<typename T, typename... Args> 
    T* loadResource(const char* filePath, Args&&... args){
        // Search if the resource is already loaded
        for (auto& pair : m_resources){
            //std::cout << "-----> " << pair.second->getFilePath() << " | " << filePath << std::endl;
            if (pair.second->getFilePath() == filePath)
                return dynamic_cast<T*>(pair.second.get());
        }

        // If the resource is not in memory, load it
        nextID++;
        auto resource = std::make_unique<T>(nextID, std::forward<Args>(args)...);
        if(resource->load(filePath)){
            auto rawPtr = resource.get();
            m_resources[nextID] = std::move(resource);
            return rawPtr;
        }
        else{
            nextID--;
            return nullptr;
        }
    }

    void unloadResource(const std::size_t& id){
        auto it = m_resources.find(id);
        if(it != m_resources.end())
            m_resources.erase(it);
    }

    void unloadAllResources(){ m_resources.clear(); }

private:
    ResourceManager() = default;

    inline static std::size_t nextID{ 0 };
    std::unordered_map<std::size_t, std::unique_ptr<Resource>> m_resources;
};