#include "object.h"

#include "utils/glm_utils.h"
#include "utils/math_utils.h"

namespace ai_npc {
    Object::Object(glm::vec3 position) {
        this->position = position;

        forward = glm::normalize(glm::vec3(0, 0, -1));
        up = glm::normalize(glm::vec3(0, 1, 0));
        right = glm::cross(forward, up);

        modelMatrix = glm::mat4(1.0f);
        redoModelMatrix = false;

        mesh = nullptr;
        shader = nullptr;
    }

    Object::Object(glm::vec3 position, Mesh *mesh, Shader *shader) {
        this->Object(position);

        this->mesh = mesh;
        this->shader = shader;
    }

    void Object::render() {
        if (redoModelMatrix) {
            modelMatrix = glm::translate(glm::mat4(1.0f), position);
            modelMatrix = glm::scale(modelMatrix, scale);
            modelMatrix = glm::rotate(modelMatrix, rotation);
        }

        if (!mesh || !shader || !shader->program)
            return;

        // Render an object using the specified shader and the specified position
        shader->Use();
        glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetViewMatrix()));
        glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetProjectionMatrix()));
        glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

        glm::mat4 bones[200];

        for (int i = 0; i < mesh->m_BoneInfo.size(); i++)
        {
            bones[i] = mesh->m_BoneInfo[i].finalTransformation;
        }

        // TODO (student): Send the bone final transformation to the shader
        int bonesLocation = glGetUniformLocation(shader->program, "Bones");
        glUniformMatrix4fv(bonesLocation, (GLsizei)mesh->m_BoneInfo.size(), GL_FALSE,
            glm::value_ptr(bones[0]));

        mesh->Render();
    }

    void Object::moveForward(float distance) {
        glm::vec3 direction = glm::vec3(forward.x, 0, forward.z);
        positon += direction * distance;
        redoModelMatrix = true;
    }

    void Object::moveRight(float distance) {
        glm::vec3 direction = glm::vec3(right.x, 0, right.z);
        position += direction * distance;
        redoModelMatrix = true;
    }

    void Object::rotateOX(float radians) {
        rotation.x += radians;
        rotate();
        redoModelMatrix = true;
    }

    void Object::rotateOY(float radians) {
        rotation.y += radians;
        rotate();
        redoModelMatrix = true;
    }

    void Object::rotateOZ(float radians) {
        rotation.z += radians;
        rotate();
        redoModelMatrix = true;
    }

    void Object::rotate() {
        glm::mat4 rotateWorldOY = glm::rotate(glm::mat4(1.0f), radians, rotation);

        glm::vec4 newForward = rotateWorldOY * glm::vec4(forward, 1.0f);
        forward = glm::normalize(glm::vec3(newForward));

        glm::vec4 newRight = rotateWorldOY * glm::vec4(right, 1.0f);
        right = glm::normalize(glm::vec3(newRight));

        up = glm::cross(forward, right);
    }

    void Object::uniformScale(float value) {
        scale += value;
        redoModelMatrix = true;
    }

    glm::vec3 Object::getForward() { return forward; }
    glm::vec3 Object::getRight() { return right; }
    glm::vec3 Object::getUp() { return up; }
    glm::vec3 Object::getPosition() { return position; }
    glm::vec3 Object::getRotation() { return rotation; }
    glm::vec3 Object::getScale() { return scale; }
    Mesh *Object::getMesh() { return mesh; }
    Shader *Object::getShader() { return shader; }

    void Object::setForward(glm::vec3 forward) { this->forward = forward; }
    void Object::setRight(glm::vec3 right) { this->right = right; }
    void Object::setUp(glm::vec3 up) { this->up = up; }
    void Object::setPosition(glm::vec3 position) { this->position = position; }
    void Object::setMesh(Mesh *mesh) { this->mesh = mesh; }
    void Object::setShader(Shader *shader) { this->shader = shader; }
}
