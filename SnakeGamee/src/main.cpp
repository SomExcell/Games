#include <shader.hpp>
#include <gameObject.hpp>
#include <snake.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>
#include <chrono>
#include <thread>
#include <ctime>

//
int g_windowSizeX = 800;
int g_windowSizeY = 600;

int codeKey;

void glfwWindowSizeCallback(GLFWwindow* window, int width, int height)
{
    g_windowSizeX = width;
    g_windowSizeY = height;
    glViewport(0, 0, g_windowSizeX, g_windowSizeY);
}

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window,GL_TRUE);
    }
    codeKey = key;
}

void drawWall(Shader& shader,GameObject &wall)
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

void drawGrass(Shader& shader,GameObject &grass)
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

void drawApple(Shader& shader, GameObject &apple)
{
    shader.setFloat("changeX",apple.getX());
    shader.setFloat("changeY",apple.getY());
    apple.draw();
}

void drawSnake(Shader& shader, Snake &snake)
{
    if(codeKey >= 262 && codeKey <= 265 ){snake.setWay(codeKey);}
    if(snake.getWay() == RIGHT){snake.setCoordinates(snake.getX()+0.1,snake.getY());}
    else if(snake.getWay() == LEFT){snake.setCoordinates(snake.getX()-0.1,snake.getY());}
    else if(snake.getWay() == DOWN){snake.setCoordinates(snake.getX(),snake.getY()+0.1);}
    else if(snake.getWay() == UP){snake.setCoordinates(snake.getX(),snake.getY()-0.1);}
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    snake.draw(shader);
}

void checkApple(GameObject &apple, Snake &snake)
{
    if(int(apple.getX()*10) == int(snake.getX()*10) && int(apple.getY()*10) == int(snake.getY()*10))
    {   
        snake.takedApple();
        std::srand(std::time(nullptr));
        float randomX = static_cast<float>((1 + std::rand())%18+1)/10;
        float randomY = static_cast<float>((1 + std::rand())%18+1)/10;
        std::cout << randomX << '\n' << randomY << '\n';
        apple.setCoordinates(randomX,randomY);
    }
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    /* Initialize the library */
    if (!glfwInit())
    {
        return -1;
    }
    
    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(g_windowSizeX, g_windowSizeY, "SnakeGame", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowSizeCallback(window, glfwWindowSizeCallback);
    glfwSetKeyCallback(window, glfwKeyCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    //
    std::cout << "Render: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version:" << glGetString(GL_VERSION)  << std::endl;

    Shader shader("resources/shaders/vertex.txt","resources/shaders/fragment.txt");

    GameObject wall("resources/images/wall.png");
    GameObject grass("resources/images/grasss.jpg");
    GameObject apple("resources/images/apple.png");
    apple.setCoordinates(0.5,0.5);
    Snake snake("resources/images/snake.png");

    shader.use();
    while (!glfwWindowShouldClose(window))
    {
        // render
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        drawWall(shader,wall);
        drawGrass(shader,grass);
        drawApple(shader,apple);
        drawSnake(shader,snake);
        checkApple(apple,snake);
        // render container
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
//
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}
