#include "game_object.h"
#include <iostream>

GameObject::GameObject() {
    transformation_matrix = glm::mat4(1.0f);
}

void GameObject::draw()
{
    std::cout << "You shouldn't be calling me" << std::endl;
}
