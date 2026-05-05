#include "ai_npc_game.h"

namespace ai_npc {
    void Game::spawnNPC(float deltaTimeSeconds)
    {
        spawnTimer -= deltaTimeSeconds;
        if (spawnTimer <= 0.0f && npcs.size() < maxNPCs) {
            std::string npcID = "npc" + std::to_string(nextNpcId++);
            // Reuse the shared mesh loaded once at Init
            auto newNPC = std::make_unique<NPC>(meshes["human"], shaders["Skinning"]);
            newNPC->setMeshRotationCorrection(Vec3Mod(FloatMod(0), FloatMod(0), FloatMod(180)));
    
            npcs.emplace(npcID, std::move(newNPC));
            spawnTimer = randFloat(2.0f, 5.0f);  // reset to random interval
        }
    }

    void Game::moveRandomNPC(float deltaTimeSeconds)
    {
        moveTimer -= deltaTimeSeconds;
        if (moveTimer <= 0.0f && !npcs.empty()) {
            std::string selectedNPCID = "npc" + std::to_string(randInt(0, (int)npcs.size() - 1));
            if (npcs.count(selectedNPCID) > 0 && !npcs[selectedNPCID]->isMovingToPosition() && !npcs[selectedNPCID]->isTalking()) {
                glm::vec3 selectedPosition = glm::vec3(randFloat(-5.0f, 5.0f), 0, randFloat(-5.0f, 5.0f));
                npcs[selectedNPCID]->moveToPosition(selectedPosition);
            }
            // next spawn happens in 2-10 seconds
            moveTimer = randFloat(2.0f, 10.0f);
        }
    }

    void Game::showChatOnPress()
    {
        // Show chat if Enter is pressed 
        if (chat->show && player->isTalking()) {
            Character* partner = player->getTalkingTo();
            chat->showChat({ player->getId(), partner->getId() }, player->getId());
        }

        // Stop talking if chat is closed
        if (!chat->show && player->isTalking()) {
            player->stopTalking();
        }
    }

    void Game::showHUD()
    {
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
    }

    void Game::renderNPCs(float deltaTimeSeconds)
    {
        for (auto& [npcID, npc] : npcs) {
            npc->render(camera.get(), deltaTimeSeconds);
		}
    }

    void Game::renderPlayer()
    {
        glm::vec3 eyeHeight = player->getPosition() + glm::vec3(0, 1, 0);
        camera->FollowTarget(eyeHeight);
        player->render(camera.get());
    }
}