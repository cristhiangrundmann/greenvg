#include "app.hpp"
#include <stdio.h>
#include <chrono>

using namespace gvg;

img_t img = {width, height};
float cur_time = 0;

static int frame_count = 0;
static float frame_time = 0;

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

auto t0 = high_resolution_clock::now();

static void render_func(void)
{
    auto t1 = high_resolution_clock::now();
    cur_time = ((duration<float, std::milli>)(t1-t0)).count()/1000;
    draw();
    auto t2 = high_resolution_clock::now();
    frame_time += ((duration<float, std::milli>)(t2-t1)).count()/1000;
    frame_count++;
    glDrawPixels(width, height, GL_RGBA, GL_FLOAT, (const GLvoid*)img.img);
    glutSwapBuffers();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow(name);
    glutDisplayFunc(render_func);
    glutIdleFunc(render_func);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	glutMainLoop();

    //results
    draw();
    char buffer[512];
    sprintf(buffer, "%s.png", name);
    img.export_to(buffer);
    printf("Average FPS: %f\n", frame_count/frame_time);

    return 0;
}