#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "glm/gtc/type_ptr.hpp"

class GameObject {
    public:
        GameObject();

        virtual void draw();
        glm::mat4 transformation_matrix;
};

#endif
