#include "ai_npc_game.h"

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
        character = new Character(glm::vec3(0, 0, 0));
        camera = new Camera();
    }


    Game::~Game()
    {
    }


    void Game::Init()
    {
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
			character->setMesh(meshes["player_character"]);
			character->setShader(shaders["Skinning"]);
            character->rotateOZ(180);
        }

        {
            camera->Set(character->getPosition() + glm::vec3(0, 1, camera->distanceToTarget), character->getPosition() + glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
			camera->SetProjectionMatrix(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
        }
    }


    void Game::FrameStart()
    {
    }


    void Game::Update(float deltaTimeSeconds)
    {
        ClearScreen();
        
        character->render(camera);

        float runningTime = (float)((double)Engine::GetElapsedTime());
        Animation::BoneTransform(meshes["player_character"], runningTime);

    }

    void Game::FrameEnd()
    {
        DrawCoordinateSystem(camera->GetViewMatrix(), camera->GetProjectionMatrix());
    }

    /*
    *  These are callback functions. To find more about callbacks and
    *  how they behave, see `input_controller.h`.
    */
    void Game::OnInputUpdate(float deltaTime, int mods)
    {
        // Treat continuous update based on input
        if (window->KeyHold(GLFW_KEY_W)) {
            character->moveForward(deltaTime);
            camera->MoveForward(deltaTime * character->getMovementSpeed(), character->getForward());
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            character->moveRight(-deltaTime);
            camera->MoveForward(-deltaTime * character->getMovementSpeed(), character->getRight());
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            character->moveForward(-deltaTime);
			camera->MoveForward(-deltaTime * character->getMovementSpeed(), character->getForward());
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            character->moveRight(deltaTime);
			camera->MoveForward(deltaTime * character->getMovementSpeed(), character->getRight());
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
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;
        // Add mouse move event
        if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
        {
            camera->RotateThirdPerson_OX(-deltaY * sensivityOY);
            camera->RotateThirdPerson_OY(-deltaX * sensivityOX);
        }
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
