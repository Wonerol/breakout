#ifndef PADDLE_H
#define PADDLE_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Paddle {
    public:
        float PADDLE_SPEED = 1.0f;
        unsigned int VAO;
        glm::mat4 transformation_matrix;

        Paddle();

        void translate(float x);
        // location of transform in vertex shader
        void draw(unsigned int transform_location);
};

#endif
