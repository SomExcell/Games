#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    Shader(const std::string &vertexShader, const std::string &fragmentShader);
    void use();

    Shader() = delete;
    Shader(const Shader&) = delete;
    Shader& operator=(Shader&) = delete;
    Shader& operator=(Shader&& rhs) noexcept;
    Shader(Shader&& rhs) noexcept;

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

private:
    bool createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID);

    bool isCompiled = false;
    GLuint ID = 0;
};

Shader::Shader(const std::string &vertexShader, const std::string &fragmentShader)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vShaderFile.open(vertexShader);
        fShaderFile.open(fragmentShader);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cerr << "Caught an ios_base::failure.\n"
                      << "Error code: " << e.code().value()
                      << " (" << e.code().message() << ")\n"
                      << "Error category: " << e.code().category().name() << "\n";
            std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
            return;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char * fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    GLuint vertex, fragment;
    // vertex shader
    if(!createShader(vShaderCode,GL_VERTEX_SHADER,vertex))
    {
        std::cerr << "VERTEX SHADER compile-time error" << std::endl;
    }
    // fragment Shader
    if(!createShader(fShaderCode,GL_FRAGMENT_SHADER,fragment))
    {
        std::cerr << "FRAGMENT SHADER compile-time error" << std::endl;
    }
    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader& Shader::operator=(Shader&& rhs) noexcept
{
    glDeleteProgram(ID);
    ID = rhs.ID;
    isCompiled = rhs.isCompiled;

    rhs.ID = 0;
    rhs.isCompiled = false;
    return *this;
}

Shader::Shader(Shader&& rhs) noexcept
{
    ID = rhs.ID;
    isCompiled = rhs.isCompiled;

    rhs.ID = 0;
    rhs.isCompiled = false;
}

bool Shader::createShader(const std::string& source, const GLenum shaderType,GLuint& shaderID)
{
    shaderID = glCreateShader(shaderType);
    const char* code = source.c_str();
    glShaderSource(shaderID, 1, &code, nullptr);
    glCompileShader(shaderID);

    GLint success;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[1024];
        glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);
        std::cerr << "ERROR::SHADER: Compile-time error:\n" << infoLog << std::endl;
        return false;
    }
    return true;
}

void Shader::use()
{
    glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const
{         
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
}

void Shader::setInt(const std::string &name, int value) const
{ 
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
}

void Shader::setFloat(const std::string &name, float value) const
{ 
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
}