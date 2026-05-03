#pragma once

#include "utils/glm_utils.h"

#include "core/gpu/mesh.h"
#include "core/gpu/shader.h"

#include "ai_npc_game/camera/camera.h"
#include "ai_npc_game/utils/float_mod.h"

namespace ai_npc
{
    class Object
    {
        // class fields
    public:
    private:
    protected:
    glm::vec3 forward;
    glm::vec3 up;
    glm::vec3 right;
    
    glm::vec3 position;
    Vec3Mod rotation;
    glm::vec3 scale;

    glm::mat4 modelMatrix;
    bool redoModelMatrix;

    Shader *shader;
    Mesh *mesh;

    // Rotation offsets only applied to the mesh at render time
    Vec3Mod meshRotation;
    // Rotation offsets to correct irregularities from mesh file. Only applied to mesh at render time
    Vec3Mod meshRotationCorrection;

        // class methods
    public:
        Object();
        Object(glm::vec3 position);
        Object(glm::vec3 position, Mesh *mesh, Shader *shader);
        virtual ~Object() = default;

        void render(Camera* camera);

        virtual void moveForward(float distance);
        virtual void moveRight(float distance);
        void rotateOX(float angle);
        void rotateOY(float angle);
        void rotateOZ(float angle);
        void uniformScale(float value);

        glm::vec3 getForward() const;
        glm::vec3 getRight() const;
        glm::vec3 getUp() const;
        glm::vec3 getPosition() const;
        glm::vec3 getRotation() const;
        glm::vec3 getScale() const;
        Mesh *getMesh();
        Shader *getShader();
        Vec3Mod getMeshRotation() const;
        Vec3Mod getMeshRotationCorrection() const;

        void setPosition(glm::vec3 position);
        void setForward(glm::vec3 forward);
        void setRight(glm::vec3 right);
        void setUp(glm::vec3 up);
        void setMesh(Mesh *mesh);
        void setShader(Shader *shader);
        void setMeshRotation(Vec3Mod meshRotation);
        void setMeshRotationCorrection(Vec3Mod rotMeshCorrection);
    private:
        void rotate();
    protected:
    };
}
