#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class CameraControls {
public:
    CameraControls(float initialDistance = 2300.0f);

    void installCallbacks(GLFWwindow* window);
    void applyView();

    void reset();

private:
    float distance;
    float yaw, pitch;
    float panX, panY;

    bool leftMousePressed;
    bool middleMousePressed;
    double lastMouseX, lastMouseY;

    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    // Pointer to currently active instance
    static CameraControls* instance;
};
