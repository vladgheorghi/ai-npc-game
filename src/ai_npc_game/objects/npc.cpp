#include "npc.h"


namespace ai_npc
{
    // Constructor with random spawn position
    NPC::NPC(Mesh* mesh, Shader* shader) : Character(glm::vec3(randFloat(-10.0f, 10.0f), 0.0f, randFloat(-10.0f, 10.0f)), mesh, shader) {
        name = mesh->GetMeshID();
        movingToPosition = false;
		degreesLeftToRotate = 0;
    }

    void NPC::moveToPosition(glm::vec3 position) {
        selectedPosition = position;
        // Calculate at which angle the NPC needs to rotate to face `selectedPosition`
        // Can be positive or negative to rotate clockwise or counter clockwise, whichever is faster
        lookAt(position);
		movingToPosition = true;
    }

    void NPC::updatePosition(float deltaTimeSeconds) {
        float rotationSpeed = 180.0f; // degrees per second

        // Rotate gradually clockwise or counter clockwise
        if (degreesLeftToRotate > 0.5) {
            float step = std::min((float)degreesLeftToRotate, rotationSpeed * deltaTimeSeconds);
            rotateOY((float)rotation.y + step);
            degreesLeftToRotate -= step;
        }
        else if (degreesLeftToRotate < -0.5) {
            float step = std::max((float)degreesLeftToRotate, -rotationSpeed * deltaTimeSeconds);
            rotateOY((float)rotation.y + step);
            degreesLeftToRotate -= step;
        } else {
            rotateOY((float)rotation.y + degreesLeftToRotate);
            degreesLeftToRotate = 0;
        }

        // When finished rotating, move towards the target if flag is set
        if (movingToPosition) {
            glm::vec3 toTarget = selectedPosition - position;
            float distToTarget = glm::length(toTarget);
            float step = movementSpeed * deltaTimeSeconds;

            if (step >= distToTarget) {
                position = selectedPosition;
                movingToPosition = false;
            }
            else {
                position += glm::normalize(toTarget) * step;
            }

            // Do movement animation only while moving
            Animation::BoneTransform(mesh, (float)Engine::GetElapsedTime());
            redoModelMatrix = true;
        }
    }

    void NPC::render(Camera* camera, float deltaTimeSeconds) {
        updatePosition(deltaTimeSeconds);

        Character::render(camera);
	}

    bool NPC::isMovingToPosition() const {
        return movingToPosition;
	}

    void NPC::lookAt(glm::vec3 position) {
        float targetAngle = DEGREES(atan2f(this->position.x - position.x, this->position.z - position.z));
        degreesLeftToRotate = targetAngle - (float)rotation.y;
    }

    void NPC::talkTo(Character *character) {
        if (!character->isNearby(this) || character->isTalking()) {
            return;
        }

        movingToPosition = false;
        talkingTo = character;
        lookAt(character->getPosition());
    }
}
