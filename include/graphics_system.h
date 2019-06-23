#ifndef GRAPHICS_SYSTEM_H
#define GRAPHICS_SYSTEM_H

#include "graphics_component.h"

class GraphicsSystem
{
    public:
        GraphicsSystem();
        void process_component(const GraphicsComponent &graphics_component);
        void draw_square();

    private:
        void draw_hexagon();
        void create_square_VAO();
        void create_hexagon_VAO();

        unsigned int square_VAO;
        unsigned int hexagon_VAO;
};

#endif
