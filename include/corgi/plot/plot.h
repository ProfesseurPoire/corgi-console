#pragma once
#include <functional>
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

    void add_point(const point& p) { points.push_back(p); }
};

enum class rendering_mode
{
    point,
    line
};

void axis_color(color color);
void rendering_mode(rendering_mode mode);
void init_plot();
void quit();
void point_size(float s);
void add_callback(int time, std::function<void()> callback);
void add_repeat_callback(int time, int repeat, std::function<void()> callback);
void show();
void clear();
std::vector<point_group>& point_groups();

void set_x_axis(float min, float max);
void set_y_axis(float min, float max);

void add_point_group(std::vector<point> points, color c);

}    // namespace corgi::plot