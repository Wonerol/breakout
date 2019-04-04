#include "paddle.h"
#include <glad/glad.h>
#include "glm/gtc/type_ptr.hpp"

Paddle::Paddle()
{
    transformation_matrix = glm::mat4(1.0f);

    // Define a triangle
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
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

void Paddle::draw(unsigned int transform_location)
{
    glUniformMatrix4fv(transform_location, 1, GL_FALSE, glm::value_ptr(transformation_matrix));
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
