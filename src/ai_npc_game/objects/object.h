#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"

#include "../utils/floatMod.h"

namespace ai_npc
{
    class Object
    {
        // class fields
    public:
    private:
    protected:
    glm::vec3 forward;
    glm::vec3 right;
    glm::vec3 up;
    
    glm::vec3 position;
    glm::vec<3, floatMod, glm::packed_highp> rotation;
    glm::vec3 scale;

    glm::mat4 modelMatrix;
    bool redoModelMatrix;

    Shader *shader;
    Mesh *mesh;

        // class methods
    public:
        Object();
        Object(glm::vec3 position);
        Object(glm::vec3 position, Mesh *mesh, Shader *shader);
        ~Object();

        void render();

        void moveForward(float distance);
        void moveRight(float distance);
        void rotateOX(float radians);
        void rotateOY(float radians);
        void rotateOZ(float radians);
        void uniformScale(float value);

        glm::vec3 getForward();
        glm::vec3 getRight();
        glm::vec3 getUp();
        glm::vec3 getPosition();
        glm::vec3 getRotation();
        glm::vec3 getScale();
        Mesh *getMesh();
        Shader *getShader();

        void setPosition(glm::vec3 position);
        void setForward(glm::vec3 forward);
        void setRight(glm::vec3 right);
        void setUp(glm::vec3 up);
        void setMesh(Mesh *mesh);
        void setShader(Shader *shader);
    private:
        void rotate();
    protected:
    };
}
