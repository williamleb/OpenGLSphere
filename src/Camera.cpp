/**
 * @file Camera.cpp
 *
 * @brief Camera that looks at the world from a perspective.
 *
 * Martin Johnson
 * Billy-Joe Lacasse
 * William Lebel
 */

#include "Camera.h"
#include <glm/gtc/quaternion.hpp>

void Camera::ProcessKeyboard(GLFWwindow* window, float deltaTime) {
    float cameraSpeed = m_speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        cameraSpeed *= 2;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        m_Pos += cameraSpeed * m_front;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        m_Pos -= cameraSpeed * m_front;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        m_Pos -= glm::normalize(glm::cross(m_front, m_worldUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        m_Pos += glm::normalize(glm::cross(m_front, m_worldUp)) * cameraSpeed;
    }
}

void Camera::ProcessMouseMovement(float xDelta, float yDelta) {
    xDelta *= m_mouseSensitivity * Zoom;
    yDelta *= m_mouseSensitivity * Zoom;
    m_rotation.x += xDelta;
    m_rotation.y -= yDelta;

    if (m_rotation.y > 89.0f)
        m_rotation.y = 89.0f;
    if (m_rotation.y < -89.0f)
        m_rotation.y = -89.0f;
    updateCamera();
}

void Camera::ProcessMouseScroll(float delta) {
    Zoom -= delta;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(m_Pos, m_Pos + m_front, m_up);
}

void Camera::updateCamera() {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(m_rotation.x)) * cos(glm::radians(m_rotation.y));
        front.y = sin(glm::radians(m_rotation.y));
        front.z = sin(glm::radians(m_rotation.x)) * cos(glm::radians(m_rotation.y));
        m_front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        m_right = glm::normalize(glm::cross(m_front, m_worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        m_up= glm::normalize(glm::cross(m_right, m_front));


}
