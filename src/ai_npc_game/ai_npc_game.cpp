#include "ai_npc_game.h"


namespace ai_npc {

    Game::Game()
    {
        facingAngle = 0.0f;

        skinningShader = new Shader("Skinning");
        player = new Character(glm::vec3(0, 0, 0));
        camera = new Camera();
        maxNPCs = 10;

        imguiLayer = new gfxc::ImGuiLayer();
        chat = new Chat();
    }


    Game::~Game()
    {
        delete imguiLayer;
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
            characterMesh->anim[0]->mTicksPerSecond = 1000;
        }

        {
			player->setMesh(meshes["player_character"]);
			player->setShader(shaders["Skinning"]);
            // Visual mesh correction
            player->setMeshRotationCorrection(Vec3Mod(FloatMod(0), FloatMod(0), FloatMod(180)));
            // Render first keyframe
            float runningTime = (float)((double)Engine::GetElapsedTime());
            Animation::BoneTransform(meshes["player_character"], runningTime);
        }

        {
            camera->Set(player->getPosition() + glm::vec3(0, 1, camera->distanceToTarget), player->getPosition() + glm::vec3(0, 1, 0));
			camera->SetProjectionMatrix(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
        }

        {
            imguiLayer->Init(window);
        }
    }


    void Game::FrameStart()
    {
        imguiLayer->BeginFrame();
    }


    void Game::Update(float deltaTimeSeconds)
    {
        ClearScreen();
        
        glm::vec3 eyeHeight = player->getPosition() + glm::vec3(0, 1, 0);
        camera->FollowTarget(eyeHeight);
        player->render(camera);

        if (randInt(0, 100) < 2 && npcs.size() < maxNPCs) {
            std::string npcID = "npc" + std::to_string(npcs.size());
            Mesh* npcMesh = new Mesh(npcID);
            npcMesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS_AI_NPC_GAME), "scene.fbx");
            meshes[npcMesh->GetMeshID()] = npcMesh;
            npcMesh->anim[0]->mTicksPerSecond = 1000;

            NPC* newNPC = new NPC(npcMesh, shaders["Skinning"]);
            newNPC->setMeshRotationCorrection(Vec3Mod(FloatMod(0), FloatMod(0), FloatMod(180)));
            newNPC->uniformScale(0.01f);

            float runningTime = (float)((double)Engine::GetElapsedTime());
            Animation::BoneTransform(npcMesh, runningTime);
            npcs[npcID] = newNPC;
		}

        // HUD — always visible, top-left corner
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.35f);
        ImGui::Begin("##hud", nullptr,
            ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
            ImGuiWindowFlags_NoNav);
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::Text("NPCs: %zu / %u", npcs.size(), maxNPCs);
        ImGui::Text("[Enter] Talk to NPC");
        ImGui::End();

        // Chat panel - toggled with Enter
        chat->ShowChat();

        if (randInt(0, 1000) < 10 && npcs.size() > 0) {
            std::string selectedNPCID = "npc" + std::to_string(randInt(0, (int)npcs.size() - 1));
            if (npcs.count(selectedNPCID) > 0 && !npcs[selectedNPCID]->isMovingToPosition()) {
                glm::vec3 selectedPosition = glm::vec3(randFloat(-5.0f, 5.0f), 0, randFloat(-5.0f, 5.0f));
                npcs[selectedNPCID]->moveToPosition(selectedPosition);
            }
        }

        for (auto& [npcID, npc] : npcs) {
            npc->render(camera, deltaTimeSeconds);
		}
    }

    void Game::FrameEnd()
    {
        DrawCoordinateSystem(camera->GetViewMatrix(), camera->GetProjectionMatrix());
        imguiLayer->EndFrame();
    }

    /*
    *  These are callback functions. To find more about callbacks and
    *  how they behave, see `input_controller.h`.
    */
    void Game::OnInputUpdate(float deltaTime, int mods)
    {
        // Stop WASD movement when typing
        if (imguiLayer->WantCaptureKeyboard()) {
            return;
        }

		// Determine input direction based on WASD keys (e.g. W + A = forward-left, direction will be at 45 degrees)
        glm::vec2 inputDirection(0.0f);
        if (window->KeyHold(GLFW_KEY_W)) {
            inputDirection.y += 1.0f;
        }
        if (window->KeyHold(GLFW_KEY_A)) {
            inputDirection.x += 1.0f;
        }
        if (window->KeyHold(GLFW_KEY_S)) {
            inputDirection.y -= 1.0f;
        }
        if (window->KeyHold(GLFW_KEY_D)) {
            inputDirection.x -= 1.0f;
        }
        
		// If there's input
        if (glm::length(inputDirection) > 0.0f) {
			// Calculate the angle to rotate toward, relative to camera's forward direction
            float angle = DEGREES(atan2f(inputDirection.x, inputDirection.y) - camera->GetRotationOY());

            // Shortest angular delta, clamped to [-180, 180]
            // (i.e. calculate how much rotation is left until reaching `angle` based on current steps added to `facingAngle`)
            FloatMod deltaAngle = angle - facingAngle;

            // Rotate toward target at a fixed speed (degrees/sec)
            float rotationSpeed = 600.f;
			// Clamp the rotation step to either subtract the max rotation speed or add it, depending on the sign of `deltaAngle`
            float step = glm::clamp((float)deltaAngle, -rotationSpeed * deltaTime, rotationSpeed * deltaTime);
            facingAngle += step;

			// Allow sprinting by holding the left shift key, which increases movement speed by 2 units/sec
			if (window->KeyHold(GLFW_KEY_LEFT_SHIFT)) {
                player->setMovementSpeed(4.5f);
            } else {
                player->setMovementSpeed(2.5f);
            }

			// Move forward in the direction the player is facing
            player->rotateOY(facingAngle);
            player->moveForward(deltaTime);

			// Update animation based on elapsed time
            float runningTime = (float)((double)Engine::GetElapsedTime());
            Animation::BoneTransform(meshes["player_character"], runningTime);
        }
    }


    void Game::OnKeyPress(int key, int mods)
    {
        // Add key press event
        
        // Stop key events going to game logic while ImGui has focus
        if (imguiLayer->WantCaptureKeyboard())
            return;

        // Toggle chat on Enter press
        if (key == GLFW_KEY_ENTER) {
            chat->show = !chat->show;
        }
    }


    void Game::OnKeyRelease(int key, int mods)
    {
        // Add key release event
    }


    void Game::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
    {
        // Add mouse move event
        if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
        {
            camera->RotateThirdPerson_OX(-deltaY);
            camera->RotateThirdPerson_OY(deltaX);
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
        camera->Zoom(-offsetY);
    }


    void Game::OnWindowResize(int width, int height)
    {
        // Treat window resize event
    }
}
