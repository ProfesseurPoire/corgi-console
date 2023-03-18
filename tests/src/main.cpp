#include <corgi/plot/plot.h>

#include <iostream>

int main(int argc, char** argv)
{
    corgi::plot::init_plot();

    corgi::plot::add_point_group({{0, 0}, {1, 0}, {2, 1}}, {0, 255, 255});

    corgi::plot::show();
    return 0;
}