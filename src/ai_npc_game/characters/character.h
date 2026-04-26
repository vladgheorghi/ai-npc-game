#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"

namespace ai_npc
{
    class Character
    {
        // class fields
        public:
        private:
        protected:
            glm::vec3 position;
            glm::vec3 forward;
            glm::vec3 right;
            glm::vec3 up;

        // class methods
        public:
            Character();
            Character(glm::vec3 position);
            ~Character();

            glm::vec3 getPosition();
            glm::vec3 getForward();
            glm::vec3 getRight();
            glm::vec3 getUp();

            void setPosition(glm::vec3 position);
            void setForward(glm::vec3 forward);
            void setRight(glm::vec3 right);
            void setUp(glm::vec3 up);
        private:
        protected:
    }
}
