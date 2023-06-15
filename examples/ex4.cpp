#include "app.hpp"
#include <cmath>

using namespace gvg;

int width = 1920;
int height = 1080;
const char *name = "ex4_snowflakes2";
const float pi = 3.1415926;

static void sketch_snowflake_line(vec2 a, vec2 b, int it)
{
    if(it == 0)
    {
        img.sketch_line(a, b);
        return;
    }

    vec2 ds2 = (b-a)*(1.0/2);
    vec2 ds3 = (b-a)*(1.0/3);
    vec2 ds3n = (vec2){ds3.y, -ds3.x}*(sqrt(3)/2);

    sketch_snowflake_line(a, a+ds3, it-1);
    sketch_snowflake_line(a+ds3, a+ds2 + ds3n, it-1);
    sketch_snowflake_line(a+ds2 + ds3n, a + ds3*2, it-1);
    sketch_snowflake_line(a + ds3*2, b, it-1);
}

static void sketch_snowflake(vec2 c, real_t r, real_t t, int max_it)
{
    vec2 p0 = {c.x + r*(float)cos(t), c.y + r*(float)sin(t)};
    vec2 p1 = {c.x + r*(float)cos(t+2*pi/3), c.y + r*(float)sin(t+2*pi/3)};
    vec2 p2 = {c.x + r*(float)cos(t+4*pi/3), c.y + r*(float)sin(t+4*pi/3)};

    sketch_snowflake_line(p0, p1, max_it);
    sketch_snowflake_line(p1, p2, max_it);
    sketch_snowflake_line(p2, p0, max_it);
}

color_shader_t backcolor({1, 1, 1, 1});
gradient_shader_t forecolor({1, 0, 0, 1}, {0, 1, 1, 1}, 0, width);

void draw()
{
    backcolor.clear(&img);
    real_t r = 512;
    real_t r0 = 10;
    int n0 = 10;
    int n = 3000;
    real_t t = cur_time/10;
    real_t k = 1;

    for(int i = 0; i < n; i++)
    {
        t *= -1;
        k += 1;
        real_t t0 = t*(real_t)(i%12+1)/12+k;
        sketch_snowflake({width/2 + r*(float)cos(t0)*i/n, height/2 + r*(float)sin(t0)*i/n}, r0, t0, 2);
    }
    forecolor.paint(&img);
}