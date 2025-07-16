#include <GLFW/glfw3.h>
#include <math.h>

static constexpr int SCREEN_WIDTH = 1200;
static constexpr int SCREEN_HEIGHT = 800;
typedef struct vector{
    float x;
    float y;
}vector;

void drawCircle(float centerX = SCREEN_WIDTH / 2.0f, float centerY = SCREEN_HEIGHT / 2.0f, float radius = 50, int res = 100){

    glBegin(GL_TRIANGLE_FAN);
    glVertex2d(centerX,centerY);

    for(int i = 0; i < res+1; i++){
            float angle = 2.0f * M_PI * (float)i/res;
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

    //Running the simulation
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        drawCircle();
        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Close window
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
