#include "characters/character.h"

#include "utils/glm_utils.h"
#include "utils/math_utils.h"

namespace ai_npc {
    Character::Character(glm::vec3 position)
    {
        setPosition(position);

        setForward(glm::normalize(glm::vec3(0, 0, -1)));
        setUp(glm::normalize(glm::vec3(0, 1, 0)));
        setRight(glm::cross(forward, up));

        setMovementSpeed(2.5f);
    }

    glm::vec3 Character::getPosition() { return position; }
    glm::vec3 Character::getForward() { return forward; }
    glm::vec3 Character::getRight() { return right; }
    glm::vec3 Character::getUp() { return up; }
    float Character::getMovementSpeed() { return movementSpeed; }

    void Character::setPosition(glm::vec3 position) { this->position = position; }
    void Character::setForward(glm::vec3 forward) { this->forward = forward; }
    void Character::setRight(glm::vec3 right) { this->right = right; }
    void Character::setUp(glm::vec3 up) { this->up = up; }
    void Character::setMovementSpeed(float movementSpeed) { this->movementSpeed = movementSpeed; }
}
