#include "resource_mesh.hpp"

bool Mesh::load(){ 
    //setupMesh();
    std::cout << "Load a mesh" << std::endl;
    return true; 
}

void Mesh::unload(){ 
    /*
    glDeleteBuffers(1, m_VBO.get());
    glDeleteBuffers(1, m_EBO.get());
    glDeleteVertexArrays(1, m_VAO.get());
    std::cout << "Unload a mesh" << std::endl; 
    */
}

bool Mesh::isLoaded() const{ 
    return m_VAO != nullptr && m_VBO != nullptr && m_EBO != nullptr;
}

void Mesh::setupMesh(){
    // Generate vertex array and buffers
    glGenVertexArrays(1, m_VAO.get());
    glGenBuffers(1, m_VBO.get());
    glGenBuffers(1, m_EBO.get());
    // glGenVertexArrays(1, &m_VAO);
    // glGenBuffers(1, &m_VBO);
    // glGenBuffers(1, &m_EBO);

    // Bind vertex array
    glBindVertexArray(*m_VAO);
    //glBindVertexArray(m_VAO);

    // Bind buffer and fill with vertex data
    glBindBuffer(GL_ARRAY_BUFFER, *m_VBO);
    //glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(Vertex), &index[0], GL_STATIC_DRAW);

    // Bind buffer and fill with index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *m_EBO);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(u_int16_t), &index[0], GL_STATIC_DRAW);

    // Enable and specify vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // Enable and specify vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // Enable and specify vertex coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textCoords));

    // Unbind vertex array
    glBindVertexArray(0);
}

void Mesh::draw(){ 
    std::cout << "Draw a mesh" << std::endl; 
    /* OpenGL */ 
}