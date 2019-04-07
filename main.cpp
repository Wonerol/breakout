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
#include <algorithm>

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
    const float SCREEN_HEIGHT = 1000;

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
    paddle.translate(0, -3.0f);
    paddle.scale(2.0f, 0.75f);
    paddle.set_colour(0.2f, 0.4f, 0.7f, 1.0f);

    Ball ball;
    ball.translate(0, -2.0f);
    ball.scale(0.75f, 0.75f);
    ball.set_colour(1.0f, 0.2f, 0.2f, 1.0f);

    Brick ceiling;
    ceiling.translate(0, 4.0f);
    ceiling.scale(7.0f, 1.0f);
    ceiling.set_colour(0.3f, 0.3f, 0.3f, 1.0f);

    Brick left_wall;
    left_wall.translate(-3.0f, 0.0f);
    left_wall.scale(1.0f, 9.0f);
    left_wall.set_colour(0.3f, 0.3f, 0.3f, 1.0f);

    Brick right_wall;
    right_wall.translate(3.0f, 0.0f);
    right_wall.scale(1.0f, 9.0f);
    right_wall.set_colour(0.3f, 0.3f, 0.3f, 1.0f);

    std::vector<GameObject*> game_objects;
    game_objects.push_back(&paddle);
    game_objects.push_back(&ball);

    const int NUM_ROWS = 4;
    const int NUM_COLUMNS = 5;

    Brick bricks[NUM_ROWS][NUM_COLUMNS];

    for (int i = 0; i < NUM_ROWS; i++) {
        float row_colours[][4] = {
            { 1.0f, 0.3f, 1.0f, 1.0f },
            { 1.0f, 1.0f, 0.3f, 1.0f },
            { 0.8f, 0.3f, 1.0f, 1.0f },
            { 0.8f, 1.0f, 0.8f, 1.0f },
        };

        for (int j = 0; j < NUM_COLUMNS; j++) {
            bricks[i][j].name = "destructible_brick";
            bricks[i][j].translate(-2.0f + j, i);
            bricks[i][j].set_colour(row_colours[i]);

            game_objects.push_back(&bricks[i][j]);
        }
    }

    game_objects.push_back(&ceiling);
    game_objects.push_back(&left_wall);
    game_objects.push_back(&right_wall);

    bool wireframe_button_pressed = false;
    bool wireframe_mode = false;

    while (!glfwWindowShouldClose(window))
    {
        start_time = glfwGetTime();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            wireframe_button_pressed = true;
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) {
            if (wireframe_button_pressed) {
                wireframe_button_pressed = false;
                wireframe_mode = !wireframe_mode;
            }

            if (wireframe_mode) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
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
                if (game_object_a != game_object_b && game_object_a->active && game_object_b->active) {
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
                        if (game_object_a->name == "destructible_brick" && game_object_b->name == "Ball") {
                            game_object_a->active = false;
                        }
                        if (game_object_b->name == "destructible_brick" && game_object_a->name == "Ball") {
                            game_object_b->active = false;
                        }
                    }
                }
            }
        }

        // remove any destroyed blocks
        game_objects.erase(
            std::remove_if(
                game_objects.begin(),
                game_objects.end(),
                [](GameObject* game_object) -> bool {
                    return !game_object->active;
                }
            ),
            game_objects.end()
        );

        glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();

        unsigned int transform_location = glGetUniformLocation(shader.ID, "view");
        glUniformMatrix4fv(transform_location, 1, GL_FALSE, glm::value_ptr(view_matrix));

        transform_location = glGetUniformLocation(shader.ID, "projection");
        glUniformMatrix4fv(transform_location, 1, GL_FALSE, glm::value_ptr(projection_matrix));

        for (GameObject* game_object : game_objects) {
            unsigned int colour_location = glGetUniformLocation(shader.ID, "colour");
            glUniform4fv(colour_location, 1, game_object->colour);

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
