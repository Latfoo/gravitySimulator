#include <GLFW/glfw3.h>
#include <math.h>

static constexpr int SCREEN_WIDTH = 1200;
static constexpr int SCREEN_HEIGHT = 800;
typedef struct vector{
    float x;
    float y;
}vector;

void drawCircle(float centerX , float centerY , float radius, int res){

    glBegin(GL_TRIANGLE_FAN);
    glVertex2d(centerX,centerY);

    for(int i = 0; i < res+1; i++){
            float angle = 2.0f * M_PI * static_cast<float>(i)/res;
            float x = centerX + cos(angle) * radius;
            float y = centerY + sin(angle) * radius;
            glVertex2d(x,y);
    }

    glEnd();
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

    // Parameters
    int res = 100;
    int radius = 50;
    vector position = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    vector velocity = { 0.0f, 0.0f };
    vector acceleration = {0.0f, -9.81f}; //Earth

    float lastTime = glfwGetTime();

    // Rendering Loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        //Time
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Numerical Integration using semi-implicit Euler
        velocity.x += acceleration.x * deltaTime;
        velocity.y += acceleration.y * deltaTime;

        position.x += velocity.x * deltaTime;
        position.y += velocity.y * deltaTime;

        // Edge Protection
        if (position.x < radius || position.x > SCREEN_WIDTH - radius) velocity.x *= -1;
        if (position.y < radius || position.y > SCREEN_HEIGHT - radius) velocity.y *= -1;

        drawCircle(position.x, position.y, radius, res);
        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // --------------------- END of Simulation -------------------------
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
