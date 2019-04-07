#ifndef PADDLE_H
#define PADDLE_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "AABB.h"
#include "game_object.h"

class Paddle : public GameObject {
    public:
        float PADDLE_SPEED = 1.0f;

        Paddle();

        void translate(float x);
        void draw();
        AABB get_AABB();

    private:
        unsigned int VAO;
};

#endif
