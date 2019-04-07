#ifndef BRICK_H
#define BRICK_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "game_object.h"

class Brick : public GameObject {
    public:
        Brick();

        void draw();
};

#endif
