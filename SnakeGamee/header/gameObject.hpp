#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glad/glad.h>

#include <iostream>


GLfloat verticesDefault[32] = 
{
    // positions          // colors           // texture coords
    -1.0f,  1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 1.0f, // top left
    -0.9f,  1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
    -1.0f,  0.9f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, // bottom left
    -0.9f,  0.9f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 0.0f  // bottom right
};

GLuint indicesDefault[6] = 
{  
    0, 1, 2, // first triangle
    1, 2, 3  // second triangle
};

class GameObject
{
public:
    GameObject(const std::string &texturePath);
    ~GameObject(){};
    void loadTexture(const std::string &texturePath);
    void setupVertex();
    void setCoordinates(GLfloat xCoord,GLfloat yCoord);
    void setSizeSides(GLfloat sizeSideX, GLfloat sizeSideY);
    void installVertices();
    void draw();
    GLuint getTexture();
    GLuint getVAO();
    GLfloat getX();
    GLfloat getY();
private:
    GLuint texture = 0;
    GLuint VBO = 0, VAO = 0, EBO = 0;
    GLfloat x = 0,y = 0,sizeX,sizeY;
    GLfloat vertices[32];
    GLuint indices[6];
};

GameObject::GameObject(const std::string &texturePath)
{
    installVertices();
    loadTexture(texturePath);
    setupVertex();
}

void GameObject::loadTexture(const std::string &texturePath)
{
    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D,texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    GLint width,height,nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(texturePath.c_str(),&width,&height,&nrChannels,0);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else
    {
        std::cerr << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

void GameObject::setupVertex()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void GameObject::setCoordinates(GLfloat xCoord,GLfloat yCoord)
{
    x = xCoord,y = yCoord;
    //vertices[0] = -1.0f + xCoord, vertices[1] = 1.0f - yCoord;
}

void GameObject::setSizeSides(GLfloat sizeSideX, GLfloat sizeSideY)
{
    sizeX = sizeSideX,sizeY = sizeSideY;
    vertices[8] = vertices[0] + sizeSideX, vertices[17] = vertices[1] - sizeSideY;
    vertices[24] = vertices[0] + sizeSideX, vertices[25] = vertices[1] - sizeSideY;
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void GameObject::installVertices()
{
    std::copy(std::begin(verticesDefault),std::end(verticesDefault),std::begin(vertices));
    std::copy(std::begin(indicesDefault),std::end(indicesDefault),std::begin(indices));
}

void GameObject::draw()
{
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

GLuint GameObject::getTexture()
{
    return texture;
}

GLuint GameObject::getVAO()
{
    return VAO;
}

GLfloat GameObject::getX()
{
    return x;
}

GLfloat GameObject::getY()
{
    return y;
}
