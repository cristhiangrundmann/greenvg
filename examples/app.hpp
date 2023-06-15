//example implementations must provide:
//a draw function
//width and height for the image
//a name for the example, so that name+.png will be written

#pragma once

#include <GL/freeglut.h>
#include "img.hpp"

extern int width, height;
extern const char *name;
extern gvg::img_t img;

extern float cur_time;

void draw();
int main(int argc, char **argv);