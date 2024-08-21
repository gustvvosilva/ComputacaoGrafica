#include <stdio.h>
#include <string.h>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <time.h>
#include <chrono>

const GLint WIDTH = 800, HEIGHT = 600;

GLuint VAO, VBO, shaderProgram;

//Vertex Shader
static const char* vShader = "                  \n\
#version 330                                    \n\
                                                \n\
layout(location=0) in vec2 pos;                 \n\
                                                \n\
void main(){                                    \n\
  gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);   \n\
}";

//Fragment Shader
static const char* fShader = "                  \n\
#version 330                                    \n\
                                                \n\
uniform vec3 triangleColor;                     \n\
                                                \n\
out vec4 color;                                 \n\
                                                \n\
void main(){                                    \n\
  color = vec4(triangleColor, 1.0);             \n\
}";

void CreateTriagle() {
	//1. Definir os pontos dos v�rtices
	GLfloat vertices[] = {
		-1.0f, -1.0f, //V�rtice 1 (x, y)
		1.0f, -1.0f,  //V�rtice 2 (x, y)
		0.0f, 1.0f,   //V�rtice 3 (x, y)
	};

	//VAO
	glGenVertexArrays(1, &VAO); //Gera um VAO ID
	glBindVertexArray(VAO); //Atribuindo o ID ao VAO

	//Carregar os dados de v�rtice para a placa de v�deo
	//Vertex Buffer Object: VBO
	glGenBuffers(1, &VBO); //Gera um VBO ID
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //Transforma o VBO em um Array Buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //Copia os dados ao VBO

	//GL_STATIC_DRAW: Os dados do v�rtice ser�o carregados uma vez e desenhados v�rias vezes (por exemplo, o mundo).
	//GL_DYNAMIC_DRAW: Os dados do v�rtice ser�o criados uma vez, alterados de tempos em tempos, mas desenhados muitas vezes mais do que isso.
	//GL_STREAM_DRAW : Os dados do v�rtice ser�o carregados uma vez e desenhados uma vez.

	//Vertex Attribute Pointer - Atributos dos dados na mem�ria
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0); //0: shader location | 2: numero de valores de vertice (x,y) | GL_FLOAT: tipo dos valores
	//GL_FALSE: normalizado | 0: pular elemento (cor) | 0: offset
//Vertex Attribute Pointer Enable
	glEnableVertexAttribArray(0); //0: shader location

	//Limpar o Buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Limpar o Vertex Array
	glBindVertexArray(0);
}

void AddShader(GLuint program, const char* shaderCode, GLenum shaderType) {
	//Come�amos a compilar cada shader
	//1. Criar um shader
	GLuint _shader = glCreateShader(shaderType);

	//2. Atribui o c�digo do GLSL para o shader
		//2.1 Guarda a variavel localmente (converte char para GLchar*)
	const GLchar* code[1];
	code[0] = shaderCode;

	//2.2 Anexando o c�digo a shader
	glShaderSource(_shader, 1, code, NULL); //1: numero de c�digos | NULL: final da string onde encontrar NULL

	//3. Compila o shader
	glCompileShader(_shader); //compilar o shader

	//4. Tratamento de erros de compila��o
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glGetShaderiv(_shader, GL_COMPILE_STATUS, &result); //Verifica o resultado
	if (!result) {
		glGetProgramInfoLog(_shader, sizeof(eLog), NULL, eLog);
		printf("Erro ao compilar o %d shader: '%s'\n", shaderType, eLog);
		return;
	}

	//5. Adiciona o shader ao programa (recebido como par�metro)
	glAttachShader(program, _shader); //adiciona o shader ao programa
}

void CompileShaders() {
	//1.0 Cria o programa
	shaderProgram = glCreateProgram(); //Inicia o programa
	if (!shaderProgram) {
		printf("Erro ao criar o Shader Program");
		return;
	}

	//2.0 Compila o Vertex Shader
	AddShader(shaderProgram, vShader, GL_VERTEX_SHADER); //Adiciona o Vertex Shader ao programa
	//3.0 Compila o Fragment Shader
	AddShader(shaderProgram, fShader, GL_FRAGMENT_SHADER); //Adiciona o Vertex Shader ao programa

	//4.0 Cria o link do programa
	glLinkProgram(shaderProgram); //Criar o link do programa na GPU

	//5.0 Tratamento de erros
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result); //Verifica o resultado
	if (!result) {
		glGetProgramInfoLog(shaderProgram, sizeof(eLog), NULL, eLog);
		printf("Erro ao linkar o programa: '%s'\n", eLog);
		return;
	}

	//6.0 Valida��o se o programa est� rodando
	glValidateProgram(shaderProgram); //Valida��o do programa
	glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &result); //Verifica o resultado
	if (!result) {
		glGetProgramInfoLog(shaderProgram, sizeof(eLog), NULL, eLog);
		printf("Erro ao validar o programa: '%s'\n", eLog);
		return;
	}
}

int main()
{
	auto t_start = std::chrono::high_resolution_clock::now();

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
	
	//Criar o triangulo
	CreateTriagle();
	CompileShaders();

	//Loop until the window close
	while (!glfwWindowShouldClose(window)) {
		//Get + Handle user input events
		glfwPollEvents();

		//Clear Window (100% = 255)
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Desenhar o triangulo
		glUseProgram(shaderProgram); //Busca o programa que est� com o shader (triangulo)
		glBindVertexArray(VAO); //Bind o VAO
		GLint uniColor = glGetUniformLocation(shaderProgram, "triangleColor"); //procura a entrada chamada triangleColor
		float r = (float)rand()/RAND_MAX;
		float g = (float)rand()/RAND_MAX;
		float b = (float)rand()/RAND_MAX;
		glUniform3f(uniColor, r, g, b); //atualiza o valor da entrada com a cor vermelha

		auto t_now = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
		glUniform3f(uniColor, (sin(time * 4.0f) + 1.0f) / 2.0f, 0.0f, 0.0f);

		//glUniform3f(uniColor, 1.0f, 0.0f, 0.0f); //atualiza o valor da entrada com a cor vermelha
		glDrawArrays(GL_TRIANGLES, 0, 3); //Desenha um triangulo | 0: Array index | 2: N�mero de pontos (v�rtices)
		glBindVertexArray(0);
		glUseProgram(0); //remove o programa da mem�ria

		glfwSwapBuffers(window);
	}
	
	/* while (!glfwWindowShouldClose(window)) {
		//cria um evento
		glfwPollEvents();

		//Define a cor do fundo
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);

	} */
}