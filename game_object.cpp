#include "game_object.h"
#include <iostream>

GameObject::GameObject() {
    active = true;
    name = "I am but a humble GameObject";
    transformation_matrix = glm::mat4(1.0f);
}

void GameObject::draw()
{
    std::cout << "You shouldn't be calling me" << std::endl;
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
