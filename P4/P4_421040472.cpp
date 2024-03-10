/*Pr�ctica 4: Modelado Jer�rquico.
Se implementa el uso de matrices adicionales para almacenar informaci�n de transformaciones geom�tricas que se quiere
heredar entre diversas instancias para que est�n unidas.
Teclas de la F a la K para rotaciones de articulaciones.
*/

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <glew.h>
#include <glfw3.h>

// GLM
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include <gtc\random.hpp>

// Clases para dar orden y limpieza al c�digo
#include "Mesh.h"
#include "Shader.h"
#include "Sphere.h"
#include "Window.h"
#include "Camera.h"

using std::vector;

// Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0f; // Grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader> shaderList;

// Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";

Sphere sp = Sphere(1.0, 20, 20); // Recibe radio, slices, stacks

void CrearCubo() {
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2, 2, 3, 0,
		// right
		1, 5, 6, 6, 2, 1,
		// back
		7, 6, 5, 5, 4, 7,
		// left
		4, 0, 3, 3, 7, 4,
		// bottom
		4, 5, 1, 1, 0, 4,
		// top
		3, 2, 6, 6, 7, 3
	};

	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f
	};

	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

void CrearPiramideTriangular() {
	unsigned int indices_piramide_triangular[] = {
		0, 1, 2, 1, 3, 2, 3, 0, 2, 1, 0, 3
	};

	GLfloat vertices_piramide_triangular[] = {
		-0.5f, -0.5f, 0.0f,  //0
		0.5f, -0.5f, 0.0f,   //1
		0.0f, 0.5f, -0.25f,  //2
		0.0f, -0.5f, -0.5f,  //3
	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);
}

void CrearCilindro(int res, float R) {
	int n, i;
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;
	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	for (n = 0; n <= res; n++) {
		x = R * cos(n * dt);
		z = R * sin(n * dt);
		if (n == res) {
			x = R * cos(0);
			z = R * sin(0);
		}
		for (i = 0; i < 6; i++) {
			switch (i) {
			case 0: case 3: vertices.push_back(x); break;
			case 1: vertices.push_back(y); break;
			case 2: case 5: vertices.push_back(z); break;
			case 4: vertices.push_back(0.5); break;
			}
		}
	}

	for (n = 0; n <= res; n++) {
		x = R * cos(n * dt);
		z = R * sin(n * dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0: vertices.push_back(x); break;
			case 1: vertices.push_back(-0.5f); break;
			case 2: vertices.push_back(z); break;
			}
		}
	}

	for (n = 0; n <= res; n++) {
		x = R * cos(n * dt);
		z = R * sin(n * dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0: vertices.push_back(x); break;
			case 1: vertices.push_back(0.5); break;
			case 2: vertices.push_back(z); break;
			}
		}
	}

	for (i = 0; i < vertices.size(); i++) indices.push_back(i);

	Mesh* cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}

void CrearCono(int res, float R) {
	int n, i;
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;
	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	vertices.push_back(0.0);
	vertices.push_back(0.5);
	vertices.push_back(0.0);

	for (n = 0; n <= res; n++) {
		x = R * cos(n * dt);
		z = R * sin(n * dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0: vertices.push_back(x); break;
			case 1: vertices.push_back(y); break;
			case 2: vertices.push_back(z); break;
			}
		}
	}

	vertices.push_back(R * cos(0) * dt);
	vertices.push_back(-0.5);
	vertices.push_back(R * sin(0) * dt);

	for (i = 0; i < res + 2; i++) indices.push_back(i);

	Mesh* cono = new Mesh();
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
	meshList.push_back(cono);
}

void CrearPiramideCuadrangular()
{
	vector<unsigned int> piramidecuadrangular_indices = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		0,2,4

	};
	vector<GLfloat> piramidecuadrangular_vertices = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh* piramide = new Mesh();
	piramide->CreateMeshGeometry(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}

void CreateShaders() {
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main() {
	mainWindow = Window(800, 600);
	mainWindow.Initialise();

	//Cilindro y cono reciben resoluci�n (slices, rebanadas) y Radio de circunferencia de la base y tapa
	CrearCubo();					// �ndice 0 en MeshList
	CrearPiramideTriangular();		// �ndice 1 en MeshList
	CrearCilindro(5, 1.0f);			// �ndice 2 en MeshList	// Cabina
	CrearCilindro(10, 1.0f);		// �ndice 3 en MeshList	// Llanta
	CrearCono(25, 2.0f);			// �ndice 4 en MeshList
	CrearPiramideCuadrangular();	// �ndice 5 en MeshList
	CrearCilindro(250, 1.0f);		// �ndice 6 en MeshList	// Para perro
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.2f, 0.2f);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	sp.init();	// Inicializar esfera
	sp.load();	// Enviar la esfera al shader

	glm::mat4 model(1.0);//Inicializar matriz de Modelo 4x4
	glm::mat4 modelaux(1.0);//Inicializar matriz de Modelo 4x4 auxiliar para Grua y Perro
	glm::mat4 modelauxbase(1.0);//Inicializar matriz de Modelo 4x4 auxiliar Grua
	glm::mat4 modelauxtronco(1.0);//Inicializar matriz de Modelo 4x4 auxiliar Perro
	glm::mat4 modelauxcabeza(1.0);//Inicializar matriz de Modelo 4x4 auxiliar Perro

	glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f); //inicializar Color para enviar a variable Uniform;
	glm::vec3 colorSecundario= glm::vec3(0.0f, 0.0f, 0.0f); //inicializar Color Perro
	glm::vec3 colorPrincipal = glm::vec3(0.0f, 0.0f, 0.0f); //inicializar Color Perro
	glm::vec3 colorArticulacion = glm::vec3(0.0f, 0.0f, 0.0f); //inicializar Color Perro
	glm::vec3 colorVisor = glm::vec3(0.0f, 0.0f, 0.0f); //inicializar Color Perro

	while (!mainWindow.getShouldClose()) {
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		// Configuraci�n inicial para la proyecci�n y vista de la c�mara.
		// Establece la proyecci�n (perspectiva u ortogonal) y la vista de la c�mara una sola vez, a menos que se requiera cambiar durante la ejecuci�n.
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		// Resetea la matriz de modelo a la identidad para iniciar la transformaci�n desde el principio.
		model = glm::mat4(1.0);

		/////////////////////////////////////////////////////////////////////////////////////////////////////// EJERCICIO 1
		
		//// Comienzo de la construcci�n de la gr�a: Dibujo de la cabina, base y llantas.
		//// Configura y dibuja la cabina de la gr�a.
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f)); // Posiciona la cabina.
		//modelaux = model; // Almacena la ubicaci�n de la cabina para aplicarle transformaciones.
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // Rota la cabina.
		//model = glm::scale(model, glm::vec3(2.0f, 6.0f, 5.0f)); // Escala la cabina.
		//color = glm::vec3(1.0f, 0.0f, 0.0f); // Establece el color de la cabina.
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//meshList[2]->RenderMeshGeometry(); // Renderiza la cabina.
		//model = modelaux; // Regresa a la posici�n inicial de la cabina.

		//// Piramide cuadrangular para la base de la gr�a
		//model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		//modelauxbase = model; // Almacena la ubicaci�n de la base para aplicarle transformaciones.
		//model = glm::scale(model, glm::vec3(8.0f, 3.0f, 5.0f)); // Rota la base.
		//color = glm::vec3(0.678f, 0.678f, 0.678f); // Establece el color de la base.
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//meshList[5]->RenderMeshGeometry(); // Renderiza la base.
		//model = modelauxbase; // Regresa a la posici�n inicial de la base.

		//
		//color = glm::vec3(0.0f, 0.0f, 0.0f); // Establece el color de las llantas.
		//
		//// Llanta 1: Izquierda delantera
		//model = glm::translate(model, glm::vec3(-4.1f, -1.4f, 3.65f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Rota la llanta.
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(2.25f, 2.25f, 2.25f));
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//meshList[3]->RenderMeshGeometry(); // Renderiza la llanta 1.
		//model = modelauxbase; // Regresa a la posici�n inicial de la base.

		//// Llanta 2: Derecha delantera
		//model = glm::translate(model, glm::vec3(-4.1f, -1.4f, -3.65f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Rota la llanta.
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion6()), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(2.25f, 2.25f, 2.25f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//meshList[3]->RenderMeshGeometry(); // Renderiza la llanta 2.
		//model = modelauxbase; // Regresa a la posici�n inicial de la base.

		//// Llanta 3: Izquierda trasera
		//model = glm::translate(model, glm::vec3(4.1f, -1.4f, 3.65f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Rota la llanta.
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion7()), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(2.25f, 2.25f, 2.25f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//meshList[3]->RenderMeshGeometry(); // Renderiza la llanta 3.
		//model = modelauxbase; // Regresa a la posici�n inicial de la base.

		//// Llanta 4: Derecha trasera
		//model = glm::translate(model, glm::vec3(4.1f, -1.4f, -3.65f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Rota la llanta.
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion8()), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(2.25f, 2.25f, 2.25f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//meshList[3]->RenderMeshGeometry(); // Renderiza la llanta 4.
		//model = modelaux; // Regresa a la posici�n inicial de la cabina.

		//// Dibujo y configuraci�n de los brazos de la gr�a.
		//// Articulaci�n 1: punto de rotaci�n que une el primer brazo con la cabina.
		//model = glm::translate(model, glm::vec3(-0.5f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f));

		//// Primer brazo: conecta con la cabina.
		//model = glm::translate(model, glm::vec3(-1.0f, 2.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//modelaux = model; // Almacena la ubicaci�n de la primera articulaci�n.
		//model = glm::scale(model, glm::vec3(5.0f, 1.0f, 1.1f)); // Escala el primer brazo.
		//color = glm::vec3(0.0f, 1.0f, 0.0f); // Establece el color del primer brazo.
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); // Cambia el color.
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); // Aplica transformaciones.
		//meshList[0]->RenderMesh(); // Renderiza el primer brazo.
		//model = modelaux; // Regresa a la ubicaci�n de la primera articulaci�n.

		//// Articulaci�n 2: Ubicaci�n en el extremo izquierdo del segundo brazo.
		//model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));

		//// Guardamos la posici�n actual para la articulaci�n 2.
		//modelaux = model;
		//// Dibujamos la esfera en la articulaci�n entre el primer y segundo brazo.
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//sp.render(); // Renderiza la esfera.
		//model = modelaux; // Restauramos la posici�n de la articulaci�n 2.

		//// Construcci�n del segundo brazo.
		//model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f));
		//// Configuraci�n y renderizaci�n del segundo brazo.
		//modelaux = model; // Guardamos la posici�n inicial del segundo brazo.
		//model = glm::scale(model, glm::vec3(1.0f, 5.0f, 1.1f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.0f, 0.0f, 1.0f); // Color para el segundo brazo: Verde.
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); // Aplicamos el color.
		//meshList[0]->RenderMesh(); // Renderiza el segundo brazo.
		//model = modelaux; // Restauramos la posici�n inicial para la siguiente articulaci�n.

		//// Articulaci�n 3: Ubicaci�n en el extremo derecho del segundo brazo.
		//model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
		//modelaux = model; // Guardamos la posici�n de la articulaci�n 3.
		//// Dibujamos la esfera en la articulaci�n entre el segundo y tercer brazo.
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//sp.render(); // Renderiza la esfera.
		//model = modelaux; // Restauramos la posici�n de la articulaci�n 3 para el tercer brazo.

		//// Construcci�n y configuraci�n del tercer brazo.
		//model = glm::translate(model, glm::vec3(-2.0f, -2.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//modelaux = model; // Guardamos la posici�n inicial del tercer brazo.
		//model = glm::scale(model, glm::vec3(1.0f, 5.0f, 1.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(0.5f, 0.5f, 0.5f); // Color para el tercer brazo
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); // Aplicamos el color.
		//meshList[0]->RenderMesh(); // Renderiza el tercer brazo.
		//model = modelaux; // Restauramos la posici�n para la siguiente articulaci�n.

		//// Articulaci�n 4: Extremo derecho del tercer brazo, preludio a la canasta.
		//model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 0.0f, 1.0f));
		//modelaux = model; // Guardamos la posici�n inicial de la articulaci�n 4.
		//// Dibujamos la esfera en la articulaci�n entre el tercer brazo y la canasta.
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//sp.render(); // Renderiza la esfera.
		//model = modelaux; // Restauramos la posici�n para construir la canasta.

		//// Construcci�n y configuraci�n de la canasta.
		//model = glm::translate(model, glm::vec3(0.0f, 0.75f, 0.0f));
		//model = glm::scale(model, glm::vec3(3.0f, 2.0f, 1.5f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(1.0f, 1.0f, 0.0f); // Color para la canasta
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); // Aplicamos el color.
		//meshList[0]->RenderMesh(); // Renderiza la canasta.

		///////////////////////////////////////////////////////////////////////////////////////////////////////


		/////////////////////////////////////////////////////////////////////////////////////////////////////// EJERCICIO 2

		// Definici�n de colores en formato RGB
		colorPrincipal = glm::vec3(0.67f, 0.75f, 0.77f);
		colorSecundario = glm::vec3(0.0f, 0.0f, 0.0f); // Color de las orejas en negro
		colorArticulacion = glm::vec3(0.678f, 0.678f, 0.678f); // Color de las articulaciones en gris
		colorVisor = glm::vec3(1.0f, 1.0f, 0.0f); // Color del visor en amarillo

		//*********************************************************************************
		// Construcci�n del Tronco
		//*********************************************************************************
		// Traslada el modelo al punto de inicio del tronco
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -8.0f));
		modelauxtronco = model; // Almacena la ubicaci�n inicial del tronco para futuras referencias
		modelaux = model;

		// Aplica rotaci�n y escala para formar el tronco
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(2.5f, 9.0f, 2.5f));

		// Establece el color principal y renderiza el cilindro para el tronco
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPrincipal));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[6]->RenderMeshGeometry(); // Renderiza el tronco como un cilindro

		// Restaura la posici�n inicial del tronco para comenzar con la articulaci�n frontal izquierda
		model = modelaux;

		// Traslada el modelo al punto de inicio del tronco
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		modelauxtronco = model; // Almacena la ubicaci�n inicial del tronco para futuras referencias
		modelaux = model;

		// Aplica rotaci�n y escala para formar el tronco
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(2.5f, 9.0f, 2.5f));

		// Establece el color principal y renderiza el cilindro para el tronco
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPrincipal));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[6]->RenderMeshGeometry(); // Renderiza el tronco como un cilindro
		model = modelaux;

		//***********************************************************************************
		// Pata completa frontal izquierda
		//***********************************************************************************
		// Articulaci�n 1 (Delantera Izquierda)
		// Traslada y rota la pata seg�n la articulaci�n 1, guardando la ubicaci�n para modificaciones posteriores
		model = glm::translate(model, glm::vec3(-2.5f, 0.0f, 2.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model; // Almacena la ubicaci�n de la articulaci�n 1

		// Primera esfera: Uni�n entre tronco y pata 1
		// Escala la esfera para formar la articulaci�n y establece el color de articulaci�n
		model = glm::scale(model, glm::vec3(1.25f, 1.25f, 1.25f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorArticulacion));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render(); // Renderiza la esfera de la articulaci�n
		model = modelaux; // Restaura la ubicaci�n de la articulaci�n 1

		// Pata 1 (Segmento superior de la pata frontal izquierda)
		// Traslada y escala la pata 1, luego establece el color de las patas y renderiza
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		modelaux = model; // Almacena la ubicaci�n del segmento superior de la pata
		model = glm::scale(model, glm::vec3(1.5f, 2.5f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorSecundario));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh(); // Renderiza el segmento superior de la pata
		model = modelaux; // Restaura la ubicaci�n del segmento superior de la pata

		// Articulaci�n 5 (Delantera Izquierda - Inferior)
		// Traslada y rota la articulaci�n 5, preparando para la esfera de uni�n con el segmento inferior de la pata
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model; // Almacena la ubicaci�n de la articulaci�n 5

		// Segunda esfera: Uni�n entre segmento superior e inferior de la pata
		// Escala la esfera para la articulaci�n y establece el color de articulaci�n
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorArticulacion));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render(); // Renderiza la esfera de la articulaci�n
		model = modelaux; // Restaura la ubicaci�n de la articulaci�n 5

		// Pata 5 (Segmento inferior de la pata frontal izquierda)
		// Traslada y escala el segmento inferior, establece el color de las patas y renderiza
		model = glm::translate(model, glm::vec3(0.0f, -1.6f, 0.0f));
		modelaux = model; // Almacena la ubicaci�n del segmento inferior de la pata
		model = glm::scale(model, glm::vec3(1.5f, 2.25f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorSecundario));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh(); // Renderiza el segmento inferior de la pata
		model = modelaux; // Restaura la ubicaci�n del segmento inferior

		// Soporte Final (Base de la pata frontal izquierda)
		// Traslada, escala y rota para formar el soporte, luego establece el color de articulaci�n y renderiza
		model = glm::translate(model, glm::vec3(-0.25f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, glm::radians(20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorArticulacion));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMeshGeometry(); // Renderiza el soporte como un tri�ngulo cuadrangular
		model = modelauxtronco; // Restaura la posici�n inicial del tronco para continuar con la siguiente articulaci�n


		//*********************************************************************************
		// Pata completa frontal derecha
		//*********************************************************************************
		// Articulaci�n 2 (Delantera Derecha)
		// Posicionamiento y rotaci�n de la articulaci�n 2, almacenando la ubicaci�n para ajustes futuros
		model = glm::translate(model, glm::vec3(-2.5f, 0.0f, -2.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model; // Almacena la posici�n de la articulaci�n 2

		// Primera esfera: Uni�n entre tronco y pata 2
		// Ajuste de escala para la articulaci�n, aplicaci�n del color de articulaciones
		model = glm::scale(model, glm::vec3(1.25f, 1.25f, 1.25f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorArticulacion));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render(); // Renderizaci�n de la esfera de la articulaci�n
		model = modelaux; // Restablecimiento de la posici�n para la pata 2

		// Pata 2 (Segmento superior de la pata frontal derecha)
		// Desplazamiento y escalamiento del segmento superior, asignaci�n del color de patas, y renderizado
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		modelaux = model; // Guarda la ubicaci�n del segmento superior
		model = glm::scale(model, glm::vec3(1.5f, 2.5f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorSecundario));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh(); // Renderizaci�n del segmento superior de la pata
		model = modelaux; // Restablece la ubicaci�n del segmento superior

		// Articulaci�n 6 (Delantera Derecha - Inferior)
		// Configuraci�n de la articulaci�n 6 con desplazamiento y rotaci�n, preparativos para la esfera de uni�n
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion6()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model; // Guarda la posici�n de la articulaci�n 6

		// Segunda esfera: Conexi�n entre segmentos superior e inferior
		// Escalamiento de la esfera para la articulaci�n y aplicaci�n del color
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorArticulacion));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render(); // Renderizaci�n de la esfera de la articulaci�n
		model = modelaux; // Restablece la posici�n de la articulaci�n 6

		// Pata 6 (Segmento inferior de la pata frontal derecha)
		// Desplazamiento y escalamiento para el segmento inferior, coloraci�n y renderizado
		model = glm::translate(model, glm::vec3(0.0f, -1.6f, 0.0f));
		modelaux = model; // Almacena la ubicaci�n del segmento inferior
		model = glm::scale(model, glm::vec3(1.5f, 2.25f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorSecundario));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh(); // Renderiza el segmento inferior de la pata
		model = modelaux; // Restaura la ubicaci�n inicial del segmento inferior

		// Soporte Final (Base de la pata frontal derecha)
		// Ajuste final con traslaci�n, escalado y rotaci�n para crear el soporte, asignaci�n del color y renderizado
		model = glm::translate(model, glm::vec3(0.25f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, glm::radians(20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorArticulacion));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMeshGeometry(); // Renderiza el soporte como un tri�ngulo cuadrangular
		model = modelauxtronco; // Restablece la posici�n inicial del tronco para avanzar a la siguiente articulaci�n


		//***********************************************************************************
		// Pata completa trasera izquierda
		//***********************************************************************************
		// Articulaci�n 3 (Trasera Izquierda)
		// Posiciona y rota la articulaci�n 3, guarda la posici�n para ajustes posteriores
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 2.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model; // Almacena la posici�n de la articulaci�n 3

		// Tercera esfera: Uni�n tronco y pata 3
		// Ajusta escala para la articulaci�n, aplica color y renderiza la esfera
		model = glm::scale(model, glm::vec3(1.25f, 1.25f, 1.25f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorArticulacion));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render(); // Renderiza la esfera de articulaci�n
		model = modelaux; // Restaura posici�n para la pata 3

		// Pata 3 (Segmento superior trasero izquierdo)
		// Desplaza, escala el segmento superior, aplica color y renderiza
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		modelaux = model; // Guarda ubicaci�n del segmento superior
		model = glm::scale(model, glm::vec3(1.5f, 2.5f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorSecundario));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh(); // Renderiza segmento superior de la pata
		model = modelaux; // Restablece ubicaci�n del segmento superior

		// Articulaci�n 7 (Trasera Izquierda - Inferior)
		// Configura desplazamiento y rotaci�n para la articulaci�n 7, prepara para uni�n
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion7()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model; // Almacena posici�n de la articulaci�n 7

		// Tercera esfera: Uni�n entre segmentos superior e inferior
		// Escala esfera, aplica color y renderiza
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorArticulacion));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render(); // Renderiza esfera de articulaci�n
		model = modelaux; // Restablece posici�n de la articulaci�n 7

		// Pata 7 (Segmento inferior trasero izquierdo)
		// Desplaza, escala segmento inferior, aplica color y renderiza
		model = glm::translate(model, glm::vec3(0.0f, -1.6f, 0.0f));
		modelaux = model; // Guarda ubicaci�n del segmento inferior
		model = glm::scale(model, glm::vec3(1.5f, 2.25f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorSecundario));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh(); // Renderiza segmento inferior de la pata
		model = modelaux; // Restablece ubicaci�n inicial del segmento inferior

		// Soporte Final (Base de la pata trasera izquierda)
		// Ajusta traslaci�n, escalado y rotaci�n para el soporte, establece color y renderiza
		model = glm::translate(model, glm::vec3(-0.25f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, glm::radians(20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorArticulacion));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMeshGeometry(); // Renderiza el soporte como un tri�ngulo cuadrangular
		model = modelauxtronco; // Restaura posici�n del tronco para la siguiente articulaci�n


		//***********************************************************************************
		// Pata completa trasera derecha
		//***********************************************************************************
		// Articulaci�n 4 (Trasera Derecha)
		// Configura la posici�n y rotaci�n de la articulaci�n 4, almacena la posici�n para ajustes futuros
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, -2.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model; // Guarda la posici�n de la articulaci�n 4

		// Cuarta esfera: Uni�n entre tronco y pata 4
		// Ajusta la escala para la articulaci�n, aplica el color de las articulaciones y renderiza la esfera
		model = glm::scale(model, glm::vec3(1.25f, 1.25f, 1.25f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorArticulacion));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render(); // Renderiza la esfera de la articulaci�n
		model = modelaux; // Restablece la posici�n para trabajar en la pata 4

		// Pata 4 (Segmento superior trasero derecho)
		// Desplaza y escala el segmento superior, aplica el color de las patas y renderiza
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		modelaux = model; // Almacena la ubicaci�n del segmento superior
		model = glm::scale(model, glm::vec3(1.5f, 2.5f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorSecundario));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh(); // Renderiza el segmento superior de la pata
		model = modelaux; // Restaura la posici�n del segmento superior

		// Articulaci�n 8 (Trasera Derecha - Inferior)
		// Prepara la articulaci�n 8 con desplazamiento y rotaci�n, lista para la uni�n
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion8()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model; // Guarda la posici�n de la articulaci�n 8

		// Cuarta esfera: Conexi�n entre segmentos superior e inferior
		// Escala la esfera para la articulaci�n, aplica color y renderiza
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorArticulacion));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render(); // Renderiza la esfera de articulaci�n
		model = modelaux; // Restablece la posici�n de la articulaci�n 8

		// Pata 8 (Segmento inferior trasero derecho)
		// Desplaza y escala el segmento inferior, aplica el color de las patas y renderiza
		model = glm::translate(model, glm::vec3(0.0f, -1.6f, 0.0f));
		modelaux = model; // Guarda la ubicaci�n del segmento inferior
		model = glm::scale(model, glm::vec3(1.5f, 2.25f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorSecundario));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh(); // Renderiza el segmento inferior de la pata
		model = modelaux; // Restablece la posici�n inicial del segmento inferior

		// Soporte Final (Base de la pata trasera derecha)
		// Realiza ajustes finales con traslaci�n, escalado y rotaci�n para crear el soporte, aplica color y renderiza
		model = glm::translate(model, glm::vec3(0.25f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, glm::radians(20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorArticulacion));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMeshGeometry(); // Renderiza el soporte como un tri�ngulo cuadrangular
		model = modelauxtronco; // Restaura la posici�n del tronco para avanzar a la siguiente articulaci�n


		//********************************************************
		// Cola
		//********************************************************
		// Articulaci�n 11 (Cola)
		// Posiciona y rota la cola seg�n la articulaci�n 11, guardando la posici�n para ajustes posteriores
		model = glm::translate(model, glm::vec3(4.75f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion11()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model; // Guarda la ubicaci�n de la articulaci�n de la cola

		// Quinta esfera: Base de la cola
		// Ajusta la escala para formar la base, aplica el color de las articulaciones y renderiza la esfera
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorArticulacion));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render(); // Renderiza la esfera de la base de la cola
		model = modelaux; // Restaura la posici�n para la cola

		// Extiende la cola desde la base, aplica el color principal y renderiza
		model = glm::translate(model, glm::vec3(1.75f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.25f, 0.5f, 0.5f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorSecundario));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh(); // Renderiza la cola como un cubo alargado
		model = modelauxtronco; // Restaura la posici�n inicial del tronco


		//********************************************************
		// Cabeza
		//********************************************************
		// Configura la ubicaci�n y dimensiones de la cabeza, aplica el color principal y renderiza
		model = glm::translate(model, glm::vec3(-4.0f, 3.0f, 0.0f));
		modelauxcabeza = model; // Guarda la ubicaci�n inicial de la cabeza
		model = glm::scale(model, glm::vec3(2.0f, 6.0f, 1.5f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPrincipal));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[6]->RenderMeshGeometry(); // Renderiza el cilindro como cabeza
		model = modelauxcabeza; // Restaura la posici�n de la cabeza

		//***********************************************************************************
		// Orejas
		//***********************************************************************************
		// Oreja Izquierda (Articulaci�n 9)
		// Posiciona y rota la oreja izquierda, ajusta escala, aplica color y renderiza
		model = glm::translate(model, glm::vec3(0.8f, 2.75f, 1.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion9()), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model; // Guarda la posici�n de la oreja izquierda
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorArticulacion));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render(); // Renderiza la base de la oreja
		model = modelaux; // Restaura la posici�n de la oreja izquierda

		// Oreja Izquierda - Extensi�n
		model = glm::translate(model, glm::vec3(0.0f, -0.75f, 0.25f));
		model = glm::scale(model, glm::vec3(1.5f, 2.5f, 0.1f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorSecundario));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh(); // Renderiza la extensi�n de la oreja
		model = modelauxcabeza;

		// Oreja Derecha (Articulaci�n 10)
		// Configura la posici�n y rotaci�n para la articulaci�n 10 de la oreja derecha, almacenando esta posici�n
		model = glm::translate(model, glm::vec3(0.8f, 2.75f, -1.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion10()), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model; // Guarda la ubicaci�n de la articulaci�n de la oreja derecha

		// Base de la Oreja Derecha
		// Ajusta la escala para la base de la oreja, aplica el color de las articulaciones y renderiza
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorArticulacion));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render(); // Renderiza la base de la oreja
		model = modelaux; // Restablece la posici�n de la articulaci�n para la extensi�n de la oreja

		// Extensi�n de la Oreja Derecha
		// Desplaza, escala y aplica el color para la extensi�n de la oreja derecha, luego renderiza
		model = glm::translate(model, glm::vec3(0.0f, -0.75f, -0.25f));
		model = glm::scale(model, glm::vec3(1.5f, 2.5f, 0.1f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorSecundario));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh(); // Renderiza la extensi�n de la oreja
		model = modelauxcabeza; // Restaura la posici�n inicial de la cabeza para continuar con el hocico

		//***********************************************************************************
		// Hocico
		//***********************************************************************************
		// Configura la posici�n y dimensiones del hocico, aplica el color de las articulaciones y renderiza
		model = glm::translate(model, glm::vec3(-2.0f, 0.75f, 0.0f));
		model = glm::scale(model, glm::vec3(1.25f, 1.3f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorSecundario));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[6]->RenderMeshGeometry(); // Renderiza el cilindro como hocico
		model = modelauxcabeza; // Restaura la ubicaci�n inicial de la cabeza para agregar el visor

		//***********************************************************************************
		// Visor
		//***********************************************************************************
		// Configura la posici�n y dimensiones del visor, aplica el color del visor y renderiza
		model = glm::translate(model, glm::vec3(-0.4f, 2.25f, 0.0f));
		model = glm::scale(model, glm::vec3(1.95f, 1.0f, 1.25f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorVisor));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[6]->RenderMeshGeometry(); // Renderiza el cilindro como visor
		model = modelauxcabeza; // Restaura la ubicaci�n inicial de la cabeza

		//***********************************************************************************
		// Cabeza (Detalle final en la parte superior de la cabeza)
		//***********************************************************************************
		// Configura la posici�n y dimensiones del tap�n, aplica el color principal y renderiza
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.95f, 1.0f, 1.55f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPrincipal));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render(); // Renderiza el detalle final de la cabeza

		///////////////////////////////////////////////////////////////////////////////////////////////////////

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}