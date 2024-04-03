#include "resource_shader.hpp"

Shader::Shader(std::size_t id, const char* vertexPath, const char* fragmentPath, ShaderType type){
    this->m_id = id;
    this->type = type;

    //----- Retrieve the vertex/fragment source code from filePath -----//
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    try {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();		
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();		
    }
    catch(const std::ifstream::failure& e){
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n"
              << "Error code: " << e.code() << "\n"
              << "Exception: " << e.what() << std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    //----- Compile shaders -----//
    GLuint vertex, fragment;
    
    // Vertex Shader
    //--------------
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // Fragment Shader
    //----------------
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // check for shader compile errors
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Link Shaders
    // ------------
    id_shader = glCreateProgram();
    glAttachShader(id_shader, vertex);
    glAttachShader(id_shader, fragment);
    glLinkProgram(id_shader);
    // check for linking errors
    glGetProgramiv(id_shader, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(id_shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    std::cout << "Load a shader (ID: " << m_id <<") -> " << vertexPath << " - " << fragmentPath << std::endl; 
}

void Shader::use(){
    glUseProgram(id_shader);
}

void Shader::unload(){
    std::cout << "Unload a shader (ID: " << m_id <<")" << std::endl; 
    glDeleteProgram(id_shader);
}