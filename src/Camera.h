#pragma once
#ifndef LAB1_CAMERA_H
#define LAB1_CAMERA_H

/**
 * @file Camera.h
 *
 * @brief Camera that looks at the world from a perspective.
 *
 * Martin Johnson
 * Billy-Joe Lacasse
 * William Lebel
 */

#include "glm/vec3.hpp"
#include "GLFW/glfw3.h"
#include "glm/fwd.hpp"

class Camera {

public:
    explicit Camera(glm::vec3 position,glm::vec3 rotation = glm::vec3(180.0,0.0,0.0))
    :m_Pos(position), m_rotation(rotation){
        updateCamera();
    }

    void SetSpeed(float speed){
        m_speed = speed;
    }
    void SetMouseSensitivity(float mouseSensitivity){
        m_mouseSensitivity = mouseSensitivity;
    }

    void ProcessKeyboard(GLFWwindow* window, float deltaTime);
    void ProcessMouseMovement(float xDelta, float yDelta);
    void ProcessMouseScroll(float delta);

    glm::vec3 GetPosition() {
        return m_Pos;
    }

    glm::mat4 GetViewMatrix();

    float Zoom = 45.0f;

private:
    glm::vec3 m_Pos;
    glm::vec3 m_front = glm::vec3 (0.0,0.0,-1.0);
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldUp  = glm::vec3 (0.0,1.0,0.0);
    glm::vec3 m_rotation{};

    float m_speed = 2.5f;
    float m_mouseSensitivity = 0.002f;

    void updateCamera();
};


#endif //LAB1_CAMERA_H
