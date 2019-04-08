#ifndef SCENE_H
#define SCENE_H

#include "paddle.h"
#include "ball.h"
#include <vector>

class Scene {
    public:
        Scene();
        ~Scene();

        std::vector<GameObject*> game_objects;

        Paddle* get_paddle();
        Ball* get_ball();
        void reset();
        void remove_inactive();

    private:
        Paddle* paddle;
        Ball* ball;
        void set_stage();
        void free_memory();
};

#endif
