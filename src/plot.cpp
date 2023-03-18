#include "corgi/plot/plot.h"

#include <iostream>
#include <vector>

namespace corgi::plot
{
namespace
{
SDL_Surface*             window_surface;
SDL_Window*              window;
SDL_Renderer*            renderer;
std::vector<point_group> groups;
}    // namespace

int height {480};
int width {680};

float x_axis_min {-1.0F};
float x_axis_max {2.0F};

float y_axis_min {-1.0F};
float y_axis_max {5.1F};

float x_width {0.0f};
float y_width {0.0f};

void add_point_group(std::vector<point> points, color c)
{
    groups.emplace_back(points, c);
}

void show()
{
    x_width = x_axis_max - x_axis_min;
    y_width = y_axis_max - y_axis_min;

    float window_ratio = (float)width / (float)height;
    float plot_ratio   = (float)x_width / (float)y_width;

    float xoffset = 0;
    float yoffset = 0;

    int   size = width;
    float unit = (float)width / (float)x_width;
    float width_size;
    float height_size;

    if(window_ratio > plot_ratio)
    {
        size    = height;
        unit    = (float)height / (float)y_width;
        xoffset = (width - (x_width * unit)) / 2.0f;

        width_size  = width;
        height_size = (y_axis_max - y_axis_min) * unit;
    }
    else
    {
        size    = width;
        unit    = (float)width / (float)x_width;
        yoffset = (height - (y_width * unit)) / 2.0f;

        width_size  = (x_axis_max - x_axis_min) * unit;
        height_size = height;
    }

    std::cout << unit << std::endl;
    std::cout << "xoffset" << xoffset << std::endl;
    std::cout << "yoffset" << yoffset << std::endl;

    bool keep_window_open = true;

    while(keep_window_open)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e) > 0)
        {
            switch(e.type)
            {
                case SDL_QUIT:
                    keep_window_open = false;
                    break;
            }
        }

        float x_axis_y = -y_axis_min / (y_axis_max - y_axis_min);

        float y_axis_x = -x_axis_min / (x_axis_max - x_axis_min);

        SDL_SetRenderDrawColor(renderer, 110, 110, 130, 230);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 9, 9, 255);

        SDL_Rect x_axis_line(
            xoffset, height - (-yoffset + x_axis_y * y_width * unit - 1),
            x_width * unit, 1);

        SDL_Rect y_axis_line(xoffset + y_axis_x * x_width * unit - 1, 0, 1,
                             y_width * unit);

        SDL_RenderDrawRect(renderer, &x_axis_line);
        SDL_RenderDrawRect(renderer, &y_axis_line);
        for(auto group : groups)
        {
            SDL_SetRenderDrawColor(renderer, group.color.r, group.color.g,
                                   group.color.b, group.color.a);

            for(auto point : group.points)
            {
                int point_size = 10;

                SDL_Rect p(point.x, point.y, point_size, point_size);

                p.x =
                    xoffset + (point.x - x_axis_min) * unit - point_size / 2.0f;
                p.y = height - (yoffset + (point.y - y_axis_min) * unit) -
                      point_size / 2.0f;
                SDL_RenderFillRect(renderer, &p);
            }
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_QUIT;
}

void init_plot()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Failed to initialize the SDL2 library\n";
        return;
    }

    window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, width, height, 0);

    if(!window)
    {
        std::cout << "Failed to create window\n";
        return;
    }

    renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_GetWindowSurface(window);

    if(!window_surface)
    {
        std::cout << "Failed to get the surface from the window\n";
        return;
    }
}

void set_x_axis(float min, float max)
{
    x_axis_min = min;
    x_axis_max = max;
}

void set_y_axis(float min, float max)
{
    y_axis_min = min;
    y_axis_max = max;
}
}    // namespace corgi::plot
