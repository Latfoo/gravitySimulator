#include <GLFW/glfw3.h>
#include <math.h>
#include "glm/glm.hpp"
#include "Planet.hpp"


static constexpr int SCREEN_WIDTH = 1200;
static constexpr int SCREEN_HEIGHT = 800;


void drawPlanet(Planet &planet){

    glBegin(GL_TRIANGLE_FAN);
    glVertex2d(planet.position.x, planet.position.y);

    for(int i = 0; i < planet.res+1; i++){
            float angle = 2.0f * M_PI * static_cast<float>(i)/planet.res;
            float x = planet.position.x + cos(angle) * planet.radius;
            float y = planet.position.y + sin(angle) * planet.radius;
            glVertex2d(x,y);
    }

    glEnd();
}


void Euler(Planet &planet, float deltaTime){
        planet.velocity.x += planet.acceleration.x * deltaTime;
        planet.velocity.y += planet.acceleration.y * deltaTime;

        planet.position.x += planet.velocity.x * deltaTime;
        planet.position.y += planet.velocity.y * deltaTime;
}


void checkEdgeProtection(Planet &planet){
    if (planet.position.x < planet.radius || planet.position.x > SCREEN_WIDTH - planet.radius) planet.velocity.x *= -1;
    if (planet.position.y < planet.radius || planet.position.y > SCREEN_HEIGHT - planet.radius) planet.velocity.y *= -1;
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
    glm::vec2 position = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    glm::vec2 velocity = { 0.0f, 0.0f };
    glm::vec2 acceleration = {0.0f, -9.81f};

    Planet planet1(res,radius,position,velocity,acceleration);

    // Rendering Loop
    float lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        Euler(planet1, deltaTime);
        checkEdgeProtection(planet1);
        drawPlanet(planet1);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // --------------------- END of Simulation -------------------------
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
