#ifndef SCENE_H
#define SCENE_H

#include "paddle.h"
#include "ball.h"
#include <vector>

class Scene {
    public:
        Scene();

        Paddle* get_paddle();
        Ball* get_ball();
        std::vector<GameObject*> game_objects;

    private:
        Paddle paddle;
        Ball ball;
};

#endif
