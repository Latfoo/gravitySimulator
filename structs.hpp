#pragma once

struct Vector {
    float x;
    float y;
};

struct Circle {
    int res;
    int radius;
    Vector position;
    Vector velocity;
    Vector acceleration;

    Circle() = default;
    Circle(int radius, int res, Vector position, Vector velocity, Vector acceleration)
        : radius(radius), res(res), position(position), velocity(velocity), acceleration(acceleration) {}
};