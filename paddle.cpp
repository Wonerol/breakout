#include "paddle.h"
#include <glad/glad.h>

Paddle::Paddle()
{
    float vertices[] = {
         0.5f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
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

void Paddle::translate(float x)
{
    transformation_matrix = glm::translate(transformation_matrix, glm::vec3(x, 0.0f, 0.0f));
}

void Paddle::draw()
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

AABB Paddle::get_AABB()
{
    glm::vec4 centre = transformation_matrix[3];
    AABB aabb{ centre.x - 0.5, centre.x + 0.5, centre.y - 0.5, centre.y + 0.5 };

    return aabb;
}
