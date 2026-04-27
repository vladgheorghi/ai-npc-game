#include "ai_npc_game.h"
#include "animation.h"
#include "objects/character.h"

#include <vector>
#include <iostream>

namespace ai_npc {

    Game::Game()
    {
        // TODO(student): Never forget to initialize class variables!
        clear_color = glm::vec3(0);
        mesh_choices = { "box.obj", "teapot.obj", "sphere.obj" };
        mesh_index = 0;
        mesh_pos = glm::vec3(3, 0, 3);

        skinningShader = new Shader("Skinning");
    }


    Game::~Game()
    {
    }


    void Game::Init()
    {
        auto camera = GetSceneCamera();
        camera->SetPositionAndRotation(glm::vec3(0, 3.5, 4), glm::quat(glm::vec3(-30 * TO_RADIANS, 0, 0)));
        camera->Update();

        // Create a shader program for rendering to texture
        {
            skinningShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::AI_NPC, "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
            skinningShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::AI_NPC, "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
            skinningShader->CreateAndLink();
            shaders[skinningShader->GetName()] = skinningShader;
        }

        {
            Mesh* characterMesh = new Mesh("player_character");
            characterMesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS_AI_NPC_GAME), "scene.fbx");
            meshes[characterMesh->GetMeshID()] = characterMesh;
        }

        {
            character = new Character(glm::vec3(0, 0, 0), meshes["player_character"], skinningShader);
        }
    }


    void Game::FrameStart()
    {
    }


    void Game::Update(float deltaTimeSeconds)
    {
        ClearScreen();
        
        character->render();

        float runningTime = (float)((double)Engine::GetElapsedTime());
        Animation::BoneTransform(meshes["player_character"], runningTime);

    }

    void Game::FrameEnd()
    {
        DrawCoordinateSystem();
    }

    /*
    *  These are callback functions. To find more about callbacks and
    *  how they behave, see `input_controller.h`.
    */
    void Game::OnInputUpdate(float deltaTime, int mods)
    {
        // Treat continuous update based on input

        // TODO(student): Add some key hold events that will let you move
        // a mesh instance on all three axes. You will also need to
        // generalize the position used by `RenderMesh`.
        float speed = 2.5f;

        if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
            if (window->KeyHold(GLFW_KEY_W)) {
                mesh_pos.z -= deltaTime * speed;
            }

            if (window->KeyHold(GLFW_KEY_A)) {
                mesh_pos.x -= deltaTime * speed;
            }

            if (window->KeyHold(GLFW_KEY_S)) {
                mesh_pos.z += deltaTime * speed;
            }

            if (window->KeyHold(GLFW_KEY_D)) {
                mesh_pos.x += deltaTime * speed;
            }

            if (window->KeyHold(GLFW_KEY_E)) {
                mesh_pos.y += deltaTime * speed;
            }

            if (window->KeyHold(GLFW_KEY_Q)) {
                mesh_pos.y -= deltaTime * speed;
            }
        }
    }


    void Game::OnKeyPress(int key, int mods)
    {
        // Add key press event
    }


    void Game::OnKeyRelease(int key, int mods)
    {
        // Add key release event
    }


    void Game::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
    {
        // Add mouse move event
    }


    void Game::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
    {
        // Add mouse button press event
    }


    void Game::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
    {
        // Add mouse button release event
    }


    void Game::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
    {
        // Treat mouse scroll event
    }


    void Game::OnWindowResize(int width, int height)
    {
        // Treat window resize event
    }
}
