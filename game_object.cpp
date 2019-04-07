#include "game_object.h"
#include <iostream>

GameObject::GameObject() {
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
    AABB aabb{ centre.x - 0.5, centre.x + 0.5, centre.y - 0.5, centre.y + 0.5 };

    return aabb;
}
