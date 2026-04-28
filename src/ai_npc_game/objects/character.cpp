#include "character.h"

#include "utils/glm_utils.h"
#include "utils/math_utils.h"

namespace ai_npc {
    Character::Character(glm::vec3 position) : Object(position) { setMovementSpeed(2.5f); }

    Character::Character(glm::vec3 position, Mesh *mesh, Shader *shader) : Object(position, mesh, shader) { setMovementSpeed(2.5f); }

    void Character::moveForward(float distance) {
        Object::moveForward(distance * movementSpeed);
    }

    void Character::moveRight(float distance) {
        Object::moveRight(distance * movementSpeed);
    }

    float Character::getMovementSpeed() const { return movementSpeed; }

    void Character::setMovementSpeed(float movementSpeed) { this->movementSpeed = movementSpeed; }
}
