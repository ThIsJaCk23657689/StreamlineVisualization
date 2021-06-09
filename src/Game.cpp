#include "Game.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "Streamline.h"
#include "PostProcessor.h"

SpriteRenderer *Renderer;
Streamline *Stream;
PostProcessor *Effects;

Game::Game(GLuint width, GLuint height)
    : State(GAME_ACTIVE), Width(width), Height(height) {}

Game::~Game() {
    delete Renderer;
    delete Effects;
}

void Game::Init() {
    ResourceManager::LoadShader("assets/shaders/sprite.vert", "assets/shaders/sprite.frag", nullptr, "sprite");
    ResourceManager::LoadShader("assets/shaders/post_processing.vert", "assets/shaders/post_processing.frag", nullptr, "post_processing");

    Stream = new Streamline(32);
    Stream->Load("assets/data/1.vec");

    glm::mat4 projection = glm::ortho(0.0f, Stream->Resolution.x, 0.0f, Stream->Resolution.y, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInt("sprite", 0);
    ResourceManager::GetShader("sprite").SetMat4("projection", projection);

    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    Effects = new PostProcessor(ResourceManager::GetShader("post_processing"), this->Width, this->Height);

    // Effects->Shake = GL_TRUE;
    // Effects->Chaos = GL_TRUE;
    // Effects->Confuse = GL_TRUE;
}

void Game::Update(GLfloat dt) {

}

void Game::ProcessInput(GLFWwindow *window, GLfloat dt) {

}

void Game::Render() {
    if (this->State == GAME_ACTIVE) {
        Effects->BeginRender();
        {
            Stream->Draw(Renderer);
        }
        Effects->EndRender();
        Effects->Render(glfwGetTime());
    }
}