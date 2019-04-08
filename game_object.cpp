#include "game_object.h"
#include <glad/glad.h>
#include <iostream>

GameObject::GameObject() {
    active = true;
    name = "I am but a humble GameObject";
    transformation_matrix = glm::mat4(1.0f);
    graphics_component = NULL;

    colour[0] = 1.0f;
    colour[1] = 1.0f;
    colour[2] = 1.0f;
    colour[3] = 1.0f;
    colour[4] = 1.0f;
}

GameObject::~GameObject() {
    if (graphics_component != NULL) {
        delete graphics_component;
    }
}

AABB GameObject::get_AABB()
{
    glm::vec4 centre = transformation_matrix[3];
    glm::vec3 scale(transformation_matrix[0].x, transformation_matrix[1].y, transformation_matrix[2].z);

    float x_extent = (0.5 * scale.x);
    float y_extent = (0.5 * scale.y);

    AABB aabb{
        centre.x - x_extent,
        centre.x + x_extent,
        centre.y - y_extent,
        centre.y + y_extent
    };

    return aabb;
}

void GameObject::translate(float x, float y)
{
    transformation_matrix = glm::translate(transformation_matrix, glm::vec3(x, y, 0.0f));
}

void GameObject::scale(float x, float y)
{
    transformation_matrix = glm::scale(transformation_matrix, glm::vec3(x, y, 0.0f));
}

void GameObject::set_colour(float r, float g, float b, float a)
{
    colour[0] = r;
    colour[1] = g;
    colour[2] = b;
    colour[3] = a;
}

void GameObject::set_colour(float colour[])
{
    set_colour(colour[0], colour[1], colour[2], colour[3]);
}
