#pragma once

#include "components/simple_scene.h"

#include "objects/character.h"


namespace ai_npc
{
    class Game : public gfxc::SimpleScene
    {
     public:
        Game();
        ~Game();

        void Init() override;

     private:
         void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);

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
        glm::vec3 clear_color;
        std::vector<std::string> mesh_choices;
        size_t mesh_index;
        glm::vec3 mesh_pos;

        Shader *skinningShader;

        Character *character;
    };
}   // namespace ai_npc
