#pragma once
#include <iostream>

struct Resource{
protected:
    std::size_t m_id;        // ID of ResourceManager
    const char* m_filePath;  // FilePath of Resource
    bool m_isLoad;

    virtual void setup(){};

public:
    virtual ~Resource(){};
    virtual bool load(const char* fileName) = 0;
    virtual void unload() = 0;

    // Getters

    bool isLoaded(){ return m_isLoad; };
    std::size_t getID(){ return m_id; };
    const char* getFilePath(){ return m_filePath; };
};