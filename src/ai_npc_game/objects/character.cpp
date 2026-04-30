#include "character.h"

namespace ai_npc {
    Character::Character(glm::vec3 position) : Object(position) {
        name = "Character";
		health = 100.0f;
		damage = 10.0f;
        movementSpeed = 2.5f;
    }

    Character::Character(glm::vec3 position, Mesh *mesh, Shader *shader) : Object(position, mesh, shader) {
        name = "Character";
        health = 100.0f;
        damage = 10.0f;
        movementSpeed = 2.5f;
    }

    Character::~Character() = default;

    void Character::moveForward(float distance) {
        Object::moveForward(distance * movementSpeed);
    }

    void Character::moveRight(float distance) {
        Object::moveRight(distance * movementSpeed);
    }

	const char* Character::getName() const { return name.c_str(); }
	float Character::getHealth() const { return health; }
	float Character::getDamage() const { return damage; }
    float Character::getMovementSpeed() const { return movementSpeed; }

    void Character::setHealth(float health) { this->health = health; }
    void Character::setDamage(float damage) { this->damage = damage; }
    void Character::setMovementSpeed(float movementSpeed) { this->movementSpeed = movementSpeed; }
}
