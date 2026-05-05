#pragma once

#include "object.h"

#include "utils/glm_utils.h"

#include "core/gpu/mesh.h"
#include "core/gpu/shader.h"

namespace ai_npc
{
    class NPC;

    class Character : public Object
    {
    // class fields
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
        float getDistanceTo(Object* object);
        void talkTo(NPC* npc);
        bool isTalking();
        void stopTalking();

        static bool tryStartConversation(Character* a, Character* b);

		const std::string& getName();
        float getHealth() const;
        float getDamage() const;
        float getMovementSpeed() const;
        float getInteractRadius() const;
        Character* getTalkingTo() const;

		void setHealth(float health);
		void setDamage(float damage);
        void setMovementSpeed(float movementSpeed);
    };
}
