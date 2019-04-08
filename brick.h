#ifndef BRICK_H
#define BRICK_H

#include "game_object.h"
#include "square.h"

class Brick : public GameObject {
    public:
        Brick();

        void draw();

    private:
        Square square;
};

#endif
