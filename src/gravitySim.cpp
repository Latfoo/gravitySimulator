#include <GLFW/glfw3.h>
#include <math.h>
#include <vector>
#include "glm/glm.hpp"
#include "Planet.hpp"


static constexpr int SCREEN_WIDTH = 1500;
static constexpr int SCREEN_HEIGHT = 920;
static constexpr float G = 6.67430e-11f / 100000000; // gravitational constant (adjust scale as needed)
static constexpr bool BORDERS = false; // Activate or Deactivate Screen Boarders


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


//reverses velocity if screen edge is hit
void checkEdgeProtection(Planet &planet){
    if (planet.position.x < planet.radius || planet.position.x > SCREEN_WIDTH - planet.radius) planet.velocity.x *= -1;
    if (planet.position.y < planet.radius || planet.position.y > SCREEN_HEIGHT - planet.radius) planet.velocity.y *= -1;
}


// Classical Newton Gravitation Formula
glm::vec2 computeGravitationalForce(const Planet& a, const Planet& b) {
    glm::vec2 r = b.position - a.position;
    float distanceSquared = glm::dot(r, r); 
    float distance = sqrt(distanceSquared);

    if (distance < 1e-5f) return glm::vec2(0.0f);

    glm::vec2 forceDir = glm::normalize(r);
    float forceMagnitude = (G * a.mass * b.mass) / distanceSquared;
    return forceDir * forceMagnitude;
}


void updateAccelerations(std::vector<Planet> &planets) {
    for (auto& planet : planets) {
        glm::vec2 netForce(0.0f);

        for (const auto& other : planets) {
            if (&planet != &other) {
                netForce += computeGravitationalForce(planet, other);
            }
        }

        planet.acceleration = netForce / planet.mass;
    }
}


void calculateMovement(std::vector<Planet> &planets, float deltaTime){

    updateAccelerations(planets);   // Analyzes gravitational forces and updates Accelerations

    for(auto& planet : planets){

        // Euler (semi-implicit)
        planet.velocity += planet.acceleration * deltaTime;
        planet.position += planet.velocity * deltaTime;

        if(BORDERS) checkEdgeProtection(planet);
    }
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

    // 2D Projection and Background Settings
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, -1, 1);  // orthographic projection
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(0, 0, 0, 1);

    // --------------------- Simulation -------------------------

    // Initial Conditions for Planet 1, 2 and 3
    int res1 = 100;
    float radius1 = 50.0f;
    float mass1 = 5.972e24;
    glm::vec2 position1 = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    glm::vec2 velocity1 = { 0.0f, 0.0f };
    glm::vec2 acceleration1 = {0.0f, 0.0f};

    int res2 = 100;
    float radius2 = 20.0f;
    float mass2 = 7.348e22;
    glm::vec2 position2 = { SCREEN_WIDTH / 4.0f, SCREEN_HEIGHT / 1.5f };
    glm::vec2 velocity2 = { 50.0f, 70.0f };
    glm::vec2 acceleration2 = {0.0f, 0.0f};

    int res3 = 100;
    float radius3 = 20.0f;
    float mass3 = 7.348e22;
    glm::vec2 position3 = { SCREEN_WIDTH / 1.1f, SCREEN_HEIGHT / 1.2f };
    glm::vec2 velocity3 = { -50.0f, -70.0f };
    glm::vec2 acceleration3 = {0.0f, 0.0f};

    std::vector<Planet> planets;
    Planet planet1(radius1, res1, mass1, position1, velocity1, acceleration1);
    planets.push_back(planet1);
    Planet planet2(radius2, res2, mass2, position2, velocity2, acceleration2);
    planets.push_back(planet2);
    Planet planet3(radius3, res3, mass3, position3, velocity3, acceleration3);
    planets.push_back(planet3);


    // Rendering Loop
    float lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);


        // time
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // physics and rendering
        calculateMovement(planets, deltaTime);
        for(auto& planet : planets){
            drawPlanet(planet);
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // --------------------- END of Simulation -------------------------
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
