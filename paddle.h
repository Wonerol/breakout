#ifndef PADDLE_H
#define PADDLE_H

#include "game_object.h"
#include "square.h"

class Paddle : public GameObject {
    public:
        float speed = 1.0f;

        Paddle();

        void draw();

    private:
        Square square;
};

#endif
