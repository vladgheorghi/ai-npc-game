#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"
#include <iostream>


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
            distanceToTarget = 3;
        }

        Camera(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
        {
            Set(position, center, up);
        }

        ~Camera()
        { }

        // Update camera
        void Set(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
        {
            this->position = position;
            forward     = glm::normalize(center - position);
            right       = glm::cross(forward, up);
            this->up    = glm::cross(right, forward);
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
            // Translates the camera using the `dir` vector computed from
            // `forward`. Movement will always keep the camera at the same
            // height. For example, if you rotate your head up/down, and then
            // walk forward, then you will still keep the same relative
            // distance (height) to the ground!
            glm::vec3 dir = glm::normalize(glm::vec3(forwardUsed.x, 0, forwardUsed.z));
            position += dir * distance;
        }

        void TranslateForward(float distance)
        {
            // TODO(student): Translate the camera using the `forward` vector.
            // What's the difference between `TranslateForward()` and
            // `MoveForward()`?

            position += glm::normalize(forward) * distance;
        }

        void TranslateUpward(float distance)
        {
            // TODO(student): Translate the camera using the `up` vector.

            position += glm::normalize(up) * distance;
        }

        void TranslateRight(float distance)
        {
            // TODO(student): See instructions below. Read the entire thing!
            // You need to translate the camera using the `right` vector.
            // Usually, however, translation using camera's `right` vector
            // is not very useful, because if the camera is rotated around the
            // `forward` vector, then the translation on the `right` direction
            // will have an undesired effect, more precisely, the camera will
            // get closer or farther from the ground. The solution is to
            // actually use the projected `right` vector (projected onto the
            // ground plane), which makes more sense because we will keep the
            // same distance from the ground plane.

            position += glm::normalize(right) * distance;

        }

        void RotateFirstPerson_OX(float angle)
        {
            // TODO(student): Compute the new `forward` and `up` vectors.
            // Don't forget to normalize the vectors! Use `glm::rotate()`.
            
            glm::mat4 RotateWorldOX = glm::rotate(glm::mat4(1.0f), angle, right);

            // forward
            glm::vec4 newForward = RotateWorldOX * glm::vec4(forward, 1.0f);
            forward = glm::normalize(glm::vec3(newForward));

            // up
            up = glm::cross(right, forward);

        }

        void RotateFirstPerson_OY(float angle)
        {
            // TODO(student): Compute the new `forward`, `up` and `right`
            // vectors. Use `glm::rotate()`. Don't forget to normalize the
            // vectors!

            glm::mat4 RotateWorldOY = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0));

            // forward
            glm::vec4 newForward = RotateWorldOY * glm::vec4(forward, 1.0f);
            forward = glm::normalize(glm::vec3(newForward));

            // right
            glm::vec4 newRight = RotateWorldOY * glm::vec4(right, 1.0f);
            right = glm::normalize(glm::vec3(newRight));

            // up
            up = glm::cross(right, forward);
        }

        void RotateFirstPerson_OZ(float angle)
        {
            // TODO(student): Compute the new `right` and `up`. This time,
            // `forward` stays the same. Use `glm::rotate()`. Don't forget
            // to normalize the vectors!

            glm::mat4 RotateWorldOZ = glm::rotate(glm::mat4(1.0f), angle, forward);

            // right
            glm::vec4 newRight = RotateWorldOZ * glm::vec4(right, 1.0f);
            right = glm::normalize(glm::vec3(newRight));

            // up
            up = glm::cross(right, forward);
        }

        void RotateThirdPerson_OX(float angle)
        {
            // TODO(student): Rotate the camera in third-person mode around
            // the OX axis. Use `distanceToTarget` as translation distance.

            TranslateForward(distanceToTarget);
            RotateFirstPerson_OX(angle);
            TranslateForward(-distanceToTarget);

        }

        void RotateThirdPerson_OY(float angle)
        {
            // TODO(student): Rotate the camera in third-person mode around
            // the OY axis.

            TranslateForward(distanceToTarget);
            RotateFirstPerson_OY(angle);
            TranslateForward(-distanceToTarget);

        }

        void RotateThirdPerson_OZ(float angle)
        {
            // TODO(student): Rotate the camera in third-person mode around
            // the OZ axis.

            TranslateForward(distanceToTarget);
            RotateFirstPerson_OZ(angle);
            TranslateForward(-distanceToTarget);

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

     public:
        glm::mat4 projectionMatrix;
        float distanceToTarget;
        glm::vec3 position;
        glm::vec3 forward;
        glm::vec3 right;
        glm::vec3 up;
    };
}   // namespace implemented
