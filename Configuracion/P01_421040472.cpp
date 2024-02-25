
#include <stdio.h>
#include <glew.h>
#include <glfw3.h>
#include <windows.h>
#include <stdlib.h>     // Para rand() y srand()
#include <time.h>       // Para time()

// Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 600;
GLuint VAO, VBO, shaderProgram;

// Vertex Shader
static const char* vertexShaderSource = R"glsl(
#version 330 core
layout (location = 0) in vec3 position;
void main() {
    gl_Position = vec4(position.x, position.y, position.z, 1.0);
}
)glsl";

// Fragment Shader
static const char* fragmentShaderSource = R"glsl(
#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
)glsl";

void setupShaders() {
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void crearLetras() {
	GLfloat vertices[] = {
		// Letra D
		-0.8f, 0.0f, 0.0f,
		-0.7f, 0.0f, 0.0f,
		-0.8f, 0.8f, 0.0f,

		-0.7f, 0.8f, 0.0f,
		-0.8f, 0.8f, 0.0f,
		-0.7f, 0.0f, 0.0f,

		-0.7f, 0.8f, 0.0f,
		-0.6f, 0.4f, 0.0f,
		-0.7f, 0.65f, 0.0f,

		-0.7f, 0.0f, 0.0f,
		-0.6f, 0.4f, 0.0f,
		-0.7f, 0.15f, 0.0f,

		// Letra A 1
		// IZQ
		-0.5f,0.0f,0.0f,
		-0.45f,0.8f,0.0f,
		-0.45f,0.0f,0.0f,

		-0.45f, 0.0f, 0.0f,
		-0.45f,0.8f,0.0f,
		-0.4f,0.8f,0.0f,

		// DER
		-0.3f,0.0f,0.0f,
		-0.4f,0.8f,0.0f,
		-0.35f,0.0f,0.0f,

		-0.3f, 0.0f, 0.0f,
		-0.4f,0.8f,0.0f,
		-0.35f,0.8f,0.0f,

		// Transversal
		-0.45f,0.4f,0.0f,
		-0.45f,0.3f,0.0f,
		-0.35f,0.3f,0.0f,

		-0.45f,0.4f,0.0f,
		-0.35f,0.4f,0.0f,
		-0.35f,0.3f,0.0f,

		// Letra A 2
		// IZQ
		-0.2f,0.0f,0.0f,
		-0.15f,0.8f,0.0f,
		-0.15f,0.0f,0.0f,

		-0.15f, 0.0f, 0.0f,
		-0.15f,0.8f,0.0f,
		-0.1f,0.8f,0.0f,

		// DER
		-0.0f,0.0f,0.0f,
		-0.1f,0.8f,0.0f,
		-0.05f,0.0f,0.0f,

		-0.0f, 0.0f, 0.0f,
		-0.1f,0.8f,0.0f,
		-0.05f,0.8f,0.0f,

		// Transversal
		-0.15f,0.4f,0.0f,
		-0.15f,0.3f,0.0f,
		-0.05f,0.3f,0.0f,

		-0.15f,0.4f,0.0f,
		-0.05f,0.4f,0.0f,
		-0.05f,0.3f,0.0f,

		// Letra M
		// (reutilizando los triángulos de la letra A, pero moviéndolos en el eje X)
		0.1f, 0.0f, 0.0f,
		0.15f, 0.8f, 0.0f,
		0.15f, 0.0f, 0.0f,

		0.15f, 0.0f, 0.0f,
		0.15f, 0.8f, 0.0f,
		0.2f, 0.8f, 0.0f,

		0.3f, 0.0f, 0.0f,
		0.2f, 0.8f, 0.0f,
		0.25f, 0.0f, 0.0f,

		0.3f, 0.0f, 0.0f,
		0.2f, 0.8f, 0.0f,
		0.25f, 0.8f, 0.0f,

		0.25f, 0.0f, 0.0f,
		0.3f, 0.8f, 0.0f,
		0.3f, 0.0f, 0.0f,

		0.3f, 0.0f, 0.0f,
		0.3f, 0.8f, 0.0f,
		0.35f, 0.8f, 0.0f,

		0.45f, 0.0f, 0.0f,
		0.35f, 0.8f, 0.0f,
		0.4f, 0.0f, 0.0f,

		0.45f, 0.0f, 0.0f,
		0.35f, 0.8f, 0.0f,
		0.4f, 0.8f, 0.0f,

	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

int main() {
	if (!glfwInit()) {
		printf("Fallo en GLFW");
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "DAAM", NULL, NULL);
	if (window == NULL) {
		printf("No se pudo crear ventana");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		printf("Fallo en GLEW");
		return -1;
	}

	srand(time(NULL));
	double lastTime = glfwGetTime();
	double interval = 3.0;

	glViewport(0, 0, WIDTH, HEIGHT);
	setupShaders();
	crearLetras();

	while (!glfwWindowShouldClose(window)) {
		double currentTime = glfwGetTime();
		if (currentTime - lastTime >= interval) {
			// Cambiar el color de fondo aleatoriamente cada 3 segundos
			float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
			float g = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
			float b = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
			glClearColor(r, g, b, 1.0f);
			lastTime = currentTime;
		}
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 73);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
