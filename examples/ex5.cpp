#include "app.hpp"
#include <cmath>

using namespace gvg;

int width = 1920;
int height = 1080;
const char *name = "ex5_texts";
const float pi = 3.1415926;

#include "textpath.h"

static void sketch_path(vec2 pos, float scale)
{
    for(int i = 0; i < sizeof(fontstring)/sizeof(Line); i++)
    {
        vec2 p0 = fontstring[i].p0;
        vec2 p1 = fontstring[i].p1;
        img.sketch_line(pos + p0*scale, pos + p1*scale);
    }
}

color_shader_t backcolor({1, 1, 1, 1});
gradient_shader_t forecolor({1, 0, 0, 1}, {0, 1, 1, 1}, 0, width);

void draw()
{
    backcolor.clear(&img);
    real_t scale = 0.1;
    real_t r = 32;
    real_t t = cur_time;
    for(real_t y = 0; y < height; y += scale*10)
    {
        sketch_path({r+r*(float)cos(t), y+r+r*(float)sin(t)}, scale);
        scale += 0.05;
    }

    forecolor.paint(&img);
}