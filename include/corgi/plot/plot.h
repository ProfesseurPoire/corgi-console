#pragma once
#include <SDL2/SDL.h>

namespace corgi::plot
{
void init_plot();

void set_x_axis(float min, float max);
void set_y_axis(float min, float max);

}    // namespace corgi::plot