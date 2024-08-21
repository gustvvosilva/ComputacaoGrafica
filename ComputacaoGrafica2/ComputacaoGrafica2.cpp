#include <stdio.h>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>

const GLint WIDTH = 800, HEIGHT = 600;

int main()
{
	if (!glfwInit()) {
		printf("erro no glfw");
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Computacao grafica", NULL, NULL);
	if (!window) {
		printf("A janela nao pode ser criada");
		glfwTerminate();
		return 1;
	}
	
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		printf("Glew nao foi iniciado");
		glfwDestroyWindow(window);
		glfwTerminate();
		return 1;
	}

	glViewport(0, 0, bufferWidth, bufferHeight);
	
	while (!glfwWindowShouldClose(window)) {
		//cria um evento
		glfwPollEvents();

		//Define a cor do fundo
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);

	}
}