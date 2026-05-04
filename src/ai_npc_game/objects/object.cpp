#include "object.h"


namespace ai_npc {
    uint32_t Object::nextId = 0;

    // Delegating default constructor
    Object::Object() : Object(glm::vec3(0.0f)) { }

    Object::Object(glm::vec3 position)
        : id(nextId++),
          rotation(FloatMod(), FloatMod(), FloatMod()),
          meshRotation(FloatMod(), FloatMod(), FloatMod()),
          meshRotationCorrection(FloatMod(), FloatMod(), FloatMod()),
          position(position),
          scale(glm::vec3(1, 1, 1)),
          forward(glm::normalize(glm::vec3(0, 0, -1))),
          up(glm::normalize(glm::vec3(0, 1, 0))),
          right(glm::cross(forward, up)),
          modelMatrix(glm::mat4(1.0f)),
          redoModelMatrix(true),
          mesh(nullptr),
		  shader(nullptr)
    { }

    Object::Object(glm::vec3 position, Mesh *mesh, Shader *shader)
        : Object(position)
    {
        this->mesh = mesh;
        this->shader = shader;
    }

    void Object::render(Camera* camera) {
        if (redoModelMatrix) {
            modelMatrix = glm::translate(glm::mat4(1.0f), position);
            modelMatrix = glm::scale(modelMatrix, scale);
            modelMatrix = glm::rotate(modelMatrix, (float)RADIANS(rotation.x + meshRotation.x + meshRotationCorrection.x), glm::vec3(1, 0, 0));
            modelMatrix = glm::rotate(modelMatrix, (float)RADIANS(rotation.y + meshRotation.y + meshRotationCorrection.y), glm::vec3(0, 1, 0));
            modelMatrix = glm::rotate(modelMatrix, (float)RADIANS(rotation.z + meshRotation.z + meshRotationCorrection.z), glm::vec3(0, 0, 1));
            redoModelMatrix = false;
        }

        if (!mesh || !shader || !shader->program)
            return;

        // Render an object using the specified shader and the specified position
        shader->Use();
        glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
        glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));
        glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

        glm::mat4 bones[200];

        for (int i = 0; i < (int)mesh->m_BoneInfo.size(); i++)
        {
            bones[i] = mesh->m_BoneInfo[i].finalTransformation;
        }

        int bonesLocation = glGetUniformLocation(shader->program, "Bones");
        glUniformMatrix4fv(bonesLocation, (GLsizei)mesh->m_BoneInfo.size(), GL_FALSE,
            glm::value_ptr(bones[0]));

        mesh->Render();
    }

    void Object::moveForward(float distance) {
        glm::vec3 direction = glm::vec3(forward.x, 0, forward.z);
        position += direction * distance;
        redoModelMatrix = true;
    }

    void Object::moveRight(float distance) {
        glm::vec3 direction = glm::vec3(right.x, 0, right.z);
        position += direction * distance;
        redoModelMatrix = true;
    }

    void Object::rotateOX(float angle) {
        rotation.x = angle;
        rotate();
    }

    void Object::rotateOY(float angle) {
        rotation.y = angle;
        rotate();
    }

    void Object::rotateOZ(float angle) {
        rotation.z = angle;
        rotate();
    }

    void Object::rotate() {
        glm::mat4 rot = glm::mat4(1.0f);
        rot = glm::rotate(rot, (float)RADIANS(rotation.x), glm::vec3(1, 0, 0));
        rot = glm::rotate(rot, (float)RADIANS(rotation.y), glm::vec3(0, 1, 0));
        rot = glm::rotate(rot, (float)RADIANS(rotation.z), glm::vec3(0, 0, 1));

        forward = glm::normalize(glm::vec3(rot * glm::vec4(0, 0, -1, 0)));
        right = glm::normalize(glm::vec3(rot * glm::vec4(1, 0, 0, 0)));
        up = glm::normalize(glm::vec3(rot * glm::vec4(0, 1, 0, 0)));

        redoModelMatrix = true;
    }

    void Object::uniformScale(float value) {
        scale *= value;
        redoModelMatrix = true;
    }

    uint32_t Object::getId() const { return id; }
    glm::vec3 Object::getForward() const { return forward; }
    glm::vec3 Object::getRight() const { return right; }
    glm::vec3 Object::getUp() const { return up; }
    glm::vec3 Object::getPosition() const { return position; }
    glm::vec3 Object::getRotation() const { return rotation; }
    glm::vec3 Object::getScale() const { return scale; }
    Mesh *Object::getMesh() { return mesh; }
    Shader *Object::getShader() { return shader; }
    Vec3Mod Object::getMeshRotation() const { return meshRotation; }
    Vec3Mod Object::getMeshRotationCorrection() const { return meshRotationCorrection; }

    void Object::setForward(glm::vec3 forward) { this->forward = forward; }
    void Object::setRight(glm::vec3 right) { this->right = right; }
    void Object::setUp(glm::vec3 up) { this->up = up; }
    void Object::setPosition(glm::vec3 position) { this->position = position; }
    void Object::setMesh(Mesh *mesh) { this->mesh = mesh; }
    void Object::setShader(Shader *shader) { this->shader = shader; }
    void Object::setMeshRotation(Vec3Mod meshRotation) {
        this->meshRotation = meshRotation;
        redoModelMatrix = true;
    }
    void Object::setMeshRotationCorrection(Vec3Mod meshRotationCorrection) {
        this->meshRotationCorrection = meshRotationCorrection;
        redoModelMatrix = true;
    }
}
