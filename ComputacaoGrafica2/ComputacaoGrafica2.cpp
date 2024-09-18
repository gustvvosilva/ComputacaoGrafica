#include <stdio.h>
#include <string.h>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <time.h>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Mesh.h"
#include "Shader.h"
#include "Window.h"

std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;
Window* window;

static const char* vertexLocation = "VertexShader.glsl";
static const char* fragmentLocation = "FragmentShader.glsl";

// const GLint WIDTH = 800, HEIGHT = 600;

// GLuint shaderProgram;
// std::vector<Mesh*> listMesh;


////Vertex Shader
//static const char* vShader = "                  \n\
//#version 330                                    \n\
//                                                \n\
//layout(location=0) in vec3 pos;                 \n\
//uniform mat4 model;                             \n\
//out vec4 vColor;                                \n\
//                                                \n\
//void main(){                                    \n\
//  gl_Position = model * vec4(pos, 1.0);         \n\
//  vColor = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);  \n\
//}";
//
////Fragment Shader
//static const char* fShader = "                  \n\
//#version 330                                    \n\
//                                                \n\
//uniform vec3 triangleColor;                     \n\
//in vec4 vColor;                                 \n\
//out vec4 color;                                 \n\
//                                                \n\
//void main(){                                    \n\
//  color = vColor;                               \n\
//}";

void CreateTriagle() {
	//1. Definir os pontos dos vértices
	GLfloat vertices[] = {
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f
	};

	GLuint indices[] = {
		0, 1, 2,
		3, 4, 5,
		0, 1, 4,
		0, 3, 4,
		1, 2, 5,
		1, 4, 5,
		0, 2, 3,
		2, 3, 5,
		0, 2, 6,
		3, 5, 7,
		0, 6, 7,
		0, 3, 7,
		2, 6, 7,
		2, 5, 7
	};

	Mesh* triangulo1 = new Mesh();
	triangulo1->CreateMesh(vertices, sizeof(vertices),
						   indices, sizeof(indices));
	meshList.push_back(triangulo1);

	Mesh* triangulo2 = new Mesh();
	triangulo2->CreateMesh(vertices, sizeof(vertices),
		indices, sizeof(indices));
	meshList.push_back(triangulo2);
}

void CreateShader() {
	Shader* shader = new Shader();
	shader->CreateFromFile(vertexLocation, fragmentLocation);
	shaderList.push_back(shader);
}

//void AddShader(GLuint program, const char* shaderCode, GLenum shaderType) {
//	//Começamos a compilar cada shader
//	//1. Criar um shader
//	GLuint _shader = glCreateShader(shaderType);
//
//	//2. Atribui o código do GLSL para o shader
//		//2.1 Guarda a variavel localmente (converte char para GLchar*)
//	const GLchar* code[1];
//	code[0] = shaderCode;
//
//	//2.2 Anexando o código a shader
//	glShaderSource(_shader, 1, code, NULL); //1: numero de códigos | NULL: final da string onde encontrar NULL
//
//	//3. Compila o shader
//	glCompileShader(_shader); //compilar o shader
//
//	//4. Tratamento de erros de compilação
//	GLint result = 0;
//	GLchar eLog[1024] = { 0 };
//	glGetShaderiv(_shader, GL_COMPILE_STATUS, &result); //Verifica o resultado
//	if (!result) {
//		glGetProgramInfoLog(_shader, sizeof(eLog), NULL, eLog);
//		printf("Erro ao compilar o %d shader: '%s'\n", shaderType, eLog);
//		return;
//	}
//
//	//5. Adiciona o shader ao programa (recebido como parâmetro)
//	glAttachShader(program, _shader); //adiciona o shader ao programa
//}
//
//void CompileShaders() {
//	//1.0 Cria o programa
//	shaderProgram = glCreateProgram(); //Inicia o programa
//	if (!shaderProgram) {
//		printf("Erro ao criar o Shader Program");
//		return;
//	}
//
//	//2.0 Compila o Vertex Shader
//	AddShader(shaderProgram, vShader, GL_VERTEX_SHADER); //Adiciona o Vertex Shader ao programa
//	//3.0 Compila o Fragment Shader
//	AddShader(shaderProgram, fShader, GL_FRAGMENT_SHADER); //Adiciona o Vertex Shader ao programa
//
//	//4.0 Cria o link do programa
//	glLinkProgram(shaderProgram); //Criar o link do programa na GPU
//
//	//5.0 Tratamento de erros
//	GLint result = 0;
//	GLchar eLog[1024] = { 0 };
//	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result); //Verifica o resultado
//	if (!result) {
//		glGetProgramInfoLog(shaderProgram, sizeof(eLog), NULL, eLog);
//		printf("Erro ao linkar o programa: '%s'\n", eLog);
//		return;
//	}
//
//	//6.0 Validação se o programa está rodando
//	glValidateProgram(shaderProgram); //Validação do programa
//	glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &result); //Verifica o resultado
//	if (!result) {
//		glGetProgramInfoLog(shaderProgram, sizeof(eLog), NULL, eLog);
//		printf("Erro ao validar o programa: '%s'\n", eLog);
//		return;
//	}
//}

int main()
{

	window = new Window(1024, 768);
	window->Initialize();

	CreateTriagle();
	CreateShader();

	/*auto t_start = std::chrono::high_resolution_clock::now();

	if (!glfwInit()) {
		printf("erro no glfw");
		return 1;
	}*/

	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Computacao grafica", NULL, NULL);
	//if (!window) {
	//	printf("A janela nao pode ser criada");
	//	glfwTerminate();
	//	return 1;
	//}
	//
	//int bufferWidth, bufferHeight;
	//glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
	//glfwMakeContextCurrent(window);

	//glewExperimental = GL_TRUE;

	//if (glewInit() != GLEW_OK) {
	//	printf("Glew nao foi iniciado");
	//	glfwDestroyWindow(window);
	//	glfwTerminate();
	//	return 1;
	//}
	//glEnable(GL_DEPTH_TEST);

	//glViewport(0, 0, bufferWidth, bufferHeight);
	//
	////Criar o triangulo
	//CreateTriagle();
	//CompileShaders();

	//Variaveis para controle da movimentação do triangulo
	bool direction = true, sizeDirection = true, angleDirection = true;
	//true=direita e false=esquerda
	float triOffset = 0.0f, maxOffset = 0.7f, minOffset = -0.7f, incOffset = 0.01f;
	float size = 0.4f, maxSize = 0.7f, minSize = -0.7f, incSize = 0.01f;
	float angle = 0.0f, maxAngle = 360.0f, minAngle = -1.0f, incAngle = 0.5f;

	//Loop until the window close
	while (!window->ShouldClose()) {
		//Get + Handle user input events
		glfwPollEvents();

		//Clear Window (100% = 255)
		glClearColor(0.6f, 0.4f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		////Desenhar o triangulo
		//glUseProgram(shaderProgram); //Busca o programa que está com o shader (triangulo)

		//listMesh[0]->RenderMesh();

		//GLint uniColor = glGetUniformLocation(shaderProgram, "triangleColor"); //procura a entrada chamada triangleColor
		//float r = (float)rand()/RAND_MAX;
		//float g = (float)rand()/RAND_MAX;
		//float b = (float)rand()/RAND_MAX;
		//glUniform3f(uniColor, r, g, b); //atualiza o valor da entrada com a cor vermelha

		Shader* shader = shaderList[0];
		shader->UseProgram();


		/*
		* Mover nosso triangulo
		*/
		if (triOffset >= maxOffset || triOffset <= minOffset)
			direction = !direction;
		triOffset += direction ? incOffset : incOffset * -1;

		if (size >= maxSize || size <= minSize)
			sizeDirection = !sizeDirection;
		size += sizeDirection ? incSize : incSize * -1;

		if (angle >= maxAngle || angle <= minAngle)
			angleDirection = !angleDirection;
		angle += angleDirection ? incAngle : incAngle * -1;


		meshList[0]->RenderMesh();

		glm::mat4 model(1.0f);

		//Movimentações do triangulo
		//model = glm::translate(model, glm::vec3(triOffset, triOffset, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -0.25f, -1.5f));


		//Tamanho do triangulo
		model = glm::scale(model, glm::vec3(0.4, 0.4, 0.4));

		//Rotação
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 1.0f, 0.0f));

		/*GLuint uniModel = glGetUniformLocation(shaderProgram, "model");*/
		glUniformMatrix4fv(shader->GetUniformModel(), 1, GL_FALSE, glm::value_ptr(model));


		meshList[1]->RenderMesh();

		glm::mat4 model2(1.0f);

		//Movimentações do triangulo
		//model2 = glm::translate(model2, glm::vec3(-triOffset, -triOffset, 0.0f));
		model2 = glm::translate(model, glm::vec3(0.0f, -0.25f, -1.5f));

		//Tamanho do triangulo
		model2 = glm::scale(model2, glm::vec3(0.2, 0.2, 0.2));

		//Rotação
		model2 = glm::rotate(model2, glm::radians(angle), glm::vec3(1.0f, 1.0f, 1.0f));

		//GLuint uniModel2 = glGetUniformLocation(shaderProgram, "model");
		glUniformMatrix4fv(shader->GetUniformModel(), 1, GL_FALSE, glm::value_ptr(model2));

		glm::mat4 projection = glm::perspective(1.0f, window->GetBufferWidth() / window->GetBufferHeight(), 0.1f, 100.0f);
		glUniformMatrix4fv(shader->GetUniformProjection(), 1, GL_FALSE, glm::value_ptr(projection));



		//auto t_now = std::chrono::high_resolution_clock::now();
		//float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
		//glUniform3f(uniColor, (sin(time * 4.0f) + 1.0f) / 2.0f, 0.0f, 0.0f);

		////glUniform3f(uniColor, 1.0f, 0.0f, 0.0f); //atualiza o valor da entrada com a cor vermelha
		//glDrawArrays(GL_TRIANGLES, 0, 3); //Desenha um triangulo | 0: Array index | 2: Número de pontos (vértices)

		//glBindVertexArray(0);

		glUseProgram(0); //remove o programa da memória

		window->SwapBuffers();
	}

	//glfwDestroyWindow(window);
	//glfwTerminate();

	window->~Window();
	return 0;
	
	/* while (!glfwWindowShouldClose(window)) {
		//cria um evento
		glfwPollEvents();

		//Define a cor do fundo
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);

	} */
}