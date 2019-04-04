#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "paddle.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

int main() {
    std::cout << "Welcome to Breakout!" << std::endl;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const float SCREEN_WIDTH = 800;
    const float SCREEN_HEIGHT = 600;

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout", NULL, NULL);
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

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    Shader shader("vertex_shader.vs", "fragment_shader.fs");

    float start_time;
    float delta_time = 1.0f / 30;

    glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);

    glm::mat4 view_matrix = glm::mat4(1.0f);
    view_matrix = glm::translate(view_matrix, glm::vec3(0.0f, 0.0f, -3.0f));

    Paddle paddle;

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

        x_translation = paddle.PADDLE_SPEED * input_direction * delta_time;

        paddle.translate(x_translation);

        glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();

        unsigned int transform_location = glGetUniformLocation(shader.ID, "model");
        glUniformMatrix4fv(transform_location, 1, GL_FALSE, glm::value_ptr(paddle.transformation_matrix));

        transform_location = glGetUniformLocation(shader.ID, "view");
        glUniformMatrix4fv(transform_location, 1, GL_FALSE, glm::value_ptr(view_matrix));

        transform_location = glGetUniformLocation(shader.ID, "projection");
        glUniformMatrix4fv(transform_location, 1, GL_FALSE, glm::value_ptr(projection_matrix));

        paddle.draw();

        glfwPollEvents();
        glfwSwapBuffers(window);

        delta_time = glfwGetTime() - start_time;
    }

    glfwTerminate();

    std::cout << "Thank you for playing Breakout!" << std::endl;

    return 0;
}
