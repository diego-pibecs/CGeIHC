#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	rotax = 0.0f;
	rotay = 0.0f;
	rotaz = 0.0f;

	/////////////////////////////////////////////////////////////////////////////////////////////////////// EJERCICIO 1
	//articulacion1 = 0.0f;
	//articulacion2 = -20.0f;
	//articulacion3 = 170.0f;
	//articulacion4 = 30.0f;
	//articulacion5 = 0.0f;
	//articulacion6 = 0.0f;
	//articulacion7 = 0.0f;
	//articulacion8 = 0.0f;
	//articulacion9 = 0.0f;
	//articulacion10 = 0.0f;
	//articulacion11 = 0.0f;
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////////////////////// EJERCICIO 2
	articulacion1 = 20.0f;
	articulacion2 = 20.0f;
	articulacion3 = 20.0f;
	articulacion4 = 20.0f;
	articulacion5 = -40.0f;
	articulacion6 = -40.0f;
	articulacion7 = -40.0f;
	articulacion8 = -40.0f;
	articulacion9 = 0.0f;
	articulacion10 = 0.0f;
	articulacion11 = 35.0f;
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Practica 4: Modelado Jerarquico", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
	// Asignar valores de la ventana y coordenadas

	//Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}

GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void avanzaPata(GLfloat* y, int index)
{
	//Variable de dirección para controlar si aumenta o disminuye
	static bool aumentando = true;

	if (index == 0)
	{
		if (aumentando) {
			// Si está aumentando y no ha alcanzado el límite superior, aumenta
			if (*y < 20.0) {
				*y += 5.0;
			}
			else {
				// Si alcanza el límite superior, cambia la dirección
				aumentando = false;
			}
		}
		else {
			// Si está disminuyendo y no ha alcanzado el límite inferior, disminuye
			if (*y > -25.0) {
				*y -= 5.0;
			}
			else {
				// Si alcanza el límite inferior, cambia la dirección
				aumentando = true;
			}
		}
	}
	else if (index == 1)
	{
		if (aumentando) {
			// Si está aumentando y no ha alcanzado el límite superior, aumenta
			if (*y < -40.0) {
				*y += 5.0;
			}
			else {
				// Si alcanza el límite superior, cambia la dirección
				aumentando = false;
			}
		}
		else {
			// Si está disminuyendo y no ha alcanzado el límite inferior, disminuye
			if (*y > -85.0) {
				*y -= 5.0;
			}
			else {
				// Si alcanza el límite inferior, cambia la dirección
				aumentando = true;
			}
		}
	}
}

void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}


	if (key == GLFW_KEY_E)
	{
		theWindow->rotax += 10.0;
	}
	if (key == GLFW_KEY_R)
	{
		theWindow->rotay += 10.0; //rotar sobre el eje y 10 grados
	}
	if (key == GLFW_KEY_T)
	{
		theWindow->rotaz += 10.0;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////// EJERCICIO 1
	//if (key == GLFW_KEY_F) { // Detecta si la tecla 'F' es presionada
	//	// Utiliza una variable estática 'aumentando' para alternar entre aumentar y disminuir
	//	static bool aumentando = true; // Inicialmente establecida para aumentar

	//	if (aumentando) { // Verifica si se está en modo de aumento
	//		// Aumenta 'articulacion1' si aún no alcanza el valor máximo de 20.0
	//		if (theWindow->articulacion1 < 20.0) { // Condición para seguir aumentando
	//			theWindow->articulacion1 += 5.0; // Aumenta 'articulacion1' en 5.0 unidades
	//		}
	//		else {
	//			// Cambia la dirección a disminuir si 'articulacion1' alcanza o supera 20.0
	//			aumentando = false; // Prepara para disminuir en el próximo evento
	//		}
	//	}
	//	else { // Verifica si se está en modo de disminución
	//		// Disminuye 'articulacion1' si aún no alcanza el valor mínimo de -40.0
	//		if (theWindow->articulacion1 > -40.0) { // Condición para seguir disminuyendo
	//			theWindow->articulacion1 -= 5.0; // Disminuye 'articulacion1' en 5.0 unidades
	//		}
	//		else {
	//			// Cambia la dirección a aumentar si 'articulacion1' alcanza o desciende de -40.0
	//			aumentando = true; // Prepara para aumentar en el próximo evento
	//		}
	//	}
	//}

	//if (key == GLFW_KEY_G) {
	//	static bool aumentando = true;

	//	if (aumentando) {
	//		if (theWindow->articulacion2 < 80.0) {
	//			theWindow->articulacion2 += 5.0;
	//		}
	//		else {
	//			aumentando = false;
	//		}
	//	}
	//	else {
	//		if (theWindow->articulacion2 > -30.0) {
	//			theWindow->articulacion2 -= 5.0;
	//		}
	//		else {
	//			aumentando = true;
	//		}
	//	}
	//}

	//if (key == GLFW_KEY_H) {
	//	static bool aumentando = true;

	//	if (aumentando) {
	//		if (theWindow->articulacion3 < 160.0) {
	//			theWindow->articulacion3 += 5.0;
	//		}
	//		else {
	//			aumentando = false;
	//		}
	//	}
	//	else {
	//		if (theWindow->articulacion3 > 80.0) {
	//			theWindow->articulacion3 -= 5.0;
	//		}
	//		else {
	//			aumentando = true;
	//		}
	//	}
	//}

	//if (key == GLFW_KEY_J) {
	//	static bool aumentando = true;

	//	if (aumentando) {
	//		if (theWindow->articulacion4 < 60.0) {
	//			theWindow->articulacion4 += 5.0;
	//		}
	//		else {
	//			aumentando = false;
	//		}
	//	}
	//	else {
	//		if (theWindow->articulacion4 > 20.0) {
	//			theWindow->articulacion4 -= 5.0;
	//		}
	//		else {
	//			aumentando = true;
	//		}
	//	}
	//}

	//if (key == GLFW_KEY_V) 	{
	//	theWindow->articulacion5 += 10.0;
	//} else if (key == GLFW_KEY_B) {
	//	theWindow->articulacion6 += 10.0;
	//} else if (key == GLFW_KEY_N) {
	//	theWindow->articulacion7 += 10.0;
	//} else if (key == GLFW_KEY_M) {
	//	theWindow->articulacion8 += 10.0;
	//}
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////////////////////// EJERCICIO 2

	switch (key)
		{
		case GLFW_KEY_F:
			avanzaPata(&theWindow->articulacion1, 0);
			break;
		case GLFW_KEY_G:
			avanzaPata(&theWindow->articulacion2, 0);
			break;
		case GLFW_KEY_H:
			avanzaPata(&theWindow->articulacion3, 0);
			break;
		case GLFW_KEY_J:
			avanzaPata(&theWindow->articulacion4, 0);
			break;
		case GLFW_KEY_V:
			avanzaPata(&theWindow->articulacion5, 1);
			break;
		case GLFW_KEY_B:
			avanzaPata(&theWindow->articulacion6, 1);
			break;
		case GLFW_KEY_N:
			avanzaPata(&theWindow->articulacion7, 1);
			break;
		case GLFW_KEY_M:
			avanzaPata(&theWindow->articulacion8, 1);
			break;
		case GLFW_KEY_O: {
			static bool aumentando = true;
			if (aumentando) {
				if (theWindow->articulacion9 < 0.0) {
					theWindow->articulacion9 += 5.0;
				}
				else {
					aumentando = false;
				}
			}
			else {
				if (theWindow->articulacion9 > -35.0) {
					theWindow->articulacion9 -= 5.0;
				}
				else {
					aumentando = true;
				}
			}
			break;
		}
		case GLFW_KEY_P: {
			static bool aumentando = false;
			if (aumentando) {
				if (theWindow->articulacion10 > 0.0) {
					theWindow->articulacion10 -= 5.0;
				}
				else {
					aumentando = false;
				}
			}
			else {
				if (theWindow->articulacion10 < 35.0) {
					theWindow->articulacion10 += 5.0;
				}
				else {
					aumentando = true;
				}
			}
			break;
		}
		case GLFW_KEY_L: {
			static bool aumentando = true;
			if (aumentando) {
				if (theWindow->articulacion11 < 35.0) {
					theWindow->articulacion11 += 5.0;
				}
				else {
					aumentando = false;
				}
			}
			else {
				if (theWindow->articulacion11 > -35.0) {
					theWindow->articulacion11 -= 5.0;
				}
				else {
					aumentando = true;
				}
			}
			break;
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////

	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_D, 0);
		//printf("se presiono la tecla: %s\n",key_name);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
