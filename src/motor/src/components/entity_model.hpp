#pragma once
#include "node.hpp"
#include "entity.hpp"

#include "../resources/resource_mesh.hpp"
#include "../resources/resource_material.hpp"
#include "../managers/resource_manager.hpp"
#include "../managers/render_manager.hpp"
#include "../utils/BoneInfo.hpp"

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
        struct BoneInfo
        {
            //id of index in finalbonematrices
            int id{};
            //offset matrix transform vertex from model space to bone space
            glm::mat4 offset{};
        };
        //vector of vertex data influence by bone
        // std::vector<Vertex> m_Bones{};
        // // //num of vertex
        // std::vector<int> mesh_base_vertex{};
        // Name and id of each bone
        std::map<std::string,BoneInfo> m_BoneInfomap{};
        int m_BoneCounter { 0 };

        auto& getboneInfoMap() { return m_BoneInfomap; };
        int& getBoneCount() { return m_BoneCounter; };
        void SetVertexBoneDataToDefault(Vertex& vertex){
            for(int i = 0; i < MAX_NUM_BONES_PER_VERTEX;i++){
                vertex.m_BonesIDs[i] = -1;
                vertex.m_Weights[i] = 0.0f;
            }
        }
        void SetVertexBoneData(Vertex& vertex, int bondeID,float weight){
            for(int i = 0; i < MAX_NUM_BONES_PER_VERTEX;i++){
                if(vertex.m_BonesIDs[i] < 0){
                    vertex.m_Weights[i] = weight;
                    vertex.m_BonesIDs[i] = bondeID;
                    break;
                }
            }
        }
        void processBone(const aiBone* pbone,std::vector<Vertex>& vertices);
        // int getBoneID(const aiBone* pBone);
        //##############################################################


        void processNode(aiNode*, const aiScene*, ResourceManager& rm);
        void processMesh(aiMesh*, aiMaterial*, const aiScene*, ResourceManager& rm);
        Material* processMaterial(aiMaterial*, ResourceManager& rm);
        void processTextures(aiMaterial*, Material*, ResourceManager& rm);
        
        

    public:

        std::vector<BoneInfo> boneVector{};
        int m_BoneCounter{ 0 };

        auto& getMeshes() { return m_meshes; };
        auto& getboneInfoMap() { return boneVector; };
        int& getBoneCount() { return m_BoneCounter; };
        void ReadMissingBones(const aiAnimation* animation);

        Color color = D_WHITE;
        bool drawModel{ true }, drawWires{ false };

        void load(const char* filePath, ResourceManager& rm);
        void unload() override;
        void draw(glm::mat4) override;

        bool isLoaded() { return m_loaded; };
        void setColor(Color c) { color = c; };
    };
}