#ifndef BALL_H
#define BALL_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "game_object.h"

class Ball : public GameObject {
    public:
        Ball();

        void translate(float x, float y);
        void draw();

    private:
        unsigned int VAO;
};

#endif
