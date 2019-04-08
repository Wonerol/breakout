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

        void queue_bounce(char axis);
        void update();

    private:
        bool bounce_queued;
        char bounce_axis;

        void bounce(char axis);
};

#endif
