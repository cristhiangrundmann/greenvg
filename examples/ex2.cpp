#include "app.hpp"
#include <cmath>

using namespace gvg;

int width = 1920;
int height = 1080;
const char *name = "ex2_carpets";
const float pi = 3.1415926;

static void sketch_carpet_remove(vec2 min, vec2 d3, int it)
{
    if(it == 0) return;
    img.sketch_line({min.x+d3.x*2, min.y+d3.y*2}, {min.x+d3.x*2, min.y+d3.y*1});
    img.sketch_line({min.x+d3.x*1, min.y+d3.y*1}, {min.x+d3.x*1, min.y+d3.y*2});
    vec2 nd3 = {d3.x/3, d3.y/3};

    sketch_carpet_remove({min.x + d3.x*0, min.y + d3.y*0}, nd3, it-1);
    sketch_carpet_remove({min.x + d3.x*1, min.y + d3.y*0}, nd3, it-1);
    sketch_carpet_remove({min.x + d3.x*2, min.y + d3.y*0}, nd3, it-1);
    sketch_carpet_remove({min.x + d3.x*0, min.y + d3.y*1}, nd3, it-1);
    sketch_carpet_remove({min.x + d3.x*2, min.y + d3.y*1}, nd3, it-1);
    sketch_carpet_remove({min.x + d3.x*0, min.y + d3.y*2}, nd3, it-1);
    sketch_carpet_remove({min.x + d3.x*1, min.y + d3.y*2}, nd3, it-1);
    sketch_carpet_remove({min.x + d3.x*2, min.y + d3.y*2}, nd3, it-1);

}

static void sketch_carpet(vec2 min, vec2 size, int max_it)
{
    img.sketch_line({min.x+size.x, min.y}, {min.x+size.x, min.y+size.y});
    img.sketch_line({min.x, min.y+size.y}, {min.x, min.y});

    sketch_carpet_remove(min, size*(1.0/3), max_it);
}

color_shader_t backcolor({1, 1, 1, 1});
gradient_shader_t forecolor({1, 0, 0, 1}, {0, 1, 1, 1}, 0, width);

void draw()
{
    backcolor.clear(&img);
    real_t r = 32;
    real_t t = cur_time;
    real_t dim = 128;

    for(real_t x = 0; x < width; x += dim+10)
    for(real_t y = 0; y < height; y += dim+10)
    {
        sketch_carpet({r+r*(float)cos(t)+x, r+r*(float)sin(t)+y}, {dim, dim}, 4);
    }
    forecolor.paint(&img);
}