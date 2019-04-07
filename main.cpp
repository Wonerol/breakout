#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "paddle.h"
#include "brick.h"
#include "ball.h"
#include "AABB.h"
#include "game_object.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

struct CollisionInfo {
    bool collided;
    float x_penetration;
    float y_penetration;
};

// Uses separating axis theorem to detect
// overlap of two Axis-Aligned Bounding Boxes
// Caveats:
// - Won't cover us (heh) if one fully overlaps the other
// - Isn't aware of tunnelling
CollisionInfo AABB_intersection(AABB a, AABB b) {
    CollisionInfo collision_info{ false };

    float y_penetration = 0;

    // y-penetration
    if (a.y_min <= b.y_max && a.y_min >= b.y_min) {
        y_penetration = b.y_max - a.y_min;
    } else if (a.y_max <= b.y_max && a.y_max >= b.y_min) {
        y_penetration = a.y_max - b.y_min;
    }

    float x_penetration = 0;

    if (a.x_min <= b.x_max && a.x_min >= b.x_min) {
        x_penetration = b.x_max - a.x_min;
    } else if (a.x_max <= b.x_max && a.x_max >= b.x_min) {
        x_penetration = a.x_max - b.x_min;
    }

    if (y_penetration != 0 && x_penetration != 0) {
        collision_info.collided = true;
        collision_info.x_penetration = x_penetration;
        collision_info.y_penetration = y_penetration;
    }

    return collision_info;
}

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
    view_matrix = glm::translate(view_matrix, glm::vec3(0.0f, 0.0f, -10.0f));

    Paddle paddle;
    Ball ball;

    paddle.translate(-2.0f, 0);

    // draw a ceiling and floor of bricks
    const int NUM_BRICKS = 20;
    Brick ceiling_bricks[NUM_BRICKS];
    for (int i = 0; i < NUM_BRICKS; i++) {
        ceiling_bricks[i].translate(-10.0f + i, 4.0f);
    }

    // draw a ceiling and floor of bricks
    const int NUM_FLOOR_BRICKS = 20;
    Brick floor_bricks[NUM_FLOOR_BRICKS];
    for (int i = 0; i < NUM_FLOOR_BRICKS; i++) {
        floor_bricks[i].translate(-10.0f + i, -4.0f);
    }

    const int NUM_LEFT_WALL_BRICKS = 10;
    Brick left_wall_bricks[NUM_LEFT_WALL_BRICKS];
    for (int i = 0; i < NUM_LEFT_WALL_BRICKS; i++) {
        left_wall_bricks[i].translate(-4.0f, 4.0f - i);
    }

    const int NUM_RIGHT_WALL_BRICKS = 10;
    Brick right_wall_bricks[NUM_RIGHT_WALL_BRICKS];
    for (int i = 0; i < NUM_RIGHT_WALL_BRICKS; i++) {
        right_wall_bricks[i].translate(4.0f, 4.0f - i);
    }

    std::vector<GameObject*> game_objects;
    game_objects.push_back(&paddle);
    game_objects.push_back(&ball);

    for (int i = 0; i < NUM_BRICKS; i ++) {
        game_objects.push_back(&ceiling_bricks[i]);
    }

    for (int i = 0; i < NUM_FLOOR_BRICKS; i ++) {
        game_objects.push_back(&floor_bricks[i]);
    }

    for (int i = 0; i < NUM_LEFT_WALL_BRICKS; i ++) {
        game_objects.push_back(&left_wall_bricks[i]);
    }

    for (int i = 0; i < NUM_RIGHT_WALL_BRICKS; i ++) {
        game_objects.push_back(&right_wall_bricks[i]);
    }

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

        paddle.translate(x_translation, 0);

        x_translation = ball.speed * ball.velocity[0] * delta_time;
        float y_translation = ball.speed * ball.velocity[1] * delta_time;
        ball.translate(x_translation, y_translation);

        // O(n^2) Slow but easy
        for (GameObject* game_object_a : game_objects) {
            for (GameObject* game_object_b : game_objects) {
                if (game_object_a != game_object_b) {
                    AABB a_AABB = game_object_a->get_AABB();
                    AABB b_AABB = game_object_b->get_AABB();

                    CollisionInfo collision_info = AABB_intersection(a_AABB, b_AABB);
                    if (collision_info.collided) {
                        if (game_object_a->name == "Ball" || game_object_b->name == "Ball") {
                            if (collision_info.x_penetration > collision_info.y_penetration) {
                                if (ball.velocity[1] > 0) {
                                    ball.translate(0, -collision_info.y_penetration);
                                } else {
                                    ball.translate(0, collision_info.y_penetration);
                                }
                                ball.bounce('y');

                            } else {
                                if (ball.velocity[0] > 0) {
                                    ball.translate(-collision_info.x_penetration, 0);
                                } else {
                                    ball.translate(collision_info.x_penetration, 0);
                                }
                                ball.bounce('x');
                            }
                        }
                    }
                }
            }
        }

        glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();

        unsigned int transform_location = glGetUniformLocation(shader.ID, "view");
        glUniformMatrix4fv(transform_location, 1, GL_FALSE, glm::value_ptr(view_matrix));

        transform_location = glGetUniformLocation(shader.ID, "projection");
        glUniformMatrix4fv(transform_location, 1, GL_FALSE, glm::value_ptr(projection_matrix));

        for (GameObject* game_object : game_objects) {
            transform_location = glGetUniformLocation(shader.ID, "model");
            glUniformMatrix4fv(transform_location, 1, GL_FALSE, glm::value_ptr(game_object->transformation_matrix));

            game_object->draw();
        }

        glfwPollEvents();
        glfwSwapBuffers(window);

        delta_time = glfwGetTime() - start_time;
    }

    glfwTerminate();

    std::cout << "Thank you for playing Breakout!" << std::endl;

    return 0;
}
