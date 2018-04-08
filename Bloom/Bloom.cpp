// Bloom.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "shader.h"

using namespace std;

GLuint VAO, VAO2, VBO, VBO2;
GLuint programId, programId2;
GLuint FBO;
GLuint colorBuffer, hdrBuffer;
GLfloat vertex1[] = {
	0.0,0.0,2.0,0.0,0.0,
	0.0,0.5,2.0,0.0,0.0,
	0.5,0.5,2.0,0.0,0.0,

	0.0,0.0,0.0,0.8,0.0,
	0.0,-0.5,0.0,0.8,0.0,
	-0.5,-0.5,0.0,0.8,0.0,
};
GLfloat vertex2[] = { -1,-1,-1,1,1,1,1,-1 };
void init() {
	//base setting
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);

	//VAO VBO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex1), vertex1, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)(sizeof(GLfloat) * 2));
	glEnableVertexAttribArray(1);

	//create shader 
	vector<GLuint> v;
	v.push_back(Shader::createShader(GL_VERTEX_SHADER, "vertexShader.glsl"));
	v.push_back(Shader::createShader(GL_FRAGMENT_SHADER, "fragmentShader.glsl"));
	programId = Shader::createProgram(v);

	//fbo
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glGenTextures(1, &colorBuffer);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);

	glGenTextures(1, &hdrBuffer);
	glBindTexture(GL_TEXTURE_2D, hdrBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, hdrBuffer, 0);

	GLuint attachments[] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//second render
	//VAO VBO
	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO2);

	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex2), vertex2, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, 0);
	glEnableVertexAttribArray(0);

	v.clear();
	v.push_back(Shader::createShader(GL_VERTEX_SHADER, "vertexShader2.glsl"));
	v.push_back(Shader::createShader(GL_FRAGMENT_SHADER, "fragmentShader2.glsl"));
	programId2 = Shader::createProgram(v);

}

void display() {
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(VAO);
	glUseProgram(programId);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glUseProgram(0);
	glBindVertexArray(0);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(VAO2);
	glUseProgram(programId2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	glUniform1i(glGetUniformLocation(programId2, "colorBuffer"), 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, hdrBuffer);
	glUniform1i(glGetUniformLocation(programId2, "hdrBuffer"), 1);

	glDrawArrays(GL_QUADS, 0, 4);
	glUseProgram(0);
	glBindVertexArray(0);

	glutSwapBuffers();
}
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Triangle demo");

	glewInit();
	init();

	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}
