/*
ctrl+k ctrl+c ��ü�ּ�ó��
ctrl+k ctrl+u ��ü�ּ�����
shader�� ����Ͽ� gasket 3D�� ����ϴ� �ڵ��Դϴ�.

*/


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "InitShader.h"
#include <iostream>
//glew�� glfw���� ���� include���� ������ ����
#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

typedef float vector[3];
vector vertex[] = { { 0.0,0.0,1.0 },{ 0.0, 0.942809, -0.33333 },{-0.816497, -0.471405,-0.333333 },{0.816497, -0.471405, -0.333333} };	

int count=1;
//�ʱ� ��ǥ ����(blaockboard ��)
//GLfloat colors[4][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0},
//{0.0, 0.0, 1.0}, {0.0, 0.0, 0.0}};




void triangle(vector dot1, vector dot2, vector dot3)
{
	
	glBegin(GL_TRIANGLES);
	
	glVertex3fv(dot1);
	
	glVertex3fv(dot2);
	
	glVertex3fv(dot3);
	
	glEnd();
	//3���� �������� Ʈ���̾ޱ��� �׸�
}

void divide_triangle(vector dot1, vector dot2,vector dot3, int recursive_time)
{
	vector v0,v1,v2;
	int j;
	if(recursive_time>0)
	{
		for (j = 0; j<3; j++) v0[j] = (dot1[j] + dot2[j]) / 2;
		for (j = 0; j<3; j++) v1[j] = (dot1[j] + dot3[j]) / 2;
		for (j = 0; j<3; j++) v2[j] = (dot2[j] + dot3[j]) / 2;
		
		
		divide_triangle(dot1, v0, v1, recursive_time - 1);
		divide_triangle(dot2, v2, v0, recursive_time - 1);
		divide_triangle(dot3, v1, v2, recursive_time - 1);
		
	}
	else(triangle(dot1, dot2, dot3)); // ����Լ��� ������ �ﰢ���� �׸�

}

int main()
{
	int divide_time=6; //����Լ� ȣ�� Ƚ��
	GLFWwindow* window;
	if (!glfwInit()) // glfw ���̺귯�� �ʱ�ȭ
		return -1;
	
	window = glfwCreateWindow(512, 512, "Gasket 3D", NULL, NULL);	// window ����
	if (!window)	// window ���� ���� �� ���α׷� ����
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // �Ű����� window�� ��ȭ���� ��� �׸��� �׸�

	if (glewInit() != GLEW_OK) std::cout << "Error\n";	// glew ���̺귯�� �ʱ�ȭ �� �ʱ�ȭ ���� �� ���� �޼���
														// glew ���̺귯���� �ݵ�� window context ���� �� �ʱ�ȭ �ƴϸ� ����

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
float colors[] =	// �簢���� �� �𼭸��� ���� ���� ���� 1���� �迭 (r, g, b, a)
	{
		1.0f, 1.0f, 0.0f, 1.0f,	// 0
		0.0f, 0.0f, 0.0f, 1.0f,	// 1
		0.0f, 1.0f, 1.0f, 1.0f,	// 2
		1.0f, 0.0f, 1.0f, 1.0f	// 3
	};	
	

	
	GLuint va;	// vertex array ����, �޸� �Ҵ�, ���ε�
	glGenVertexArrays(1, &va);
	glBindVertexArray(va);

	GLuint vb;	// vertex buffer ����, �޸� �Ҵ�, ���ε�, data�� ������ ���Ŀ� �°� buffer data �غ�
	glGenBuffers(1, &vb);
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glBufferData(GL_ARRAY_BUFFER, 5000*sizeof(float), vertex, GL_STATIC_DRAW); // buffer����� float5000���� �����ϰ� vertex��� �迭���� data�� �ҷ��� ������
	glBufferSubData(GL_ARRAY_BUFFER, 0, 5000*sizeof(float), vertex);	// ������ ó��(0)���� sizeof(positions) ũ������� positions �迭�� ������ ����
	glBufferSubData(GL_ARRAY_BUFFER, 5000*sizeof(float), sizeof(colors), colors);	// ������ sizeof(positions)���� sizeof(colors) ũ������� colors �迭�� ������ ����

	// 6*2*sizeof(float) = (vertex ����)*(�� vertex�� ��� ����)*(�� ��� ������ Ÿ���� ũ��) 5000���� ũ������
	GLuint program = InitShader("vshader3D.glsl", "fshader3D.glsl");	// shader program ��������
	glUseProgram(program);	// � shader program�� ����� ������

	GLuint location = glGetAttribLocation(program, "vPosition");	// position vertex�� ���� ������ shader program ���� � ������ �����ų ���ΰ�
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(vb, 1, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0)); // 1���� �ҷ����� �ٷεڿ� ���� ������ ȣ���


	//glEnableVertexAttribArray(1);
	//glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	//glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,0);

	GLuint locColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(locColor);
	glVertexAttribPointer(locColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(5000*sizeof(float)));	// vb�� sizeof(positions) ��ġ�������� color�� ���� ������ ������
	
	glEnable(GL_DEPTH_TEST); //�յڸ� ���� ���� ��
	glClearColor(1.0, 1.0, 1.0, 1.0); //�������� ����� �Ͼ������ ����
	
	
	
///////////////////////////////////////////////////////////////////////////
// window â�� ����� ������ ���ѷ��� 
	while (!glfwWindowShouldClose(window))
	{
		

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
		
		divide_triangle(vertex[1], vertex[2], vertex[3], divide_time); 		
		divide_triangle(vertex[0], vertex[2], vertex[3], divide_time);		
		divide_triangle(vertex[0], vertex[1], vertex[2], divide_time);		
		divide_triangle(vertex[0], vertex[1], vertex[3], divide_time);
		//4���� �׸������� 4�� ȣ��
		//�鸶�� ������ �ٲ��ְ����
		
		

	//////////////////////////////////////////////////////////////////////
		//glDrawArrays(GL_TRIANGLES, 0, 5000); //0~5000���� ���� �׷���
		// glDrawArrays �Լ��� �ߺ��Ǵ� vertex�� �����ϴ� vertex ������ �����.
		// vertex�� index�� �����ذ��� ����� �ƴ�,
		// vertex ������ ����ִ� positions �迭�� ���ʷ� �о� 3���� �ϳ��� �ﰢ������ ó���ϴ� ���.

	//////////////////////////////////////////////////////////////////////
		glfwSwapBuffers(window);	// front buffer�� back buffer ��ü
		glfwPollEvents(); //�����쿡 ����� ���� �� �ְ� �̺�Ʈ�� ó�����ִ� �Լ�
	}
	glfwTerminate();
	
	return 0;
}