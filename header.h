#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION

#endif

#define _USE_MATH_DEFINES // M_PI constant


#include <stdlib.h>
#include <stdio.h>
#include <GLFW/glfw3.h>

#include <math.h>



#define MCML_SIZE_X 50
#define MCML_SIZE_Y 50
#define MCML_SIZE_Z 200

#pragma once







struct Data;

struct Vertex;

struct Color;







void loadMCML(void);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

Color getHeatMapColor(float v, float vmin, float vmax);

void drawMCMLPoints(void);

void drawMCMLSlices(void);


void drawOrigin(void);



  
