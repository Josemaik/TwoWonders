#include "entity_model.hpp"

namespace DarkMoon {
    void Model::load(const char* filePath, ResourceManager& rm) {
        // Read file
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);
        if (!scene || !scene->mRootNode) {
            std::cerr << "[ERROR ASSIMP] : " << importer.GetErrorString() << std::endl;
            return;
        }

        m_loaded = true;
        m_name = filePath;

        std::cout << " - Load a model -> " << m_name << std::endl;

        processNode(scene->mRootNode, scene, rm);
    };

    void Model::unload() {
        //ResourceManager rm = ResourceManager::getInstance();
        //std::cout << "Unload -> " << this->m_name << std::endl; 
        //for(int i=0; i<static_cast<int>(m_meshes.size()); i++)
        //    rm.unloadResource(m_meshes[i]->getID());
    }

    void Model::draw(glm::mat4 transMatrix) {

        // Draw meshes
        if (drawModel)
            for (const auto& mesh : m_meshes)
                mesh->draw(transMatrix, color);

        // Draw wires
        if (drawWires)
            for (const auto& mesh : m_meshes)
                mesh->drawLines(transMatrix);

    };

    // PRIVATE

    void Model::processNode(aiNode* node, const aiScene* scene, ResourceManager& rm) {
        // Find the directory of the obj file
        std::string directory = m_name;
        size_t last_slash_idx = directory.find_last_of('/');
        if (std::string::npos != last_slash_idx)
            directory = directory.substr(0, last_slash_idx);

        // Process all the node's meshes
        for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            aiMaterial* aiMaterial = scene->mMaterials[mesh->mMaterialIndex];
            //std::cout << "Material: " << aiMaterial->GetName().C_Str() << std::endl;
            processMesh(mesh, aiMaterial, scene, rm);
        }

        // Recursively process each of the node's children
        for (unsigned int i = 0; i < node->mNumChildren; ++i) {
            processNode(node->mChildren[i], scene, rm);
        }
    }

    void Model::processMesh(aiMesh* mesh, aiMaterial* aiMaterial, const aiScene*, ResourceManager& rm) {
        // Process the vertices
        std::vector<Vertex> vertices(mesh->mNumVertices);

        for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
            Vertex vertex;

            // Position
            if (mesh->HasPositions()) {
                vertex.position.x = mesh->mVertices[i].x;
                vertex.position.y = mesh->mVertices[i].y;
                vertex.position.z = mesh->mVertices[i].z;
            }

            // Normal
            if (mesh->HasNormals()) {
                vertex.normal.x = mesh->mNormals[i].x;
                vertex.normal.y = mesh->mNormals[i].y;
                vertex.normal.z = mesh->mNormals[i].z;
            }

            // Text Coords
            if (mesh->HasTextureCoords(0)) {
                vertex.textCoords.x = mesh->mTextureCoords[0][i].x;
                vertex.textCoords.y = mesh->mTextureCoords[0][i].y;
            }
            else // Default Texture Coords
                vertex.textCoords = glm::vec2(0.0f, 0.0f);

            vertices[i] = vertex;
        }

        // Process the indices
        std::vector<u_int16_t> indices;

        for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; ++j) {
                indices.push_back(static_cast<unsigned short>(face.mIndices[j]));
            }
        }

        // Process material
        auto material = processMaterial(aiMaterial, rm);
        processTextures(aiMaterial, material, rm);

        auto currentMesh = rm.loadResource<Mesh>(mesh->mName.C_Str(), vertices, indices, material);

        m_meshes.push_back(currentMesh);
    }

    Material* Model::processMaterial(aiMaterial* aiMaterial, ResourceManager& rm) {
        aiColor4D ambient(0.0f, 0.0f, 0.0f, 1.0f);
        aiColor4D diffuse(0.0f, 0.0f, 0.0f, 1.0f);
        aiColor4D specular(0.0f, 0.0f, 0.0f, 1.0f);
        float shininess = 0.0f;

        // Retrieve material properties
        aiGetMaterialColor(aiMaterial, AI_MATKEY_COLOR_AMBIENT, &ambient);
        aiGetMaterialColor(aiMaterial, AI_MATKEY_COLOR_DIFFUSE, &diffuse);
        aiGetMaterialColor(aiMaterial, AI_MATKEY_COLOR_SPECULAR, &specular);
        aiGetMaterialFloat(aiMaterial, AI_MATKEY_SHININESS, &shininess);

        // Create and return Material object
        auto material = rm.loadResource<Material>(aiMaterial->GetName().C_Str());
        //auto material = rm.loadResource<Material>(aiMaterial->GetName(),
        //                                          glm::vec3(ambient.r, ambient.g, ambient.b),
        //                                          glm::vec3(diffuse.r, diffuse.g, diffuse.b),
        //                                          glm::vec3(specular.r, specular.g, specular.b),
        //                                          shininess);

        return material;
    }

    void Model::processTextures(aiMaterial* aiMaterial, Material* material, ResourceManager& rm) {
        aiString aiTexturePath;
        aiReturn texFound = aiMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &aiTexturePath);
        if (texFound == AI_SUCCESS) {
            std::string texturePath = aiTexturePath.C_Str();
            std::string modelDirectory = m_name;
            size_t lastSlashIndex = modelDirectory.find_last_of('/');
            if (lastSlashIndex != std::string::npos) {
                modelDirectory = modelDirectory.substr(0, lastSlashIndex);
            }
            texturePath = modelDirectory + "/" + texturePath;

            auto texture = rm.loadResource<Texture>(texturePath.c_str());
            //texture->load(texturePath.c_str());

            material->texture = texture;
        }
        else
            material->texture = nullptr;
    }
}