#include <GLFW/glfw3.h>
#include <math.h>
#include "glm/glm.hpp"
#include "structs.hpp"


static constexpr int SCREEN_WIDTH = 1200;
static constexpr int SCREEN_HEIGHT = 800;


void drawCircle(Circle &circle){

    glBegin(GL_TRIANGLE_FAN);
    glVertex2d(circle.position.x, circle.position.y);

    for(int i = 0; i < circle.res+1; i++){
            float angle = 2.0f * M_PI * static_cast<float>(i)/circle.res;
            float x = circle.position.x + cos(angle) * circle.radius;
            float y = circle.position.y + sin(angle) * circle.radius;
            glVertex2d(x,y);
    }

    glEnd();
}


void Euler(Circle &circle, float deltaTime){
        circle.velocity.x += circle.acceleration.x * deltaTime;
        circle.velocity.y += circle.acceleration.y * deltaTime;

        circle.position.x += circle.velocity.x * deltaTime;
        circle.position.y += circle.velocity.y * deltaTime;
}


void checkEdgeProtection(Circle &circle){
    if (circle.position.x < circle.radius || circle.position.x > SCREEN_WIDTH - circle.radius) circle.velocity.x *= -1;
    if (circle.position.y < circle.radius || circle.position.y > SCREEN_HEIGHT - circle.radius) circle.velocity.y *= -1;
}


int main() {

    //Setup Window
    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "GLFW Window", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Projection and Background Settings
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, -1, 1);  // orthographic projection
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(0, 0, 0, 1);

    // --------------------- Simulation -------------------------

    int res = 100;
    int radius = 50;
    Vector position = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    Vector velocity = { 0.0f, 0.0f };
    Vector acceleration = {0.0f, -9.81f};

    Circle circle1(res,radius,position,velocity,acceleration);

    // Rendering Loop
    float lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        Euler(circle1, deltaTime);
        checkEdgeProtection(circle1);
        drawCircle(circle1);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // --------------------- END of Simulation -------------------------
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
