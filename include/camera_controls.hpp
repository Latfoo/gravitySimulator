#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

/**
 * @brief Orbit-style camera with pan and zoom controls.
 */
class CameraControls {
public:
    /**
     * @brief Constructor.
     * @param initialDistance Initial zoom distance.
     */
    CameraControls(float initialDistance = 2300.0f);

    /**
     * @brief Install GLFW input callbacks.
     * @param window GLFW window pointer.
     */
    void installCallbacks(GLFWwindow* window);

    /**
     * @brief Apply view transform.
     */
    void applyView();

    /**
     * @brief Reset camera to defaults.
     */
    void reset();

private:
    float distance;   
    float yaw, pitch;
    float panX, panY;

    bool leftMousePressed;
    bool middleMousePressed;
    double lastMouseX, lastMouseY;

    /**
     * @brief Mouse button callback.
     * @param window GLFW window pointer.
     * @param button Mouse button.
     * @param action Press/release action.
     * @param mods Modifier keys.
     */
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

    /**
     * @brief Cursor movement callback.
     * @param window GLFW window pointer.
     * @param xpos Mouse X position.
     * @param ypos Mouse Y position.
     */
    static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);

    /**
     * @brief Scroll wheel callback.
     * @param window GLFW window pointer.
     * @param xoffset Horizontal scroll offset.
     * @param yoffset Vertical scroll offset.
     */
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    static CameraControls* instance; ///< Active instance pointer.
};
