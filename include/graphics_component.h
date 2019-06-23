#ifndef GRAPHICS_COMPONENT_H
#define GRAPHICS_COMPONENT_H

class GraphicsComponent
{
    public:
        enum Shapes { Square, Hexagon };

        GraphicsComponent(Shapes argShape);
        Shapes shape;
};

#endif
