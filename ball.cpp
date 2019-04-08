#include "ball.h"
#include <glad/glad.h>
#include <algorithm>

Ball::Ball()
{
    name = "Ball";

    velocity[0] = 0.3f;
    velocity[1] = 0.7f;

    speed = 2.0f;

    float vertices[] = {
        0.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f,
        0.5f, 0.25f, 0.0f,
        0.5f, -0.25f, 0.0f,
        0.0f, -0.5f, 0.0f,
        -0.5f, -0.25f, 0.0f,
        -0.5f, 0.25f, 0.0f,
    };

    unsigned int indices[] = {
        0, 1, 2, 3, 4, 5, 6, 1
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    unsigned int EBO;
    glGenBuffers(1, &EBO);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Copy vertex array to buffer for OpenGL
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Explain to OpenGL how the vertex data is structured
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Ball::draw()
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLE_FAN, 8, GL_UNSIGNED_INT, 0);
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
