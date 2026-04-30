#pragma once

#include "character.h"
#include "ai_npc_game/utils/rng.h"
#include "ai_npc_game/animation.h"

#include "utils/glm_utils.h"

#include "core/gpu/mesh.h"
#include "core/gpu/shader.h"
#include "core/engine.h"

namespace ai_npc
{
    class NPC : public Character
    {
        // class fields
    public:
    private:
    protected:
        bool movingToPosition;
        glm::vec3 selectedPosition;
        FloatMod degreesLeftToRotate;
        // class methods
    public:
        NPC();
        // Constructor with random spawn position
        NPC(Mesh* mesh, Shader* shader);
        void moveToPosition(glm::vec3 position);
        bool isMovingToPosition() const;
        void render(Camera* camera, float deltaTimeSeconds);
    private:
        void updatePosition(float deltaTimeSeconds);
    protected:
    };
}
