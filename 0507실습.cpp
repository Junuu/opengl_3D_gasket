//Ctrl+k , Ctrl+c 전체 주석처리
//Ctrl+k , Ctrl+u 전체 주석해제
//3D gasket with interface
//1을 누르면 색상이 바뀜

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "InitShader.h"
#include "math.h"
#include "vec.h"
#include <iostream>

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

int NumSubdivides=2; //초기 분할 횟수
int NumTriangles;
int NumVertices;

int color_a=0; //Left Click에 관한 색변경을 위한 변수 
int color_b=1;
int color_c=2;
int color_d=3;

int first=0; // 회전을 위한 변수
int x_flag=0;
int y_flag=0;
int z_flag=0;

float scale = 0.5f; //사면체의 크기 변환을 위한 변수
float thetaX = 0.0; //사면체의 X회전을 위한 변수
float thetaY = 0.0; //사면체의 Y회전을 위한 변수
float thetaZ = 0.0; //사면체의 Z회전을 위한 변수
vec3* points;
vec3* colors;

int index = 0;


GLuint vao;
GLuint buffer;
GLuint simpleProgram;
mat4 rotation;

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
   vec3(1.0f, 0.0f, 1.0f),
   vec3(0.5f, 0.5f, 0.5f)
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
   NumSubdivides;
   NumTriangles=4*pow(4.0f,NumSubdivides);
   NumVertices=3*NumTriangles; //회색부분을 위해 동적할당을 3->4로 바꾸어 주어야 함
   
   
   points = (vec3*)malloc(sizeof(vec3)*NumVertices);
   colors = (vec3*)malloc(sizeof(vec3)*NumVertices);

   

   
   glGenVertexArrays(1, &vao);
   glBindVertexArray(vao);

   
   glGenBuffers(1, &buffer);
   glBindBuffer(GL_ARRAY_BUFFER, buffer);
   glBufferData(GL_ARRAY_BUFFER, 2*sizeof(vec3)*NumVertices, NULL, GL_STATIC_DRAW);

   setGasketVertices(color_a,color_b,color_c,color_d);
   

   simpleProgram = InitShader("vshader0507.glsl", "fshader0507.glsl");
   glUseProgram(simpleProgram);

   GLuint vPosition = glGetAttribLocation(simpleProgram, "vPosition");
   glEnableVertexAttribArray(vPosition);
   glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

   GLuint vColor = glGetAttribLocation(simpleProgram, "vColor");
   glEnableVertexAttribArray(vColor);
   glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec3)*NumVertices));
   //mat4 rotation = mat4(1.0f); //단위행렬
   //mat4 rotation = RotateX(thetaX)*RotateY(thetaY)*RotateZ(thetaZ); //math.h 안에 함수가 모두 존재한다.
   
   //GLuint uMat = glGetUniformLocation(simpleProgram, "uMat");
   //glUniformMatrix4fv(uMat, 1, GL_FALSE, rotation);
   //v는 cpu에서 보내주려고 하는 상태 1은 shader에서의 matrix 1개가 들어간다는 의미?, GL_FLASE는 행렬의 tranpose 여부
   glEnable(GL_DEPTH_TEST);
   glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void display()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
   glBindVertexArray(vao);
   glBindBuffer(GL_ARRAY_BUFFER, buffer);
   glUseProgram(simpleProgram);
   rotation = RotateX(thetaX)*RotateY(thetaY)*RotateZ(thetaZ);
   mat4 scaling = Scale(scale);
   GLuint uMat = glGetUniformLocation(simpleProgram, "uMat");
   glUniformMatrix4fv(uMat, 1, GL_FALSE, rotation);

   glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

void idle()
{
   if(first==0 )
   {
	thetaX = 0.0f;
	thetaY = 0.0f;
	thetaZ = 0.0f;
   }
   if(x_flag==1)
   {
	thetaX += 0.1f;
	thetaY = 0.0f;
	thetaZ = 0.0f;
   }
   if(y_flag==1)
   {
	thetaX = 0.0f;
	thetaY += 0.1f;
	thetaZ = 0.0f;
	//std::cout << thetaY << "\n";
   }
   if(z_flag==1)
   {
	thetaX = 0.0f;
	thetaY = 0.0f;
	thetaZ += 0.1f;
   }
}



void divide_triangle(vec3 a, vec3 b, vec3 c, unsigned int cnt, unsigned int coli)
{
	
   if(cnt > 0)
   {
      vec3 ab = (a+b)/2.0f;
      vec3 ac = (a+c)/2.0f;
      vec3 bc = (b+c)/2.0f;
	  
	  //triangle(ab,ac,bc,0); // 가운데 정점들로 회색의 삼각형을 그린다.
	
	  triangle(ab,bc,ac,4); // 가운데 정점들로 회색의 삼각형을 그린다.

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
   points[index] = scale * a;
   colors[index++]  = baseColors[coli];
   points[index] = scale * b;
   colors[index++]  = baseColors[coli];
   points[index] = scale * c;
   colors[index++]  = baseColors[coli];


   //coli++하면 색깔 이뿜

}

void gasketSetKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	
   if(key == GLFW_KEY_1 && action == GLFW_PRESS)
   {
      std::cout << "Key 1 is pressed\n";
	  std::cout << "색 바뀜\n";
	  color_a=1;
	  color_b=2;
	  color_c=3;
	  color_d=0;
	  setGasketVertices(color_a,color_b,color_c,color_d);
   }
   if(key == GLFW_KEY_2 && action == GLFW_PRESS)
   {
      std::cout << "Key 2 is pressed\n";
	  std::cout << "색 바뀜\n";
	  color_a=2;
	  color_b=3;
	  color_c=0;
	  color_d=1;
	  setGasketVertices(color_a,color_b,color_c,color_d);
	  
   }
   if(key == GLFW_KEY_3 && action == GLFW_PRESS)
   {
      std::cout << "Key 3 is pressed\n";
	  std::cout << "색 바뀜\n";
	  color_a=3;
	  color_b=0;
	  color_c=1;
	  color_d=2;
	  setGasketVertices(color_a,color_b,color_c,color_d);
	  
   }
   if(key == GLFW_KEY_4 && action == GLFW_PRESS)
   {
      std::cout << "Key 4 is pressed\n";
	  std::cout << "색 바뀜\n";
	  color_a=0;
	  color_b=1;
	  color_c=2;
	  color_d=3;
	  setGasketVertices(color_a,color_b,color_c,color_d);
   }
   if(key == GLFW_KEY_Q && action == GLFW_PRESS) //q키를 누르면 프로그램 종료
   {
      std::cout << "Key q is pressed\n";
	  glfwTerminate();
	  std::cout << "프로그램이 종료되었습니다. \n";
   }
   if(key == GLFW_KEY_D && action == GLFW_PRESS) // D or d 키를 누르면 분할횟수 감소
   {
      std::cout << "Key D is pressed\n";
	  if(NumSubdivides != 0)
	  {
		  std::cout << "분할 횟수 감소\n";
		NumSubdivides--;
		init();
		setGasketVertices(color_a,color_b,color_c,color_d);
	  }
	  else
	  {
		  std::cout << "나눌 수 없습니다.\n";
	  }
   }
   if(key == GLFW_KEY_U && action == GLFW_PRESS) // U or u 키를 누르면 분할횟수 증가
   {
      std::cout << "Key U is pressed\n";
	 
	  if(NumSubdivides >=10)
	  {
		  std::cout << "메모리 부족 오류 발생 가능\n";
	  }
	  else
	  {
		std::cout << "분할 횟수 증가\n";
		NumSubdivides++;
		init();
		setGasketVertices(color_a,color_b,color_c,color_d);
	  }
	  
   }
   if(key == GLFW_KEY_EQUAL  && action == GLFW_PRESS) // + 키를 누르면 사면체 크기 증가
   {
	   
	std::cout << "Key + is pressed\n" ;
	std::cout << "크기 커짐\n";
	scale = scale + 0.1;
	setGasketVertices(color_a,color_b,color_c,color_d);
	  
   }
   if(key == GLFW_KEY_MINUS && action == GLFW_PRESS) // - 키를 누르면 사면체 크기 감소
   {
	   std::cout << "Key - is pressed\n";
	   scale = scale - 0.1;

	   if(scale<= 0.1)
	   {
		   scale = 0.1;
		   std::cout << "크기가 더이상 작아질 수 없습니다\n";
	   }
	   else
	   {
		   
			std::cout << "크기 작아짐\n";
	   }
	   
	   setGasketVertices(color_a,color_b,color_c,color_d);
   }
}
void gasketSetMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
   if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
   {
      std::cout << "Left button is pressed\n";
	  std::cout << "색 바뀜\n";
	  color_a=(color_a+1)%4;
	  color_b=(color_b+1)%4;
	  color_c=(color_c+1)%4;
	  color_d=(color_d+1)%4;
	  setGasketVertices(color_a,color_b,color_c,color_d);
	  
   }
   if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
   {
	   std::cout << "Right button is pressed\n";
	   if(first ==0)
	   {
		   std::cout << "Z축 회전 \n";		   
		   first=1;
		   z_flag=1;
		   thetaX = 0.0f;
		   thetaY = 0.0f;						   
	   }
	   else if(first ==1)
	   {
		   std::cout << "X축 회전 \n";
		   x_flag=1;
		   z_flag=0;
		   first=2;		   
		   thetaY = 0.0f;
		   thetaZ = 0.0f;
		   
	   }
	   else if(first ==2)
	   {
		   std::cout << "Y축 회전 \n";
		   x_flag=0;
		   y_flag=1;
		   first=3;		
		   thetaX = 0.0f;			
		   thetaZ = 0.0f;
	   }
	   else
	   {
			std::cout << "초기 상태 \n";
			   
			y_flag=0;

			thetaX = 0.0f;
			thetaY = 0.0f;
			thetaZ = 0.0f;
		    first = 0;
		   
	   }
	   init();
	   setGasketVertices(color_a,color_b,color_c,color_d);
   }
}

void setGasketVertices(unsigned int col1, unsigned int col2, unsigned int col3, unsigned int col4)
{
   index = 0;

   // set gasket vertices
   divide_triangle(baseVertices[0],baseVertices[1], baseVertices[2], NumSubdivides, col1);
   divide_triangle(baseVertices[3],baseVertices[2], baseVertices[1], NumSubdivides, col2);
   divide_triangle(baseVertices[0],baseVertices[3], baseVertices[1], NumSubdivides, col3);
   divide_triangle(baseVertices[0],baseVertices[2], baseVertices[3], NumSubdivides, col4);

   glBindVertexArray(vao);
   glBindBuffer(GL_ARRAY_BUFFER,buffer);
   glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3)*NumVertices, points);
   glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec3)*NumVertices, sizeof(vec3)*NumVertices, colors);
   // bufferdata
}
