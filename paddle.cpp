#include "paddle.h"
#include <glad/glad.h>

Paddle::Paddle()
{
    name = "Paddle";
}

void Paddle::draw()
{
    square.draw();
}
