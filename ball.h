#ifndef BALL_H
#define BALL_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "game_object.h"

class Ball : public GameObject {
    public:
        float velocity[2];
        float speed;

        Ball();

        void draw();
        void bounce(char axis);
};

#endif
