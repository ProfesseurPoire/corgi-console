#include "corgi/plot/plot.h"

#include <iostream>
namespace corgi::plot
{

int height {480};
int width {680};

float x_axis_min {-1.0F};
float x_axis_max {2.0F};

float y_axis_min {-1.0F};
float y_axis_max {1.1F};

float x_width {0.0f};
float y_width {0.0f};

void init_plot()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Failed to initialize the SDL2 library\n";
        return;
    }

    SDL_Window* window =
        SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_CENTERED,
                         SDL_WINDOWPOS_CENTERED, width, height, 0);

    if(!window)
    {
        std::cout << "Failed to create window\n";
        return;
    }

    auto renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_Surface* window_surface = SDL_GetWindowSurface(window);

    if(!window_surface)
    {
        std::cout << "Failed to get the surface from the window\n";
        return;
    }

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
        xoffset = (width - x_width * unit) / 2.0f;

        width_size  = width;
        height_size = (y_axis_max - y_axis_min) * unit;
    }
    else
    {
        size    = width;
        unit    = (float)width / (float)x_width;
        yoffset = (height - y_width * unit) / 2.0f;

        width_size  = (x_axis_max - x_axis_min) * unit;
        height_size = height;
    }

    std::cout << unit << std::endl;
    std::cout << "xoffset" << xoffset << std::endl;
    std::cout << "yoffset" << yoffset << std::endl;

    SDL_UpdateWindowSurface(window);
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

        SDL_Rect x_axis_line(xoffset, -yoffset + x_axis_y * height_size - 1,
                             x_width * unit, 1);

        SDL_Rect y_axis_line(y_axis_x * width_size - 1, 0, 1, y_width * unit);

        SDL_RenderDrawRect(renderer, &x_axis_line);
        SDL_RenderDrawRect(renderer, &y_axis_line);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_QUIT;
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
