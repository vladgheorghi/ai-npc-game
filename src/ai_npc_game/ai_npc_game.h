#pragma once

#include <memory>

#include "imgui.h"

#include "components/simple_scene.h"
#include "components/imgui_layer.h"

#include "animation.h"
#include "ai_npc_game/objects/character.h"
#include "ai_npc_game/objects/npc.h"
#include "ai_npc_game/camera/camera.h"
#include "ai_npc_game/hud/chat.h"


namespace ai_npc
{

    class Character;

    class Game : public gfxc::SimpleScene
    {
    public:
        Game();
        ~Game();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void spawnNPC();
        void moveRandomNPC();

        // Class variables
        float facingAngle;

		std::unordered_map<std::string, std::unique_ptr<NPC>> npcs;
        unsigned int maxNPCs;
        unsigned int nextNpcId;

        Shader *skinningShader;
        // Using unique_ptr for automatic garbage collection
        std::unique_ptr<Character> player;
        std::unique_ptr<Camera> camera;
        
        // HUD and chat logic
        std::unique_ptr<gfxc::ImGuiLayer> imguiLayer;
        std::unique_ptr<Chat> chat;

        // Timers
        float spawnTimer;        // seconds until next spawn attempt
        float moveTimer;       // seconds until next wander roll
    };
}   // namespace ai_npc
