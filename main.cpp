
#include "header.h"


const int WINDOWS_WIDTH = 640;
const int WINDOWS_HEIGHT = 640;

      
int main(void)
{

  loadMCML();
       

  
  GLFWwindow* window;
  int width, height;

  if (!glfwInit())
    exit(EXIT_FAILURE);
  window = glfwCreateWindow(WINDOWS_WIDTH, WINDOWS_HEIGHT, "Primitive Drawings", NULL, NULL);

  if(!window){
    glfwTerminate();
    exit(EXIT_FAILURE);
  }


  //framebuffer size callback - i.e., resize the windows
  
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


  
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  //get the frame buffer (window) size

  glfwGetFramebufferSize(window, &width, &height);
	
  framebuffer_size_callback(window, width, height);

  //Enable anti-aliasing and smoothing
	
	//enable anti-aliasing
	glEnable(GL_BLEND);
	//smooth the points
	glEnable(GL_LINE_SMOOTH);
	//smooth the lines
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	//needed for alpha blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);

  //Define loop which terminates when the window is closed.
  //and Set up viewport using the size of the window and clear color buffer


	GLfloat alpha = 220.0f, beta = -60.0f, zoom = 2.5f;

	while (!glfwWindowShouldClose(window))
	  {
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	    //draw the scene
	    //switch to modelview so that the tranformation applies to entire model
	    glMatrixMode(GL_MODELVIEW);
	    glLoadIdentity();
	    //move the object back and forth based on the zoom level
	    glTranslatef(0.0, 0.0, -2.5f);
	    // rotate beta degrees around the x-axis
	    glRotatef(beta, 1.0, 0.0, 0.0);
	    // rotate alpha degrees around the z-axis
	    glRotatef(alpha, 0.0, 0.0, 1.0);

	
	    //disable depth test so we can render the points with blending
	    glDisable(GL_DEPTH_TEST);
	    drawMCMLPoints();

	    //must enable this to ensure the slides are rendered in the right order 
	    glEnable(GL_DEPTH_TEST);
	    drawMCMLSlices();

	    //draw the origin with the x,y,z axes for visualization
	    drawOrigin();

      

	    //swap the front and back buffers of the window and process event queue
	    //such as IO to avoid lock-up

	    glfwSwapBuffers(window);
	    glfwPollEvents();
	  }

  //release the memory and termiante the GLFW library

  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
