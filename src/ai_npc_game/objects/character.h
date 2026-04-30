#pragma once

#include "object.h"

#include "utils/glm_utils.h"

#include "core/gpu/mesh.h"
#include "core/gpu/shader.h"

namespace ai_npc
{
    class Character : public Object
    {
        // class fields
    public:
    private:
    protected:
        std::string name;
        float health;
        float damage;
        float movementSpeed;

        // class methods
    public:
        Character();
        Character(glm::vec3 position);
        Character(glm::vec3 position, Mesh* mesh, Shader* shader);
        ~Character();

        void moveForward(float distance) override;
        void moveRight(float distance) override;

		const char* getName() const;
        float getHealth() const;
        float getDamage() const;
        float getMovementSpeed() const;

		void setHealth(float health);
		void setDamage(float damage);
        void setMovementSpeed(float movementSpeed);
    private:
    protected:
    };
}
