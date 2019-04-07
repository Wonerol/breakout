#ifndef BALL_H
#define BALL_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Ball {
    public:
        glm::mat4 transformation_matrix;

        Ball();

        void translate(float x, float y);
        void draw();

    private:
        unsigned int VAO;
};

#endif
