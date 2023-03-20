#include <corgi/plot/plot.h>

#include <iostream>

int main(int argc, char** argv)
{
    std::vector<corgi::plot::point> points {{0, 0}, {1, 1}, {2, 2}, {3, 4}};
    corgi::plot::init_plot();
    corgi::plot::add_point_group({}, {0, 255, 255});
    // corgi::plot::rendering_mode(corgi::plot::rendering_mode::line);

    corgi::plot::add_repeat_callback(
        500, -1,
        [&]() -> void
        {
            if(!points.empty())
            {
                corgi::plot::point_groups().back().add_point(points.front());
                points.erase(points.begin());
            }
        });
    corgi::plot::show();
    corgi::plot::quit();
    return 0;
}