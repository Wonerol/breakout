#include <string>
#include <unistd.h>
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
#include "scene.h"
#include "graphics_system.h"

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

int main(int argc, char* argv[]) {
    std::cout << "Welcome to Breakout!" << std::endl;

    int buffer_size= 2048;
    char buffer[buffer_size];

    readlink("/proc/self/exe", buffer, buffer_size);
    std::string working_directory(buffer);
    working_directory = working_directory.substr(0, working_directory.find_last_of('/') + 1);

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

    std::string shader_dir = working_directory + "shaders/";
    Shader shader((shader_dir + "vertex_shader.vs").c_str(), (shader_dir + "fragment_shader.fs").c_str());

    float start_time;
    float delta_time = 1.0f / 30;

    glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);

    glm::mat4 view_matrix = glm::mat4(1.0f);
    view_matrix = glm::translate(view_matrix, glm::vec3(0.0f, 0.0f, -10.0f));

    Scene scene;

    Paddle* paddle = scene.get_paddle();
    Ball* ball = scene.get_ball();

    bool wireframe_button_pressed = false;
    bool wireframe_mode = false;

    bool collider_button_pressed = false;
    bool collider_mode = false;

    bool reset_button_pressed = false;

    GraphicsSystem graphics_system;

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

                if (wireframe_mode) {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                } else {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
            }
        }

        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
            reset_button_pressed = true;
        }

        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE) {
            if (reset_button_pressed) {
                reset_button_pressed = false;
                scene.reset();
                paddle = scene.get_paddle();
                ball = scene.get_ball();
            }
        }

        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
            collider_button_pressed = true;
        }

        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE) {
            if (collider_button_pressed) {
                collider_button_pressed = false;
                collider_mode = !collider_mode;
            }
        }

        ball->update();

        float x_translation = 0;
        int input_direction = 0;

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            input_direction = -1.0f;
        } else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            input_direction = 1.0f;
        }

        x_translation = paddle->speed * input_direction * delta_time;

        paddle->translate(x_translation, 0);

        x_translation = ball->speed * ball->velocity[0] * delta_time;
        float y_translation = ball->speed * ball->velocity[1] * delta_time;
        ball->translate(x_translation, y_translation);

        // O(n^2) Slow but easy
        for (GameObject* game_object_a : scene.game_objects) {
            for (GameObject* game_object_b : scene.game_objects) {
                if (game_object_a != game_object_b && game_object_a->active && game_object_b->active) {
                    AABB a_AABB = game_object_a->get_AABB();
                    AABB b_AABB = game_object_b->get_AABB();

                    CollisionInfo collision_info = AABB_intersection(a_AABB, b_AABB);
                    if (collision_info.collided) {
                        if ((game_object_a->name == "Paddle" || game_object_b->name == "Paddle") &&
                                (game_object_a->name == "Wall" || game_object_b->name == "Wall")) {
                            if (input_direction > 0) {
                                paddle->translate(-collision_info.x_penetration, 0);
                            } else {
                                paddle->translate(collision_info.x_penetration, 0);
                            }
                        }

                        if (game_object_a->name == "Ball" || game_object_b->name == "Ball") {
                            if (game_object_a->name == "pit" || game_object_b->name == "pit") {
                                // You lose D: now play again...
                                scene.reset();
                                paddle = scene.get_paddle();
                                ball = scene.get_ball();
                            }

                            if (collision_info.x_penetration > collision_info.y_penetration) {
                                if (ball->velocity[1] > 0) {
                                    ball->translate(0, -collision_info.y_penetration);
                                } else {
                                    ball->translate(0, collision_info.y_penetration);
                                }
                                ball->queue_bounce('y');
                            } else {
                                if (ball->velocity[0] > 0) {
                                    ball->translate(-collision_info.x_penetration, 0);
                                } else {
                                    ball->translate(collision_info.x_penetration, 0);
                                }
                                ball->queue_bounce('x');
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

        scene.remove_inactive();

        if (scene.are_all_bricks_destroyed()) {
            // You win! now play again...
            scene.reset();
            paddle = scene.get_paddle();
            ball = scene.get_ball();
        }

        glClearColor(0.2f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();

        unsigned int transform_location = glGetUniformLocation(shader.ID, "view");
        glUniformMatrix4fv(transform_location, 1, GL_FALSE, glm::value_ptr(view_matrix));

        transform_location = glGetUniformLocation(shader.ID, "projection");
        glUniformMatrix4fv(transform_location, 1, GL_FALSE, glm::value_ptr(projection_matrix));

        for (GameObject* game_object : scene.game_objects) {
            unsigned int colour_location = glGetUniformLocation(shader.ID, "colour");
            glUniform4fv(colour_location, 1, game_object->colour);

            transform_location = glGetUniformLocation(shader.ID, "model");
            glUniformMatrix4fv(transform_location, 1, GL_FALSE, glm::value_ptr(game_object->transformation_matrix));

            if (game_object->graphics_component != NULL) {
                graphics_system.process_component(*game_object->graphics_component);
            }

            if (collider_mode) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

                graphics_system.draw_square();

                if (!wireframe_mode) {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
            }
        }

        glfwPollEvents();
        glfwSwapBuffers(window);

        delta_time = glfwGetTime() - start_time;
    }

    glfwTerminate();

    std::cout << "Thank you for playing Breakout!" << std::endl;

    return 0;
}
