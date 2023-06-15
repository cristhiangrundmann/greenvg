#pragma once

namespace gvg
{

using real_t = float;

struct vec2;
struct clip_t;
struct core_t;
struct painter_t;

struct vec2
{
    real_t x, y;
    vec2 operator+(const vec2 &v) const;
    vec2 operator-(const vec2 &v) const;
    vec2 operator*(const real_t &v) const;
    vec2 operator/(const real_t &v) const;
};

//axis aligned bounding box of the lines currently sketched
struct clip_t
{
    int min_x, min_y;
    int max_x, max_y;
};


struct core_t
{
    /*
    this buffer is a delta buffer used to compute coverage area for all pixels.
    a row's partial sum defines the coverage area (this makes the path sketching much faster)
    */
    real_t *buffer;
    clip_t clip;
    const int width, height;
    core_t(int width, int height);
    ~core_t();
    void light_pixel(int x, int y, real_t int_xdy, real_t int_dy); //signals some pixel coverage, interpreted as light
    void sketch_line(vec2 p0, vec2 p1); //sketches a line into the buffer
};

//abstract painter class. A concrete painter must override paint_pixel to receive the light values.
struct painter_t
{
    void paint(core_t *core); //paint only inside the clip
    void paint_all(core_t *core); //paint all, ignoring the clip

    /*
    x, y are the pixel coordinates
    v is the light value: 
        0 = 100% outside path
        1 = 100% inside path
        inbetween values usually mean the pixel is in the border,
        and the value itself can be used in antialiasing.
    The v light value is the signed coverage area of the pixel by the path region.
    It can be negative and it can be greater than 1.
    Clamping it to [0, 1] can be used to get the union of paths.
    */
    virtual void paint_pixel(core_t *core, int x, int y, real_t v) = 0;
};

}