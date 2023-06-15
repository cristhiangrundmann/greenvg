#include "img.hpp"
#include "stb_image_write.hpp"
#include <cmath>

using namespace gvg;

static color_t blend(color_t cur, color_t src, real_t v)
{
    float f = v*src.a;
    return
    (color_t)
    {
        (float)fma(f, src.r - cur.r, cur.r),
        (float)fma(f, src.g - cur.g, cur.g),
        (float)fma(f, src.b - cur.b, cur.b),
        (float)fma(f, 1 - cur.a, cur.a)
    };
}

void shader_t::paint_pixel(core_t *core, int x, int y, real_t v)
{
    img_t *img = (img_t*)core;
    if(v < 1.0/255) return;
    if(v > 1) v = 1;
    color_t &cur = img->img[y*img->width+x];
    color_t src = get_color(x, y);
    cur = blend(cur, src, v);
}

void shader_t::clear(img_t *img)
{
    for(int y = 0; y < img->height; y++)
    for(int x = 0; x < img->width; x++)
    img->img[y*img->width+x] = get_color(x, y);
}

img_t::img_t(int width, int height) : 
core_t(width, height)
{
    img = new color_t[width*height];
}

img_t::~img_t()
{
    delete[] img;
}

color_t color_shader_t::get_color(int x, int y)
{
    return col;
}

color_shader_t::color_shader_t(color_t col) :
col(col) {}

color_t gradient_shader_t::get_color(int x, int y)
{
    float t = (x-ta)/tbta;
    return 
    {
        a.r + t*ba.r,
        a.g + t*ba.g,
        a.b + t*ba.b,
        a.a + t*ba.a
    };
}

gradient_shader_t::gradient_shader_t(color_t a, color_t b, float ta, float tb) : 
a(a), ba({b.r-a.r, b.g-a.g, b.b-a.b, b.a-a.a}), 
ta(ta), tbta(tb-ta), tb(tb) {}

void img_t::export_to(const char* filename)
{
    unsigned char *data = new unsigned char[width*height*4];

    int index = 0;
    for(int y = 0; y < height; y++)
    for(int x = 0; x < width; x++)
    {
        color_t c = img[y*width+x];
        data[index++] = floor(c.r*255);
        data[index++] = floor(c.g*255);
        data[index++] = floor(c.b*255);
        data[index++] = floor(c.a*255);
    }

    stbi_flip_vertically_on_write(1);
    stbi_write_png(filename, width, height, 4, data, width*4);

    delete[] data;
}