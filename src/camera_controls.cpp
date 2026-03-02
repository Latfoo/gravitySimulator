#include "camera_controls.hpp"
#include <GL/glu.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <iostream>

CameraControls* CameraControls::instance = nullptr;

CameraControls::CameraControls(float initialDistance)
    : distance(initialDistance), yaw(30.0f), pitch(-60.0f), panX(0.0f), panY(0.0f),
      leftMousePressed(false), middleMousePressed(false), lastMouseX(0.0), lastMouseY(0.0) {
    instance = this;
}

void CameraControls::installCallbacks(GLFWwindow* window) {
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetScrollCallback(window, scrollCallback);
}

void CameraControls::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (!instance) return;
    if (button == GLFW_MOUSE_BUTTON_LEFT)
        instance->leftMousePressed = (action == GLFW_PRESS);
    if (button == GLFW_MOUSE_BUTTON_MIDDLE)
        instance->middleMousePressed = (action == GLFW_PRESS);
}

void CameraControls::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
    if (!instance) return;
    double dx = xpos - instance->lastMouseX;
    double dy = ypos - instance->lastMouseY;

    if (instance->leftMousePressed) {
        instance->yaw += dx * 0.2f;
        instance->pitch += dy * 0.2f;
        if (instance->pitch > 89.9f) instance->pitch = 89.9f;
        if (instance->pitch < -89.9f) instance->pitch = -89.9f;
    }

    if (instance->middleMousePressed) {
        instance->panX += dx * 1.0f;
        instance->panY -= dy * 1.0f;
    }

    instance->lastMouseX = xpos;
    instance->lastMouseY = ypos;
}

void CameraControls::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    if (!instance) return;
    instance->distance *= (1.0f - yoffset * 0.1f);
    if (instance->distance < 10.0f) instance->distance = 10.0f;
}

void CameraControls::applyView() {
    float pitchRad = glm::radians(pitch);
    float yawRad = glm::radians(yaw);

    float camX = distance * cos(pitchRad) * sin(yawRad);
    float camY = distance * sin(pitchRad);
    float camZ = distance * cos(pitchRad) * cos(yawRad);

    glm::vec3 eye = glm::vec3(camX, camY, camZ) + glm::vec3(panX, panY, 0.0f);
    glm::vec3 center = glm::vec3(panX, panY, 0.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z);
}

void CameraControls::reset() {
    yaw = 0.0f; pitch = 30.0f; panX = panY = 0.0f;
    distance = 2300.0f;
}
