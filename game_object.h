#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "glm/gtc/type_ptr.hpp"
#include "AABB.h"
#include <string>

class GameObject {
    public:
        glm::mat4 transformation_matrix;
        std::string name;

        GameObject();

        virtual void draw();
        AABB get_AABB();
};

#endif
