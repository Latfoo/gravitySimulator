#pragma once

struct Planet {
    int radius;
    int res;
    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec2 acceleration;

    Planet() = default;
    Planet(int radius, int res, glm::vec2 position, glm::vec2 velocity, glm::vec2 acceleration)
        : radius(radius), res(res), position(position), velocity(velocity), acceleration(acceleration) {}
};