//Ctrl+k , Ctrl+c 전체 주석처리
//Ctrl+k , Ctrl+u 전체 주석해제
//3D gasket

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "InitShader.h"
#include "math.h"
#include "vec.h"
#include <iostream>

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

int NumSubdivides;
int NumTriangles;
int NumVertices;

vec3* points;
vec3* colors;
int index = 0;

void init();
void display();
void idle();
void divide_triangle(vec3 a, vec3 b, vec3 c, unsigned int cnt,unsigned int coli);
void triangle(vec3 a, vec3 b, vec3 c, unsigned int coli);

void setGasketVertices(unsigned int col1, unsigned int col2, unsigned int col3, unsigned int col4);
void gasketSetKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void gasketSetMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

vec3 baseVertices[] =
{
   vec3(0.0f, 0.0f, 1.0f),
   vec3(0.0f, 0.942809f, -0.333333f),
   vec3(-0.816497f, -0.471405f, -0.333333f),
   vec3(0.816497f, -0.471405f, -0.333333f)
};

vec3 baseColors[] =
{
   vec3(1.0f, 0.0f, 0.0f),
   vec3(0.0f, 1.0f, 0.0f),
   vec3(0.0f, 0.0f, 1.0f),
   vec3(1.0f, 0.0f, 1.0f)
};

int main()
{
   GLFWwindow* window;
   if (!glfwInit()) /* Initialize the library */
      return -1;

   window = glfwCreateWindow(512, 512, "practice0507", NULL, NULL);
   if (!window)
   {
      glfwTerminate();
      return -1;
   }
   glfwMakeContextCurrent(window); /* Make the window's context current */

   if (glewInit() != GLEW_OK) std::cout << "Error\n";

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

   init();

   glfwSetKeyCallback(window, gasketSetKeyCallback);
   glfwSetMouseButtonCallback(window, gasketSetMouseButtonCallback);

///////////////////////////////////////////////////////////////////////////
/* Loop until the user closes the window */
   while (!glfwWindowShouldClose(window))
   {
      display();
      idle();

      glfwSwapBuffers(window);
      glfwPollEvents();
   }
   glfwTerminate();
   free(points);
   free(colors);
   return 0;
}

void init()
{
   NumSubdivides=3;
   NumTriangles=4*pow(3.0f,NumSubdivides);
   NumVertices=3*NumTriangles;

   points = (vec3*)malloc(sizeof(vec3)*NumVertices);
   colors = (vec3*)malloc(sizeof(vec3)*NumVertices);

   // set gasket vertices
   divide_triangle(baseVertices[0],baseVertices[1], baseVertices[2], NumSubdivides, 0);
   divide_triangle(baseVertices[3],baseVertices[2], baseVertices[1], NumSubdivides, 1);
   divide_triangle(baseVertices[0],baseVertices[3], baseVertices[1], NumSubdivides, 2);
   divide_triangle(baseVertices[0],baseVertices[2], baseVertices[3], NumSubdivides, 3);

   GLuint vao;
   glGenVertexArrays(1, &vao);
   glBindVertexArray(vao);

   GLuint buffer;
   glGenBuffers(1, &buffer);
   glBindBuffer(GL_ARRAY_BUFFER, buffer);
   glBufferData(GL_ARRAY_BUFFER, 2*sizeof(vec3)*NumVertices, NULL, GL_STATIC_DRAW);
   glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3)*NumVertices, points);
   glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec3)*NumVertices, sizeof(vec3)*NumVertices, colors);
   // bufferdata

   GLuint simpleProgram = InitShader("vshader0507.glsl", "fshader0507.glsl");
   glUseProgram(simpleProgram);

   GLuint vPosition = glGetAttribLocation(simpleProgram, "vPosition");
   glEnableVertexAttribArray(vPosition);
   glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

   GLuint vColor = glGetAttribLocation(simpleProgram, "vColor");
   glEnableVertexAttribArray(vColor);
   glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec3)*NumVertices));

   glEnable(GL_DEPTH_TEST);
   glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void display()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

void idle()
{
   
}

void divide_triangle(vec3 a, vec3 b, vec3 c, unsigned int cnt, unsigned int coli)
{
   if(cnt > 0)
   {
      vec3 ab = (a+b)/2.0f;
      vec3 ac = (a+c)/2.0f;
      vec3 bc = (b+c)/2.0f;

      divide_triangle(a, ab, ac, cnt-1, coli);
      divide_triangle(c, ac, bc, cnt-1, coli);
      divide_triangle(b, bc, ab, cnt-1, coli);
   }
   else
   {
      triangle(a,b, c, coli);
   }
   
   
}

void triangle(vec3 a, vec3 b, vec3 c, unsigned int coli)
{
   points[index] = a;
   colors[index++]  = baseColors[coli];
   points[index] = b;
   colors[index++]  = baseColors[coli];
   points[index] = c;
   colors[index++]  = baseColors[coli];
   //coli++하면 색깔 이쁨 대신 면의 색상들을 구분할 수 없음.

}

void gasketSetKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   if(key == GLFW_KEY_1 && action == GLFW_PRESS)
   {
      std::cout << "Key 1 is pressed\n";
   }

}
void gasketSetMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
   if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
   {
      std::cout << "Left button is pressed\n";
   }
}

void setGasketVertices(unsigned int col1, unsigned int col2, unsigned int col3, unsigned int col4)
{
   std::cout << "Left button is pressed\n";

}
