#include "header.h"


struct Data
{
  GLfloat x,y,z;
};

struct Vertex
{
	GLfloat x, y, z;
	GLfloat r, g, b, a;
};


struct Color {
  GLfloat r,g,b;
};

float mcml_data[MCML_SIZE_X][MCML_SIZE_Y][MCML_SIZE_Z];
Vertex  mcml_vertices[MCML_SIZE_X][MCML_SIZE_Y][MCML_SIZE_Z]; //inefficient







//simple parser that extracts the simulation output matrix from the file and stores it in the local memory.
void loadMCML()
{

  float max_data, min_data, half_max;


  FILE *ifp;
  //open the file for reading
  ifp = fopen("MCML_output.txt", "r");
  if(ifp == NULL) {
    fprintf(stderr, "ERROR: Can't open MCML Data file !\n");
    exit(1);
  }
  float data;
  float max=0, min=9999999;
  for(int x =0; x<MCML_SIZE_X; x++){
    for(int z =0; z<MCML_SIZE_Z; z++){
      for(int y =0; y<MCML_SIZE_Y; y++){
	if(fscanf(ifp, "%f\n",&data) == EOF){
	  fprintf(stderr, "ERROR: Missing MCML Data file !\n");
	  exit(1);
	}
	//store the log compressed data point
	data = log(data+1);
	mcml_data[x][y][z]=data;

	//find the max and min from the data set for heatmap
	if(data>max){
	  max = data;
	}
	if(data<min){
	  min=data;
	}

	//normalize the coordinates, each data point is stored as a vertex for the rendering
	//pipeline
	mcml_vertices[x][y][z].x=(float)(x-MCML_SIZE_X/2.0f)/MCML_SIZE_X;
        mcml_vertices[x][y][z].y=(float)(y-MCML_SIZE_Y/2.0f)/MCML_SIZE_Y;
        mcml_vertices[x][y][z].z=(float)(z-MCML_SIZE_Z/2.0f)/MCML_SIZE_Z*2.0f;
      }
    }
  }
  fclose(ifp);
  max_data= max;
  min_data = min;
  half_max = (max+min)/2.0f;

  //store the heat map representation of the data
  for(int z =0; z<MCML_SIZE_Z; z++){  
   for(int x =0; x<MCML_SIZE_X; x++){
    for(int y =0; y<MCML_SIZE_Y; y++){
      float value = mcml_data[x][y][z];
      Color c = getHeatMapColor(value, min_data,max_data);
      mcml_vertices[x][y][z].r = c.r;
      mcml_vertices[x][y][z].g = c.g;
      mcml_vertices[x][y][z].b = c.b;
    }
   }
  }
}


// implement the heat map generator with jet color scheme;

Color getHeatMapColor(float value, float min, float max)
{
  //remap the value to the JET color scheme
  Color c = {1.0f,1.0f,1.0f}; //default
  float dv;
  float range=1.0f;
  //clamp the data
  if(value < min)
    value = min;
  if(value > max)
    value = max;
  range = max - min;

  // the first region (0% to 25%)
  if(value < (min+0.25f * range)){
    c.r = 0.0f;
    c.g = 4.0f * (value - min) / range;
  }

  //the second region of value ( 25% to 50% )
  else if (value < (min + 0.5f *range)){
    c.r = 0.0f;
    c.b = 1.0f + 4.0f * (min + 0.25f * range - value) / range;
  }
  //the third region of value (50% to 75%)
  else if (value < (min + 0.75f * range)){
    c.r = 4.0f * (value - min - 0.5f * range) / range;
    c.b = 0.0f;
  }

  //the fourth region (75% to 100%)
  else{
    c.g = 1.0f + 4.0f * (min+0.75f * range - value) / range;
    c.b = 0.0f;
  }
  return (c);
}


//draw all the data points on screen
void drawMCMLPoints(){
  float point_size = 5.0f;
  glPointSize(point_size);
  glBegin(GL_POINTS);
  for(int z=0; z<MCML_SIZE_Z; z++){
    for(int x=0; x<MCML_SIZE_X; x++){
      for(int y=0; y<MCML_SIZE_Y; y++){
        glColor4f(mcml_vertices[x][y][z].r,mcml_vertices[x][y][z].g,mcml_vertices[x][y][z].b, 0.15f);
        glVertex3f(mcml_vertices[x][y][z].x,mcml_vertices[x][y][z].y,mcml_vertices[x][y][z].z);
      }
    }
  }
  glEnd();
}


//only draw three slices

void drawMCMLSlices(){

  int slice_x = 0, slice_y = 0, slice_z = 0;
  
  glPointSize(10.0f);
  glBegin(GL_POINTS);

  //display data on xy plane
  for(int x=0; x<MCML_SIZE_X; x++){
    for(int y=0; y<MCML_SIZE_Y; y++){
      int z = slice_z;
      glColor4f(mcml_vertices[x][y][z].r,mcml_vertices[x][y][z].g,mcml_vertices[x][y][z].b, 0.9f);
      glVertex3f(mcml_vertices[x][y][z].x,mcml_vertices[x][y][z].y,mcml_vertices[x][y][z].z);
    }
  }

  //display data on yz plane
  for(int z=0; z<MCML_SIZE_Z; z++){
    for(int y=0; y<MCML_SIZE_Y; y++){
      int x = slice_x;
      glColor4f(mcml_vertices[x][y][z].r,mcml_vertices[x][y][z].g,mcml_vertices[x][y][z].b, 0.9f);
      glVertex3f(mcml_vertices[x][y][z].x,mcml_vertices[x][y][z].y,mcml_vertices[x][y][z].z);
    }
  }

  //display data on xz plane
  for(int z=0; z<MCML_SIZE_Z; z++){
    for(int x=0; x<MCML_SIZE_X; x++){
      int y = slice_y;
      glColor4f(mcml_vertices[x][y][z].r,mcml_vertices[x][y][z].g,mcml_vertices[x][y][z].b, 0.9f);
      glVertex3f(mcml_vertices[x][y][z].x,mcml_vertices[x][y][z].y,mcml_vertices[x][y][z].z);
    }
  }
  glEnd();
}



//key_callback




//draw origin

void drawOrigin(){
	float transparency = 0.5f;
	glLineWidth(4.0f);
	glBegin(GL_LINES);
	//draw a red line for x-axis
	glColor4f(1.0f, 0.0f, 0.0f, transparency);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glColor4f(1.0f, 0.0f, 0.0f, transparency);
	glVertex3f(0.3f, 0.0f, 0.0f);

	//draw a green line for the y-axis
	glColor4f(0.0f, 1.0f, 0.0f, transparency);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glColor4f(0.0f, 1.0f, 0.0f, transparency);
	glVertex3f(0.0f, 0.0f, 0.3f);

	//draw a blue line for the z-axis
	glColor4f(0.0f, 0.0f, 1.0f, transparency);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glColor4f(0.0f, 0.0f, 1.0f, transparency);
	glVertex3f(0.0f, 0.3f, 0.0f);
	glEnd();
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	const double DEG2RAD = 3.14159265 / 180;
	const float fovY = 55.0f;
	const float front = 0.1f;
	const float back = 128.0f;
	float ratio = 1.0f;
	if (height > 0)
		ratio = (float) width / (float) height;

	// Setup viewport
	glViewport(0, 0, width, height);

	// Change to the projection matrix and set our viewing volume
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	double tangent = tan(fovY/2 * DEG2RAD);   // tangent of half fovY
	double height_f = front * tangent;          // half height of near plane
	double width_f = height_f * ratio;      // half width of near plane

	//this will create the projection matrix based on the front
	//clipping plane, and the locations
	glFrustum(-width_f, width_f, -height_f, height_f, front, back);

	//Alternative: gluPerspective will provide the same solution as above
	//gluPerspective(fovY, ratio, front, back);
}
