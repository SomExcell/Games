#pragma once

#include <gameObject.hpp>
#include <shader.hpp>
#include <snake.hpp>

#include <glad/glad.h>

#include <iostream>
#include <thread>
#include <chrono>

class Game
{
public:
    Game();
    void setKey(int codeKey);
    void drawWall();
    void drawGrass();
    void drawApple();
    void drawSnake();
    void checkApple();
    void drawObjects();
    void checkCollision();
    void update();
private:
    Shader shader{"resources/shaders/vertex.txt","resources/shaders/fragment.txt"};
    GameObject wall{"resources/images/wall.png"};
    GameObject grass{"resources/images/grasss.jpg"};
    GameObject apple{"resources/images/apple.png"};
    GameObject gameOver{"resources/images/gameOver.png"};
    Snake snake{"resources/images/snake.png"};
    int codeKey;
    bool inGame = true;
};

Game::Game()
{
    shader.use();
    apple.setCoordinates(0.5,0.5);
}

void Game::setKey(int codeKey)
{
    this->codeKey = codeKey;
}

void Game::drawWall()
{
    for (GLfloat x = 0; x <= 2; x+=0.1)
    {
        for (GLfloat y = 0; y <= 2; y+=0.1)
        {
            if(x == 0 || y == 0 || x >= 1.9 || y >= 1.9)
            {
                shader.setFloat("changeX",x);
                shader.setFloat("changeY",y);
                wall.draw();
            }        
        }
    }
    shader.setFloat("changeX",0);
    shader.setFloat("changeY",0);
}

void Game::drawGrass()
{
    for (GLfloat x = 0.1; x <= 1.9; x+=0.1)
    {
        for (GLfloat y = 0.1; y <= 1.9; y+=0.1)
        {
            shader.setFloat("changeX",x);
            shader.setFloat("changeY",y);
            grass.draw();     
        }
    }
    shader.setFloat("changeX",0);
    shader.setFloat("changeY",0);
}

void Game::drawApple()
{
    shader.setFloat("changeX",apple.getX());
    shader.setFloat("changeY",apple.getY());
    apple.draw();
}

void Game::drawSnake()
{
    if(codeKey >= 262 && codeKey <= 265 ){snake.setWay(codeKey);}
    if(snake.getWay() == RIGHT){snake.setCoordinates(snake.getX()+0.1,snake.getY());}
    else if(snake.getWay() == LEFT){snake.setCoordinates(snake.getX()-0.1,snake.getY());}
    else if(snake.getWay() == DOWN){snake.setCoordinates(snake.getX(),snake.getY()+0.1);}
    else if(snake.getWay() == UP){snake.setCoordinates(snake.getX(),snake.getY()-0.1);}
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    snake.draw(shader);
}

void Game::checkApple()
{
    if(int(apple.getX()*10) == int(snake.getX()*10) && int(apple.getY()*10) == int(snake.getY()*10))
    {   
        snake.takedApple();
        std::srand(std::time(nullptr));
        float randomX = static_cast<float>((1 + std::rand())%18+1)/10;
        float randomY = static_cast<float>((1 + std::rand())%18+1)/10;
        apple.setCoordinates(randomX,randomY);
    }
}

void Game::drawObjects()
{
    drawWall();
    drawGrass();
    drawApple();
    drawSnake();
}

void Game::checkCollision()
{
    if(snake.getX()<=0.05 || snake.getX() >= 1.9 || snake.getY() <= 0.05 || snake.getY() >= 1.9)
    {
        inGame = false;
        gameOver.setCoordinates(0.5,0.5);
        gameOver.setSizeSides(1,1);
    }
}

void Game::update()
{
    checkCollision();
    if(inGame)
    {
        drawObjects();
        checkApple();
    }else
    {
        gameOver.draw();
    }
}