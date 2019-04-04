#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

int main() {
    std::cout << "Welcome to Breakout!" << std::endl;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Breakout", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);

    // Define a triangle
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    // Copy vertex array to buffer for OpenGL
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Explain to OpenGL how the vertex data is structured
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    Shader shader("vertex_shader.vs", "fragment_shader.fs");

    glm::mat4 transformation_matrix = glm::mat4(1.0f);

    float start_time;
    float delta_time = 1.0f / 30;

    float PADDLE_SPEED = 1.0f;

    while (!glfwWindowShouldClose(window))
    {
        start_time = glfwGetTime();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        float x_translation = 0;
        int input_direction = 0;

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            input_direction = -1.0f;
        } else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            input_direction = 1.0f;
        }

        x_translation = PADDLE_SPEED * input_direction * delta_time;

        transformation_matrix = glm::translate(transformation_matrix, glm::vec3(x_translation, 0.0f, 0.0f));

        glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();

        unsigned int transform_location = glGetUniformLocation(shader.ID, "transform");
        glUniformMatrix4fv(transform_location, 1, GL_FALSE, glm::value_ptr(transformation_matrix));


        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwPollEvents();
        glfwSwapBuffers(window);

        delta_time = glfwGetTime() - start_time;
    }

    glfwTerminate();

    std::cout << "Thank you for playing Breakout!" << std::endl;

    return 0;
}
