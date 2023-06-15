#include "app.hpp"
#include <cmath>

using namespace gvg;

int width = 1920;
int height = 1080;
const char *name = "ex1_pentagons";
const float pi = 3.1415926;

vec2 get_circle_point(vec2 c, float r, int n, int sign, float t)
{
    return {(r*(float)cos(t*sign)+c.x), (r*(float)sin(t*sign)+c.y)};
}

void sketch_regular_polygon(vec2 c, float r, int n, int sign, float angle)
{
    float k = (float)2*pi/n;
    vec2 first = get_circle_point(c, r, n, sign, 0+angle);
    vec2 p0 = first;
    vec2 p1;
    for(int i = 1; i < n; i++)
    {
        p1 = get_circle_point(c, r, n, sign, i*k+angle);
        img.sketch_line(p0, p1);
        p0 = p1;
    }
    img.sketch_line(p0, first);
}

color_shader_t backcolor({1, 1, 1, 1});
gradient_shader_t forecolor({1, 0, 0, 1}, {0, 1, 1, 1}, 0, width);

void draw()
{
    backcolor.clear(&img);
    real_t t = cur_time;
    real_t r = 16;
    int n = 5;
    for(real_t x = 0; x < width; x += 2*r+1)
    for(real_t y = 0; y < height; y += 2*r+1)
    {
        sketch_regular_polygon({x, y}, r, n, 1, t);
    }
    forecolor.paint(&img);
}