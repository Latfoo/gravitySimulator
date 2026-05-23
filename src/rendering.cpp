#include <GLFW/glfw3.h>
#include <math.h>
#include "Planet.hpp"
#include "functions.hpp"


void drawPlanet(Planet& planet) {
    glPushMatrix();
    glTranslated(planet.position.x, planet.position.y, planet.position.z);

    const int layers = planet.res;
    const int slices = planet.res;
    const float radius = planet.radius;

    for (int i = 0; i < layers; ++i) {
        float phi1 = M_PI * i / layers;
        float phi2 = M_PI * (i + 1) / layers;

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= slices; ++j) {
            float theta = 2.0f * M_PI * j / slices;

            float x1 = radius * sin(phi1) * cos(theta);
            float y1 = radius * cos(phi1);
            float z1 = radius * sin(phi1) * sin(theta);

            float x2 = radius * sin(phi2) * cos(theta);
            float y2 = radius * cos(phi2);
            float z2 = radius * sin(phi2) * sin(theta);

            glNormal3f(x1 / radius, y1 / radius, z1 / radius);
            glVertex3f(x1, y1, z1);

            glNormal3f(x2 / radius, y2 / radius, z2 / radius);
            glVertex3f(x2, y2, z2);
        }
        glEnd();
    }

    glPopMatrix();
}
