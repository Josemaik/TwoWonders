#pragma once
#include "node.hpp"
#include "entity.hpp"

#include "../resources/resource_mesh.hpp"
#include "../resources/resource_material.hpp"
#include "../managers/resource_manager.hpp"
#include "../managers/render_manager.hpp"

#include <vector>
#include <map>
#include <string>
#include <array>
#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace DarkMoon {
    struct Model : Entity {

    private:
        const char* m_name;
        std::vector<Mesh*> m_meshes;
        bool m_loaded{ false };

        //#############Aniamtion data#############################
        //methods
        void processBone(int index, const aiBone* pbone);
        int getBoneID(const aiBone* pBone);
        //vector of vertex data influence by bone
        std::vector<VertexBoneData> m_Bones{};
        //num of vertex
        std::vector<int> mesh_base_vertex{};
        // Name and id of each bone
        std::map<std::string,uint> m_BoneNameToIndexMap{};
        //##############################################################


        void processNode(aiNode*, const aiScene*, ResourceManager& rm);
        void processMesh(aiMesh*, aiMaterial*, const aiScene*, ResourceManager& rm);
        Material* processMaterial(aiMaterial*, ResourceManager& rm);
        void processTextures(aiMaterial*, Material*, ResourceManager& rm);
        

    public:
        Color color = D_WHITE;
        bool drawModel{ true }, drawWires{ false };

        void load(const char* filePath, ResourceManager& rm);
        void unload() override;
        void draw(glm::mat4) override;

        bool isLoaded() { return m_loaded; };
        void setColor(Color c) { color = c; };

        u_int64_t NumBones() const {
            return m_BoneNameToIndexMap.size();
        }
    };
}