#pragma once

#include "core/gpu/shader.h"
#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"
#include "utils/math_utils.h"

#include "object.h"

namespace ai_npc
{
    class Character : public Object
    {
        // class fields
    public:
    private:
    protected:
        float movementSpeed;

        // class methods
    public:
        Character();
        Character(glm::vec3 position);
        Character(glm::vec3 position, Mesh* mesh, Shader* shader);
        ~Character();

        float getMovementSpeed();

        void setMovementSpeed(float movementSpeed);
    private:
    protected:
    };
}
