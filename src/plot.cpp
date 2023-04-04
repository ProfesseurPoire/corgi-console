#include "corgi/plot/plot.h"

#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <corgi/chrono/timer.h>

#include <cmath>
#include <iostream>
#include <vector>
namespace corgi::plot
{
namespace
{
TTF_Font*                font {nullptr};
enum rendering_mode      rendering_mode_;
SDL_Surface*             window_surface;
SDL_Window*              window;
SDL_Renderer*            renderer;
std::vector<point_group> groups;
color                    axis_color_ {20, 20, 40, 255};

int point_size_ = 6;

}    // namespace

int height {480};
int width {480};

int margin = 30;

float x_axis_min {-5.0F};
float x_axis_max {6.0F};

float y_axis_min {-5.0F};
float y_axis_max {6.0F};

float x_width {0.0f};
float y_width {0.0f};

// Probably would be nice to be able to destroy the timers at one point
corgi::chrono::timer t;

void add_point_group(std::vector<point> points, color c)
{
    groups.emplace_back(points, c);
}

std::vector<SDL_Texture*> textures;
std::vector<SDL_Surface*> surfaces;

void draw_text(const std::string& text, int x, int y)
{
    SDL_Color color {0, 0, 0, 255};

    auto surface = TTF_RenderText_Blended(font, text.c_str(), color);
    auto texture = SDL_CreateTextureFromSurface(renderer, surface);

    textures.push_back(texture);
    surfaces.push_back(surface);

    int texW = 0;
    int texH = 0;

    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    SDL_Rect dstrect = {x - (texW / 2.0f), y - (texH / 2.0f), texW, texH};
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);
}

void add_callback(int time, std::function<void()> callback)
{
    t.set(time, callback, corgi::chrono::timer::mode::async);
    t.start();
}

void add_repeat_callback(int time, int repeat, std::function<void()> callback)
{
    t.set(time, callback, corgi::chrono::timer::mode::async);
    t.repeat(repeat);
    t.start();
}

int plot_width()
{
    return width - 2 * margin;
}

int plot_height()
{
    return height - 2 * margin;
}

static point get_point(point p, float unit, float xoffset, float yoffset)
{
    p.x = xoffset + (p.x - x_axis_min) * unit;
    p.y = yoffset + (y_width * unit) - (p.y - y_axis_min) * unit;
    return p;
}

void show()
{
    font = TTF_OpenFont("./resources/Roboto-Regular.ttf", 12);

    x_width = x_axis_max - x_axis_min;
    y_width = y_axis_max - y_axis_min;

    float window_ratio = (float)plot_width() / (float)(height - 2 * margin);
    float plot_ratio   = (float)x_width / (float)y_width;

    float xoffset = 0;
    float yoffset = 0;

    int   size = plot_width();
    float unit = (float)(plot_width()) / (float)x_width;
    float width_size;
    float height_size;

    if(window_ratio > plot_ratio)
    {
        unit        = (float)plot_height() / (float)y_width;
        xoffset     = (plot_width() - (x_width * unit)) / 2.0f;
        yoffset     = margin;
        width_size  = plot_width();
        height_size = (y_axis_max - y_axis_min) * unit;
    }
    else
    {
        size        = width;
        unit        = (float)(plot_width()) / (float)x_width;
        yoffset     = margin + (plot_height() - (y_width * unit)) / 2.0f;
        xoffset     = margin;
        width_size  = (x_axis_max - x_axis_min) * unit;
        height_size = plot_height();
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

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect background =
            SDL_Rect(xoffset, yoffset, x_width * unit, y_width * unit);

        SDL_RenderFillRect(renderer, &background);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderDrawRect(renderer, &background);

        SDL_SetRenderDrawColor(renderer, axis_color_.r, axis_color_.g,
                               axis_color_.b, axis_color_.a);

        point pxaxis {y_axis_min, 0};
        point pyaxis {0, x_axis_max};

        pxaxis = get_point(pxaxis, unit, xoffset, yoffset);
        pyaxis = get_point(pyaxis, unit, xoffset, yoffset);

        SDL_Rect x_axis_line(pxaxis.x, pxaxis.y - 1, x_width * unit, 2);
        SDL_Rect y_axis_line(pyaxis.x - 1, pyaxis.y, 2, y_width * unit);
        SDL_RenderDrawRect(renderer, &x_axis_line);
        SDL_RenderDrawRect(renderer, &y_axis_line);

        // Draw grid

        // Draw grid x
        for(float i = std::ceilf(x_axis_min); i < x_axis_max; i++)
        {
            if(i == x_axis_min)
                continue;

            point pgrid {i, 0};
            point pgridmin {i, y_axis_min};
            point pgridmax {i, y_axis_max};

            pgrid    = get_point(pgrid, unit, xoffset, yoffset);
            pgridmin = get_point(pgridmin, unit, xoffset, yoffset);
            pgridmax = get_point(pgridmax, unit, xoffset, yoffset);

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawLine(renderer, pgrid.x, pgrid.y - 5, pgrid.x,
                               pgrid.y + 5);

            SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);

            SDL_RenderDrawLine(renderer, pgridmin.x, pgridmin.y, pgridmax.x,
                               pgridmax.y);
            if(i != 0.0f)
                draw_text(std::format("{:.1f}", i), pgrid.x, pgrid.y + 15);
        }

        for(float i = std::ceilf(y_axis_min); i < y_axis_max; i++)
        {
            if(i == y_axis_min)
                continue;
            point pgrid {0, i};
            point pgridmin {x_axis_min, i};
            point pgridmax {x_axis_max, i};

            pgrid    = get_point(pgrid, unit, xoffset, yoffset);
            pgridmin = get_point(pgridmin, unit, xoffset, yoffset);
            pgridmax = get_point(pgridmax, unit, xoffset, yoffset);
            pgrid    = get_point(pgrid, unit, xoffset, yoffset);

            SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
            SDL_RenderDrawLine(renderer, pgridmin.x, pgridmin.y, pgridmax.x,
                               pgridmax.y);

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

            SDL_RenderDrawLine(renderer, pgrid.x - 5, pgrid.y, pgrid.x + 5,
                               pgrid.y);

            if(i != 0.0f)
                draw_text(std::format("{:.1f}", i), pgrid.x + 20, pgrid.y);
        }

        // Draw group points
        for(auto group : groups)
        {
            SDL_SetRenderDrawColor(renderer, group.color.r, group.color.g,
                                   group.color.b, group.color.a);

            switch(rendering_mode_)
            {
                case rendering_mode::line:

                    if(group.points.empty())
                        break;
                    for(int i = 0; i < group.points.size() - 1; i++)
                    {
                        auto p1 =
                            get_point(group.points[i], unit, xoffset, yoffset);

                        auto p2 = get_point(group.points[i + 1], unit, xoffset,
                                            yoffset);

                        SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);
                    }
                    break;
                case rendering_mode::point:
                    for(auto point : group.points)
                    {
                        auto pp = get_point(point, unit, xoffset, yoffset);
                        int  m  = (float)point_size_ / 2.0F;

                        SDL_Rect p(pp.x - m, pp.y - m, point_size_,
                                   point_size_);
                        SDL_RenderFillRect(renderer, &p);
                    }
                    break;
            }
        }

        auto pixels =
            new uint32_t[width * height];    // width x height x
                                             // SDL_PIXELFORMAT_RGBA8888

        // This whole thing save the current image
        static bool save_once = true;
        if(save_once)
        {
            auto surface = SDL_CreateRGBSurface(SDL_WINDOW_SHOWN, width, height,
                                                32, 0xff000000, 0x00ff0000,
                                                0x0000ff00, 0x000000ff);

            SDL_RenderReadPixels(renderer, nullptr, SDL_PIXELFORMAT_RGBA32,
                                 pixels, 4 * width);

            auto surfacePixels = (Uint32*)surface->pixels;

            for(int i = 0; i < width * height; ++i)
                surfacePixels[i] = pixels[i];

            SDL_LockSurface(surface);

            SDL_SaveBMP(surface,
                        ("screen1_" + std::to_string(1) + ".bmp")
                            .c_str());    // Output buffer

            SDL_UnlockSurface(surface);
        }

        SDL_RenderPresent(renderer);

        for(auto& t : textures)
            SDL_DestroyTexture(t);
        textures.clear();

        for(auto& s : surfaces)
            SDL_FreeSurface(s);
        surfaces.clear();
    }
    SDL_DestroyWindow(window);
    SDL_QUIT;
    TTF_Quit();
}

void clear()
{
    groups.clear();
}

std::vector<point_group>& point_groups()
{
    return groups;
}

void axis_color(color color)
{
    axis_color_ = color;
}

void rendering_mode(enum rendering_mode mode)
{
    rendering_mode_ = mode;
}

void init_plot()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Failed to initialize the SDL2 library\n";
        return;
    }
    TTF_Init();
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

void quit()
{
    t.stop();
}

void point_size(float s)
{
    point_size_ = s;
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
