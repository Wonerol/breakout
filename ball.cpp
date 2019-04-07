#include "ball.h"
#include <glad/glad.h>
#include "glm/gtc/type_ptr.hpp"

Ball::Ball()
{
    transformation_matrix = glm::mat4(1.0f);

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

AABB Ball::get_AABB()
{
    glm::vec4 centre = transformation_matrix[3];
    AABB aabb{ centre.x - 0.5, centre.x + 0.5, centre.y - 0.5, centre.y + 0.5 };

    return aabb;
}