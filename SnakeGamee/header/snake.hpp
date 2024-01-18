#pragma once

#include <gameObject.hpp>
#include <shader.hpp>

#include <glad/glad.h>

//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

#include <iostream>
#include <vector>
#include <utility>

enum Way
{
    RIGHT = 262,
    LEFT = 263,
    DOWN = 264,
    UP = 265
};

class Snake:public GameObject
{
public:
    Snake(const std::string &texturePath);
    void setWay(const GLuint &codeKey);
    void setCoordinates(GLfloat xCoord,GLfloat yCoord);
    void draw(Shader& shader);
    void takedApple();
    Way getWay();
private:
    Way way = RIGHT;
    GLuint size = 3;
    std::vector<std::pair<GLfloat,GLfloat>> body={{0.0,0.0},{0.0,0.0},{0.0,0.0}};
};

Snake::Snake(const std::string &texturePath):GameObject(texturePath)
{
    GameObject::setCoordinates(0.4,0.3);
    body[0].first = 0.4,body[0].second = 0.3;
    body[1].first = 0.3,body[1].second = 0.3;
    body[2].first = 0.2,body[1].second = 0.3;
}

void Snake::setWay(const GLuint &codeKey)
{
    if(codeKey == 262 && way != 263){way = RIGHT;}
    else if(codeKey == 263 && way != 262){way = LEFT;}
    else if(codeKey == 264 && way != 265){way = DOWN;}
    else if(codeKey == 265 && way != 264){way = UP;}
}

void Snake::setCoordinates(GLfloat xCoord,GLfloat yCoord)
{
    for (size_t i = size-1; i > 0; --i)
    {
        body[i] = body[i-1];
    }
    body[0].first = xCoord,body[0].second = yCoord;
    GameObject::setCoordinates(xCoord,yCoord);
}

void Snake::draw(Shader& shader)
{
    shader.setFloat("changeX",body[0].first);
    shader.setFloat("changeY",body[0].second);
    GameObject::draw();
    loadTexture("resources/images/body.png");
    for (size_t i = 1; i < size; i++)
    {
        shader.setFloat("changeX",body[i].first);
        shader.setFloat("changeY",body[i].second);
        GameObject::draw();
    }
    loadTexture("resources/images/snake.png");
    shader.setFloat("changeX",0);
    shader.setFloat("changeY",0);
}

void Snake::takedApple()
{
    ++size;
    body.push_back(std::make_pair(0.0,0.0));
}

Way Snake::getWay()
{
    return way;
}
