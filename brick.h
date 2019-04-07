#ifndef BRICK_H
#define BRICK_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Brick {
    public:
        glm::mat4 transformation_matrix;

        Brick();

        void translate(float x);
        void draw();

    private:
        unsigned int VAO;
};

#endif
