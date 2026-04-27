#include "character.h"

#include "utils/glm_utils.h"
#include "utils/math_utils.h"

namespace ai_npc {
    Character::Character(glm::vec3 position) : Object(glm::vec3 position)
    {
        setMovementSpeed(2.5f);
    }

    float Character::getMovementSpeed() { return movementSpeed; }

    void Character::setMovementSpeed(float movementSpeed) { this->movementSpeed = movementSpeed; }
}
