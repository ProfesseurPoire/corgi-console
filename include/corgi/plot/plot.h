#pragma once
#include <SDL2/SDL.h>

#include <vector>
namespace corgi::plot
{

struct point
{
    float x;
    float y;
};

struct color
{
    int r;
    int g;
    int b;
    int a;
};

struct point_group
{
    std::vector<point> points;
    color              color;
};

void init_plot();
void show();

void set_x_axis(float min, float max);
void set_y_axis(float min, float max);

void add_point_group(std::vector<point> points, color c);

}    // namespace corgi::plot