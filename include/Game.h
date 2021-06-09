#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

class Game {
public:
    GameState State;
    GLuint Width, Height;

    Game(GLuint width, GLuint height);
    ~Game();

    void Init();
    void ProcessInput(GLFWwindow *window, GLfloat dt);
    void Update(GLfloat dt);
    void Render();
};