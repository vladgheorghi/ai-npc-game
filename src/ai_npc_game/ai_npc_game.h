#pragma once

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

        // Class variables
        float facingAngle;

		std::unordered_map<std::string, NPC*> npcs;
        unsigned int maxNPCs;

        Shader *skinningShader;

        Character *player;

        Camera* camera;

        // HUD and chat logic
        gfxc::ImGuiLayer *imguiLayer;
        Chat* chat;
    };
}   // namespace ai_npc
