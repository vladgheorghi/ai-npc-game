#include "character.h"
#include "npc.h"
#include "ai_npc_game/hud/chat.h"

namespace ai_npc {
    Character::Character(glm::vec3 position) : Object(position) {
        name = "Character";
		health = 100.0f;
		damage = 10.0f;
        movementSpeed = 2.5f;
        interactRadius = 2.0f;
        talkingTo = nullptr;
    }

    Character::Character(glm::vec3 position, Mesh *mesh, Shader *shader) : Object(position, mesh, shader) {
        name = "Character";
        health = 100.0f;
        damage = 10.0f;
        movementSpeed = 2.5f;
        interactRadius = 2.0f;
        talkingTo = nullptr;
    }

    Character::~Character() = default;

    void Character::moveForward(float distance) {
        Object::moveForward(distance * movementSpeed);
    }

    void Character::moveRight(float distance) {
        Object::moveRight(distance * movementSpeed);
    }

    bool Character::isNearby(Object* object) {
        return glm::distance(position, object->getPosition()) < interactRadius;
    }

    void Character::talkTo(NPC* npc, Chat* chat) {
        if (!npc->isNearby(this) || npc->isTalking()) {
            return;
        }

        npc->talkTo(this);
        talkingTo = npc;
    }

    bool Character::isTalking() {
        return talkingTo != nullptr;
    }

    void Character::stopTalking() {
        if (talkingTo == nullptr) return;
        talkingTo->talkingTo = nullptr;
        talkingTo = nullptr;
    }

	const char* Character::getName() const { return name.c_str(); }
	float Character::getHealth() const { return health; }
	float Character::getDamage() const { return damage; }
    float Character::getMovementSpeed() const { return movementSpeed; }
    float Character::getInteractRadius() const { return interactRadius; }
    Character* Character::getTalkingTo() const { return talkingTo; }

    void Character::setHealth(float health) { this->health = health; }
    void Character::setDamage(float damage) { this->damage = damage; }
    void Character::setMovementSpeed(float movementSpeed) { this->movementSpeed = movementSpeed; }
}
