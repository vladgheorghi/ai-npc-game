#pragma once

#include "object.h"
#include "npc.h"
#include "ai_npc_game/hud/chat.h"

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
        float interactRadius;

        Character* talkingTo;

    // class methods
    public:
        Character();
        Character(glm::vec3 position);
        Character(glm::vec3 position, Mesh* mesh, Shader* shader);
        ~Character();

        void moveForward(float distance) override;
        void moveRight(float distance) override;
        bool isNearby(Object* object);
        virtual void talkTo(NPC* npc, Chat* chat);
        bool isTalking();
        void stopTalking();

		const char* getName() const;
        float getHealth() const;
        float getDamage() const;
        float getMovementSpeed() const;
        float getInteractRadius() const;

		void setHealth(float health);
		void setDamage(float damage);
        void setMovementSpeed(float movementSpeed);
    private:
    protected:
    };
}
