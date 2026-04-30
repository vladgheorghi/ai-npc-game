#pragma once

#include "utils/glm_utils.h"

namespace ai_npc
{
    class Camera
    {
     public:
        Camera()
        {
            position    = glm::vec3(0, 2, 5);
            forward     = glm::vec3(0, 0, -1);
            up          = glm::vec3(0, 1, 0);
            right       = glm::vec3(1, 0, 0);
            yaw = 0.0f;
            pitch = 0.0f;
            roll = 0.0f;
            distanceToTarget = 3;
        }

        Camera(const glm::vec3 &position, const glm::vec3 &center)
        {
            Set(position, center);
        }

        ~Camera()
        { }

        void Set(const glm::vec3& position, const glm::vec3& center)
        {
            this->position = position;
            glm::vec3 f = glm::normalize(center - position);

            yaw = atan2f(f.x, -f.z);
            pitch = asinf(glm::clamp(f.y, -1.f, 1.f));
            roll = 0.f;

            UpdateVectors();
        }

        void SetBack(glm::vec3 &position, glm::vec3 &forward, glm::vec3 &right, glm::vec3 &up)
        {
            this->position = position;
            this->forward = forward;
            this->right = right;
            this->up = up;
        }

        void MoveForward(float distance, glm::vec3 forwardUsed)
        {
            glm::vec3 dir = glm::normalize(glm::vec3(forwardUsed.x, 0, forwardUsed.z));
            position += dir * distance;
        }

        void TranslateForward(float distance)
        {
            position += glm::normalize(forward) * distance;
        }

        void TranslateUpward(float distance)
        {
            position += glm::normalize(up) * distance;
        }

        void TranslateRight(float distance)
        {
            position += glm::normalize(right) * distance;
        }

        void RotateFirstPerson_OX(float radians)
        {
            pitch += radians;
            // clamp so you can't flip upside-down
            pitch = glm::clamp(pitch, -glm::half_pi<float>() + 0.01f,
                glm::half_pi<float>() - 0.01f);
            UpdateVectors();
        }

        void RotateFirstPerson_OY(float radians)
        {
            yaw += radians;
            UpdateVectors();
        }

        void RotateFirstPerson_OZ(float radians)
        {
            roll += radians;
            UpdateVectors();
        }

        void RotateThirdPerson_OX(float radians)
        {
			float sensitivity = 0.002f;
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OX(sensitivity * radians);
            TranslateForward(-distanceToTarget);
        }

        void RotateThirdPerson_OY(float radians)
        {
            float sensitivity = 0.002f;
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OY(sensitivity * radians);
            TranslateForward(-distanceToTarget);
        }

        void RotateThirdPerson_OZ(float radians)
        {
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OZ(radians);
            TranslateForward(-distanceToTarget);
        }

        void UpdateVectors()
        {
            forward = glm::normalize(glm::vec3(
                cosf(pitch) * sinf(yaw),   // X
                sinf(pitch),               // Y
                -cosf(pitch) * cosf(yaw)    // Z
            ));

            // right stays horizontal (no pitch/roll yet)
            right = glm::normalize(glm::vec3(cosf(yaw), 0.f, sinf(yaw)));

            // up is perpendicular to both
            up = glm::cross(right, forward);

            // apply roll: rotate right and up around forward
            if (roll != 0.f) {
                glm::mat4 r = glm::rotate(glm::mat4(1.f), roll, forward);
                right = glm::normalize(glm::vec3(r * glm::vec4(right, 0.f)));
                up = glm::cross(right, forward);
            }
        }

        void FollowTarget(const glm::vec3& target)
        {
            position = target - forward * distanceToTarget;
        }

        glm::mat4 GetViewMatrix() const
        {
            
            return glm::lookAt(position, position + forward, up);
        }

        void SetProjectionMatrix(float fov, float aspectRatio, float zNear, float zFar)
        {
            projectionMatrix = glm::perspective(fov, aspectRatio, zNear, zFar);
		}

        glm::mat4 GetProjectionMatrix() const
        {
            return projectionMatrix;
		}

        glm::vec3 GetTargetPosition() const
        {
            return position + forward * distanceToTarget;
        }

        float GetRotationOX() const { return pitch; }
        float GetRotationOY() const { return yaw; }
        float GetRotationOZ() const { return roll; }

        void Zoom(float value) {
            // Set scroll sensitivity
            float sensitivity = 0.3f;

            // Keep 0.5f <= distanceToTarget <= 10.0f
            distanceToTarget = glm::clamp(distanceToTarget + sensitivity * value, 0.5f, 10.0f);
        }

     public:
        glm::mat4 projectionMatrix;
        float distanceToTarget;
        glm::vec3 position;
        float yaw;    // rotation around the world Y axis (left/right)
        float pitch;  // rotation around the local X axis (up/down)
        float roll;   // rotation around the local Z axis (tilt)
        glm::vec3 forward;
        glm::vec3 right;
        glm::vec3 up;
    };
}   // namespace implemented
