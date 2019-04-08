#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "glm/gtc/type_ptr.hpp"
#include "AABB.h"
#include "graphics_component.h"
#include <string>

class GameObject {
    public:
        glm::mat4 transformation_matrix;
        std::string name;
        bool active;
        float colour[4];

        GameObject();
        ~GameObject();

        void translate(float x, float y);
        void scale(float x, float y);
        AABB get_AABB();

        void set_colour(float r, float g, float b, float a);
        void set_colour(float colour[]);

        GraphicsComponent* graphics_component;
};

#endif
