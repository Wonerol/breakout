#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "glm/gtc/type_ptr.hpp"
#include "AABB.h"
#include <string>

class GameObject {
    public:
        glm::mat4 transformation_matrix;
        std::string name;
        bool active;

        GameObject();

        void translate(float x, float y);
        virtual void draw();
        AABB get_AABB();

    protected:
        unsigned int VAO;
};

#endif
