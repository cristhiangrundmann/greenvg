#include "core.hpp"
#include <cmath>

using namespace gvg;

vec2 vec2::operator+(const vec2 &v) const
{ return {x+v.x, y+v.y}; }

vec2 vec2::operator-(const vec2 &v) const
{ return {x-v.x, y-v.y}; }

vec2 vec2::operator*(const real_t &v) const
{ return {x*v, y*v}; }

vec2 vec2::operator/(const real_t &v) const
{ return {x/v, y/v}; }

core_t::core_t(int width, int height) :
width(width), height(height)
{
    if(width < 0 || height < 0) throw "Invalid dimensions";

    //setup empty clip, which is a degenerate one
    //the clip is considered empty when max_y < 0
    buffer = new real_t[width*height];
    clip = {width, height, -1, -1};
};

core_t::~core_t()
{
    delete[] buffer;
};

/*
signals that a line covers a pixel

x, y are the pixel's coordinate
int_xdy is the integral of xdy of the line inside the pixel
int_dy is the integral of dy of the line inside the pixel

this function also updates the clip
*/
void core_t::light_pixel(int x, int y, real_t int_xdy, real_t int_dy)
{
    if(y < 0 || y >= height) return; //out of bounds

    clip.min_y = std::min(clip.min_y, y);
    clip.max_y = std::max(clip.max_y, y);

    if(x < 0) //still have to write into the buffer at column 0
    {
        buffer[y*width] -= int_dy;
        clip.min_x = 0;
        clip.max_x = std::max(clip.max_x, 0);
    }
    else if(x > width-1)
    {
        clip.min_x = std::min(clip.min_x, width-1);
        clip.max_x = width-1;
    }
    else //main case
    {
        buffer[y*width+x] += int_xdy - int_dy;

        if(x < width-1) 
        {
            buffer[y*width+x+1] -= int_xdy;
            clip.min_x = std::min(clip.min_x, x);
            clip.max_x = std::max(clip.max_x, x+1);
        }
        else
        {
            clip.min_x = std::min(clip.min_x, x);
            clip.max_x = std::max(clip.max_x, x);
        }
    }
}


/*
raster a line into the buffer
the pixel coverage area is analytical

this function breaks up the line segment on pixel boundaries,
computes the integrals, according to `green.pdf`, and calls light_pixel.
*/
void core_t::sketch_line(vec2 p0, vec2 p1)
{
    vec2 dir = p1-p0;
    if(dir.y == 0) return; //horizontal lines have no effect
    if(dir.x == 0) //this separate case is not required and can be removed
    {
        int xi = floor(p0.x);
        int yi = floor(p0.y);
        real_t rdiry = 1.0/dir.y;
        int yd = 1-std::signbit(dir.y)*2;
        real_t dty = yd*rdiry;
        real_t t0y = (yi+(yd+1)/2-p0.y)*rdiry;

        real_t p0mxi = p0.x - xi;

        real_t t0 = 0;
        while(true)
        {
            real_t t1 = std::fmin(t0y, (real_t)1);
            real_t dt = t1-t0;
            real_t dy = dir.y*dt;
            real_t avg_x = p0mxi;

            light_pixel(xi, yi, dy*avg_x, dy);

            t0 = t1;

            if(t1 == t0y)
            {
                t0y += dty;
                yi += yd;
            }
            else break;
        }
        return;
    }

    int xi = floor(p0.x);
    int yi = floor(p0.y);
    real_t rdirx = 1.0/dir.x;
    real_t rdiry = 1.0/dir.y;
    int xd = 1-std::signbit(dir.x)*2;
    int yd = 1-std::signbit(dir.y)*2;
    real_t dtx = xd*rdirx;
    real_t dty = yd*rdiry;
    real_t t0x = (xi+(xd+1)/2-p0.x)*rdirx;
    real_t t0y = (yi+(yd+1)/2-p0.y)*rdiry;

    real_t p0mxi = p0.x - xi;
    real_t dirxo2 = dir.x/2;

    real_t t0 = 0;
    while(true)
    {
        real_t t1 = std::min(std::min(t0x, t0y), (real_t)1);
        real_t dt = t1-t0;
        real_t dy = dir.y*dt;
        real_t avg_x = p0mxi + dirxo2*(t1+t0);

        light_pixel(xi, yi, dy*avg_x, dy);

        t0 = t1;

        if(t1 == t0y)
        {
            t0y += dty;
            yi += yd;
        }
        else if(t1 == t0x)
        {
            t0x += dtx;
            xi += xd;
            p0mxi -= xd;
        }
        else break;
    }
}

//fill the path inside considering its clip
//the clip is reset after painting
void painter_t::paint(core_t *core)
{
    if(core->clip.max_y < 0) return;

    for(int y = core->clip.min_y; y <= core->clip.max_y; y++)
    {
        real_t sum = 0;
        for(int x = core->clip.min_x; x <= core->clip.max_x; x++)
        {
            sum += core->buffer[y*core->width+x];
            core->buffer[y*core->width+x] = 0;
            paint_pixel(core, x, y, sum);
        }
        core->buffer[y*core->width] = 0;
    }

    core->clip.min_x = core->width;
    core->clip.min_y = core->height;
    core->clip.max_x = -1;
    core->clip.max_y = -1;
}

//fill the path ignoring its clip
//the clip is reset after painting
void painter_t::paint_all(core_t *core)
{
    for(int y = 0; y < core->height; y++)
    {
        real_t sum = 0;
        for(int x = 0; x < core->width; x++)
        {
            sum += core->buffer[y*core->width+x];
            core->buffer[y*core->width+x] = 0;
            paint_pixel(core, x, y, sum);
        }
        core->buffer[y*core->width] = 0;
    }

    core->clip.min_x = core->width;
    core->clip.min_y = core->height;
    core->clip.max_x = -1;
    core->clip.max_y = -1;
}