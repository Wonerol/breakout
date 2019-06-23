#include "graphics_system.h"

#include <glad/glad.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>

GraphicsSystem::GraphicsSystem()
{
    create_square_VAO();
    create_hexagon_VAO();
}

void GraphicsSystem::create_hexagon_VAO()
{
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

    glGenVertexArrays(1, &hexagon_VAO);
    glBindVertexArray(hexagon_VAO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Copy vertex array to buffer for OpenGL
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Explain to OpenGL how the vertex data is structured
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void GraphicsSystem::create_square_VAO()
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

    glGenVertexArrays(1, &square_VAO);
    glBindVertexArray(square_VAO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Copy vertex array to buffer for OpenGL
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Explain to OpenGL how the vertex data is structured
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void GraphicsSystem::draw_square()
{
    glBindVertexArray(square_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void GraphicsSystem::draw_hexagon()
{
    glBindVertexArray(hexagon_VAO);
    glDrawElements(GL_TRIANGLE_FAN, 8, GL_UNSIGNED_INT, 0);
}

void GraphicsSystem::process_component(const GraphicsComponent &graphics_component) {
    switch (graphics_component.shape) {
        case GraphicsComponent::Shapes::Square :
            draw_square();
            break;
        case GraphicsComponent::Shapes::Hexagon :
            draw_hexagon();
            break;
    }
}
