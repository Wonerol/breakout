#include "ball.h"
#include <algorithm>

Ball::Ball()
{
    name = "Ball";

    velocity[0] = 0.3f;
    velocity[1] = 0.7f;

    speed = 2.0f;
}

void Ball::update()
{
    if (bounce_queued) {
        bounce_queued = false;
        bounce(bounce_axis);
    }
}

void Ball::queue_bounce(char axis)
{
    bounce_queued = true;
    bounce_axis = axis;
}

void Ball::bounce(char axis)
{
    int index = -1;

    if (axis == 'x') {
        index = 0;
    } else if (axis == 'y') {
        index = 1;
    }

    velocity[index] *= -1.1;

    // keep the ball from moving unreasonably fast and tunneling
    float max_speed = 2.0f;
    velocity[index] = std::clamp(velocity[index], -max_speed, max_speed);
}
