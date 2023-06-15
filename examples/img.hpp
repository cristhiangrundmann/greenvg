#pragma once

#include "../core.hpp"

namespace gvg
{

struct color_t;
struct shader_t;
struct img_t;

struct color_t
{
    real_t r, g, b, a;
};

struct shader_t : public painter_t
{
    virtual color_t get_color(int x, int y) = 0;
    void paint_pixel(core_t *core, int x, int y, real_t v) override;
    void clear(img_t *img);
};

struct img_t : core_t
{
    color_t *img;
    img_t(int width, int height);
    ~img_t();
    void export_to(const char *filename);
};

struct color_shader_t : public shader_t
{
    color_t col;
    color_shader_t(color_t col);
    color_t get_color(int x, int y) override;  
};

struct gradient_shader_t : public shader_t
{
    color_t a, ba;
    float ta, tbta, tb;
    gradient_shader_t(color_t a, color_t b, float ta, float tb);
    color_t get_color(int x, int y) override;
};

}