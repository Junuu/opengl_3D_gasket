/*
ctrl+k ctrl+c 전체주석처리
ctrl+k ctrl+u 전체주석해제
shader를 사용하여 gasket 3D를 출력하는 코드입니다.

*/


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "InitShader.h"
#include <iostream>
//glew를 glfw보다 먼저 include하지 않으면 오류
#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

typedef float vector[3];
vector vertex[] = { { 0.0,0.0,1.0 },{ 0.0, 0.942809, -0.33333 },{-0.816497, -0.471405,-0.333333 },{0.816497, -0.471405, -0.333333} };	

int count=1;
//초기 좌표 설정(blaockboard 값)
//GLfloat colors[4][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0},
//{0.0, 0.0, 1.0}, {0.0, 0.0, 0.0}};




void triangle(vector dot1, vector dot2, vector dot3)
{
	
	glBegin(GL_TRIANGLES);
	
	glVertex3fv(dot1);
	
	glVertex3fv(dot2);
	
	glVertex3fv(dot3);
	
	glEnd();
	//3개의 정점으로 트라이앵글을 그림
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
	else(triangle(dot1, dot2, dot3)); // 재귀함수가 끝나면 삼각형을 그림

}

int main()
{
	int divide_time=6; //재귀함수 호출 횟수
	GLFWwindow* window;
	if (!glfwInit()) // glfw 라이브러리 초기화
		return -1;
	
	window = glfwCreateWindow(512, 512, "Gasket 3D", NULL, NULL);	// window 생성
	if (!window)	// window 생성 실패 시 프로그램 종료
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // 매개변수 window를 도화지로 삼아 그림을 그림

	if (glewInit() != GLEW_OK) std::cout << "Error\n";	// glew 라이브러리 초기화 및 초기화 실패 시 에러 메세지
														// glew 라이브러리는 반드시 window context 생성 후 초기화 아니면 에러

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
float colors[] =	// 사각형의 각 모서리에 대한 색상 정보 1차원 배열 (r, g, b, a)
	{
		1.0f, 1.0f, 0.0f, 1.0f,	// 0
		0.0f, 0.0f, 0.0f, 1.0f,	// 1
		0.0f, 1.0f, 1.0f, 1.0f,	// 2
		1.0f, 0.0f, 1.0f, 1.0f	// 3
	};	
	

	
	GLuint va;	// vertex array 선언, 메모리 할당, 바인드
	glGenVertexArrays(1, &va);
	glBindVertexArray(va);

	GLuint vb;	// vertex buffer 선언, 메모리 할당, 바인드, data가 구성된 형식에 맞게 buffer data 준비
	glGenBuffers(1, &vb);
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glBufferData(GL_ARRAY_BUFFER, 5000*sizeof(float), vertex, GL_STATIC_DRAW); // buffer사이즈를 float5000으로 설정하고 vertex라는 배열에서 data를 불러와 저장함
	glBufferSubData(GL_ARRAY_BUFFER, 0, 5000*sizeof(float), vertex);	// 공간의 처음(0)부터 sizeof(positions) 크기까지는 positions 배열의 정보를 저장
	glBufferSubData(GL_ARRAY_BUFFER, 5000*sizeof(float), sizeof(colors), colors);	// 공간의 sizeof(positions)부터 sizeof(colors) 크기까지는 colors 배열의 정보를 저장

	// 6*2*sizeof(float) = (vertex 갯수)*(각 vertex의 요소 갯수)*(각 요소 데이터 타입의 크기) 5000으로 크게잡음
	GLuint program = InitShader("vshader3D.glsl", "fshader3D.glsl");	// shader program 가져오기
	glUseProgram(program);	// 어떤 shader program을 사용할 것인지

	GLuint location = glGetAttribLocation(program, "vPosition");	// position vertex에 대한 정보를 shader program 안의 어떤 변수와 연결시킬 것인가
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(vb, 1, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0)); // 1개씩 불러오고 바로뒤에 다음 정점이 호출됨


	//glEnableVertexAttribArray(1);
	//glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	//glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,0);

	GLuint locColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(locColor);
	glVertexAttribPointer(locColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(5000*sizeof(float)));	// vb의 sizeof(positions) 위치에서부터 color에 대한 정보를 가져옴
	
	glEnable(GL_DEPTH_TEST); //앞뒤를 구분 시켜 줌
	glClearColor(1.0, 1.0, 1.0, 1.0); //윈도우의 배경을 하얀색으로 변경
	
	
	
///////////////////////////////////////////////////////////////////////////
// window 창이 종료될 때까지 무한루프 
	while (!glfwWindowShouldClose(window))
	{
		

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
		
		divide_triangle(vertex[1], vertex[2], vertex[3], divide_time); 		
		divide_triangle(vertex[0], vertex[2], vertex[3], divide_time);		
		divide_triangle(vertex[0], vertex[1], vertex[2], divide_time);		
		divide_triangle(vertex[0], vertex[1], vertex[3], divide_time);
		//4면을 그리기위해 4번 호출
		//면마다 색상을 바꿔주고싶음
		
		

	//////////////////////////////////////////////////////////////////////
		//glDrawArrays(GL_TRIANGLES, 0, 5000); //0~5000까지 점을 그려줌
		// glDrawArrays 함수는 중복되는 vertex를 포함하는 vertex 정보를 사용함.
		// vertex의 index를 참조해가는 방식이 아닌,
		// vertex 정보가 들어있는 positions 배열을 차례로 읽어 3개씩 하나의 삼각형으로 처리하는 방식.

	//////////////////////////////////////////////////////////////////////
		glfwSwapBuffers(window);	// front buffer와 back buffer 교체
		glfwPollEvents(); //윈도우에 명령을 내릴 수 있게 이벤트를 처리해주는 함수
	}
	glfwTerminate();
	
	return 0;
}