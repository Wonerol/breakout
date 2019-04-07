#ifndef BALL_H
#define BALL_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "AABB.h"

class Ball {
    public:
        glm::mat4 transformation_matrix;

        Ball();

        void translate(float x, float y);
        void draw();
        AABB get_AABB();

    private:
        unsigned int VAO;
};

#endif
