#include "scene.h"
#include "brick.h"
#include <algorithm>

Scene::Scene()
{
    set_stage();
}

Scene::~Scene()
{
    free_memory();
}

void Scene::set_stage()
{
    paddle = new Paddle();
    paddle->translate(0, -3.0f);
    paddle->scale(2.0f, 0.75f);
    paddle->set_colour(0.2f, 0.4f, 0.7f, 1.0f);
    paddle->graphics_component = new GraphicsComponent(GraphicsComponent::Shapes::Square);
    game_objects.push_back(paddle);

    ball = new Ball();
    ball->translate(0, -2.0f);
    ball->scale(0.75f, 0.75f);
    ball->set_colour(1.0f, 0.2f, 0.2f, 1.0f);
    ball->graphics_component = new GraphicsComponent(GraphicsComponent::Shapes::Hexagon);
    game_objects.push_back(ball);

    Brick* ceiling = new Brick();
    ceiling->name = "Wall";
    ceiling->translate(0, 4.0f);
    ceiling->scale(7.0f, 1.0f);
    ceiling->set_colour(0.3f, 0.3f, 0.3f, 1.0f);
    ceiling->graphics_component = new GraphicsComponent(GraphicsComponent::Shapes::Square);
    game_objects.push_back(ceiling);

    Brick* left_wall = new Brick();
    left_wall->name = "Wall";
    left_wall->translate(-3.0f, 0.0f);
    left_wall->scale(1.0f, 9.0f);
    left_wall->set_colour(0.3f, 0.3f, 0.3f, 1.0f);
    left_wall->graphics_component = new GraphicsComponent(GraphicsComponent::Shapes::Square);
    game_objects.push_back(left_wall);

    Brick* right_wall = new Brick();
    right_wall->name = "Wall";
    right_wall->translate(3.0f, 0.0f);
    right_wall->scale(1.0f, 9.0f);
    right_wall->set_colour(0.3f, 0.3f, 0.3f, 1.0f);
    right_wall->graphics_component = new GraphicsComponent(GraphicsComponent::Shapes::Square);
    game_objects.push_back(right_wall);

    GameObject* pit = new GameObject();
    pit->name = "pit";
    pit->translate(0, -4.0f);
    pit->scale(6.0f, 2.0f);
    game_objects.push_back(pit);

    const int NUM_ROWS = 4;
    const int NUM_COLUMNS = 5;

    for (int i = 0; i < NUM_ROWS; i++) {
        float row_colours[][4] = {
            { 1.0f, 0.3f, 1.0f, 1.0f },
            { 1.0f, 1.0f, 0.3f, 1.0f },
            { 0.8f, 0.3f, 1.0f, 1.0f },
            { 0.8f, 1.0f, 0.8f, 1.0f },
        };

        for (int j = 0; j < NUM_COLUMNS; j++) {
            Brick* brick = new Brick();
            brick->name = "destructible_brick";
            brick->translate(-2.0f + j, i);
            brick->set_colour(row_colours[i]);
            brick->graphics_component = new GraphicsComponent(GraphicsComponent::Shapes::Square);

            game_objects.push_back(brick);
        }
    }
}

void Scene::free_memory()
{
    for (GameObject* game_object : game_objects) {
        delete game_object;
    }

    game_objects.clear();

    paddle = NULL;
    ball = NULL;
}

void Scene::reset()
{
    free_memory();
    set_stage();
}

Paddle* Scene::get_paddle()
{
    return paddle;
}

Ball* Scene::get_ball()
{
    return ball;
}

void Scene::remove_inactive()
{
    // remove any destroyed blocks
    game_objects.erase(
        std::remove_if(
            game_objects.begin(),
            game_objects.end(),
            [](GameObject* game_object) -> bool {
                if (!game_object->active) {
                    delete game_object;
                }

                return !game_object->active;
            }
        ),
        game_objects.end()
    );
}

bool Scene::are_all_bricks_destroyed()
{
    for (GameObject* game_object : game_objects) {
        if (game_object->name == "destructible_brick") {
            return false;
        }
    }

    return true;
}
