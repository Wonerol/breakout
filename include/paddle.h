#ifndef PADDLE_H
#define PADDLE_H

#include "game_object.h"

class Paddle : public GameObject {
    public:
        float speed = 1.0f;

        Paddle();
};

#endif
