/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "allmodels.h"
#include "lodepng.h"
#include "shaderprogram.h"

// assimp imports
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/mesh.h>
#include <string>



float speed = 0; //Prędkość kątowa obrotu obiektu
float skret = 0; //skret kół
glm::vec3 cameraPos = glm::vec3(0.0f, 2.0f, -7.0f);
//glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

float pcpz = 0.0f;
float pcpx = 0.0f;
float pcfy = 0.0f;
float pcfx = 0.0f;

float mcpz = 0.0f;
float mcpx = 0.0f;
float mcfy = 0.0f;
float mcfx = 0.0f;


Models::Torus carWheel(0.3, 0.1, 12, 12);


void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

class CameraCore
{
public:
	  float yaw;
	  float pitch;
	  glm::vec3 position;
	  glm::vec3 front;
	  glm::vec3 up;
	  glm::vec3 last_up;
	  bool freeCamera;
	  float fov;
	  float aspectRatio;
	  float nearClip;
	  float farClip;
	  float speed;
	  float rotate;
	  double last_mouse_x;
	  double last_mouse_y;
	  float sensitivity;
	  bool rotate_l;
	  bool rotate_r;
	  bool rotate_u;
	  bool rotate_d;
	  bool move_f;
	  bool move_b;
	  bool move_l;
	  bool move_r;
	  bool move_u;
	  bool move_d;
	  CameraCore();
	  ~CameraCore();

private:

};

CameraCore::CameraCore()
{
	this->yaw = 0.0f;
	this->pitch = 0.0f;
	this->position = glm::vec3(0.0f, 2.0f, -7.0f);
	this->front = glm::vec3(0.0f, 0.0f, -1.0f);
	this->up = glm::vec3(0.0f, 1.0f, 0.0f);
	this->last_up = glm::vec3(0.0f, 1.0f, 0.0f);
	this->freeCamera = false;
	this->fov = 45.0f;
	this->aspectRatio = 0;
	this->nearClip = 0.1f;
	this->farClip = 100.0f;
	this->speed = 0.10f;
	this->rotate = 2.50f;
	this->last_mouse_x = 0.0;
	this->last_mouse_y = 0.0;
	this->sensitivity = 0.1f;
	this->rotate_l = false;
	this->rotate_r = false;
	this->rotate_u = false;
	this->rotate_d = false;
	this->move_f= false;
	this->move_b = false;
	this->move_l = false;
	this->move_r = false;
	this->move_u = false;
	this->move_d = false;
}
CameraCore::~CameraCore() {}

bool allow_mouse_collback = false;
CameraCore *camera = new CameraCore();

void key_callback(GLFWwindow* window, int key,
	int scancode, int action, int mods) {
	if (key == GLFW_KEY_V) {
		if (action == GLFW_PRESS) {
			allow_mouse_collback = true;
		} else if (action == GLFW_RELEASE) {
			allow_mouse_collback = false;
		}
	}
	if (key == GLFW_KEY_W) {
		if (action == GLFW_PRESS) {
			camera->move_f = true;
		} else if (action == GLFW_RELEASE) {
			camera->move_f = false;
		}
	} else if (key == GLFW_KEY_S) {
		if (action == GLFW_PRESS) {
			camera->move_b = true;
		} else if (action == GLFW_RELEASE) {
			camera->move_b = false;
		}
	} else if (key == GLFW_KEY_A) {
		if (action == GLFW_PRESS) {
			camera->move_l = true;
		} else if (action == GLFW_RELEASE) {
			camera->move_l = false;
		}
	} else if (key == GLFW_KEY_D) {
		if (action == GLFW_PRESS) {
			camera->move_r = true;
		} else if (action == GLFW_RELEASE) {
			camera->move_r = false;
		}
	} else if (key == GLFW_KEY_SPACE) {
		if (action == GLFW_PRESS) {
			camera->move_u = true;
		} else if (action == GLFW_RELEASE) {
			camera->move_u = false;
		}	
	}
	else if (key == GLFW_KEY_LEFT_SHIFT) {
		if (action == GLFW_PRESS) {
			camera->move_d = true;
		} else if (action == GLFW_RELEASE) {
			camera->move_d = false;
		}	
	} else if (key == GLFW_KEY_LEFT) {
		if (action == GLFW_PRESS) {
			camera->rotate_l = true;
		}
		else if (action == GLFW_RELEASE) {
			camera->rotate_l = false;
		}	
	}
	else if (key == GLFW_KEY_RIGHT) {
		if (action == GLFW_PRESS) {
			camera->rotate_r = true;
		} else if (action == GLFW_RELEASE) {
			camera->rotate_r = false;
		}	
	} else if (key == GLFW_KEY_UP) {
		if (action == GLFW_PRESS) {
			camera->rotate_u = true;
		} else if (action == GLFW_RELEASE) {
			camera->rotate_u = false;
		}	
	}
	else if (key == GLFW_KEY_DOWN) {
		if (action == GLFW_PRESS) {
			camera->rotate_d = true;
		} else if (action == GLFW_RELEASE) {
			camera->rotate_d = false;
		}
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	double deltaX = xpos - camera->last_mouse_x;
	double deltaY = ypos - camera->last_mouse_y;
	if (allow_mouse_collback == true) {
		camera->yaw += deltaX * camera->sensitivity;
		camera->pitch -= deltaY * camera->sensitivity;
	}
	camera->last_mouse_x = xpos;
	camera->last_mouse_y = ypos;
}


//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	initShaders();
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(0, 0, 0, 1);//Ustaw czarny kolor czyszczenia ekranu
	glEnable(GL_DEPTH_TEST); //Włącz test głębokości pikseli
	glfwSetKeyCallback(window, key_callback); //Zarejestruj procedurę obsługi klawiatury
	glfwSetCursorPosCallback(window, mouse_callback);
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
	freeShaders();
	//************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************
}


void createWheelWithSpokes(const glm::mat4& wheelMatrix, float wheelAngle, float size)
{
	glm::mat4 scaledWheelMatrix = glm::scale(wheelMatrix, glm::vec3(size));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(scaledWheelMatrix));
	Models::kolo3.drawSolid(spLambert, "./model/kolo3.obj");
}

void createSmallWheel(const glm::mat4& Ms, const glm::vec3& position, float skret, float wheelAngle, float smallsize)
{
	glm::mat4 MkSmall = Ms;
	MkSmall = glm::translate(MkSmall, position);
	MkSmall = glm::rotate(MkSmall, skret, glm::vec3(0.0f, 1.0f, 0.0f));
	MkSmall = glm::rotate(MkSmall, wheelAngle, glm::vec3(0.0f, 0.0f, 1.0f));
	createWheelWithSpokes(MkSmall, wheelAngle, smallsize);
}

int switch_belka = 0;
//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window, float angle, float wheelAngle, float belkaAngle) {
	auto oldcameraPosition = camera->position;
	if (camera->move_f)
		camera->position += camera->speed * camera->front;
	if (camera->move_b)
		camera->position -= camera->speed * camera->front;
	if (camera->move_l)
		camera->position -= glm::normalize(glm::cross(camera->front, camera->up)) * camera->speed;
	if (camera->move_r)
		camera->position += glm::normalize(glm::cross(camera->front, camera->up)) * camera->speed;
	if (camera->move_u)
		camera->position += camera->speed * camera->up;
	if (camera->move_d)
		camera->position -= camera->speed * camera->up;
	if (camera->rotate_l)
		camera->yaw -= camera->rotate;
	if (camera->rotate_r)
		camera->yaw += camera->rotate;
	if (camera->rotate_u)
		camera->pitch += camera->rotate;
	if (camera->rotate_d)
		camera->pitch -= camera->rotate;
	if (camera->pitch > 89.0f)
		camera->pitch = 89.0f;
	if (camera->pitch < -89.0f)
		camera->pitch = -89.0f;
	glm::vec3 front;
	front.x = cos(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
	front.y = sin(glm::radians(camera->pitch));
	front.z = sin(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
	camera->front = glm::normalize(front);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 50.0f);
	glm::mat4 V;
	V = glm::lookAt(camera->position, camera->position + camera->front, camera->up);

	spLambert->use();//Aktywacja programu cieniującego
	glUniformMatrix4fv(spLambert->u("P"), 1, false, glm::value_ptr(P)); //Załadowanie macierzy rzutowania do programu cieniującego
	glUniformMatrix4fv(spLambert->u("V"), 1, false, glm::value_ptr(V)); //Załadowanie macierzy widoku do programu cieniującego
	

	glm::mat4 Ms = glm::mat4(1.0f);
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(Ms));
	//Ms = glm::rotate(Ms, angle, glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 translationMatrix1 = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 1.2f, 0.0f));
	translationMatrix1 = glm::scale(translationMatrix1, glm::vec3(1.0f, 0.4f, 1.0f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(translationMatrix1));
	Models::object.drawSolid(spLambert, "./model/cube.obj");

	// Rysowanie słupków w narożnikach
	float poleHeight = 0.47f;  // Wysokość słupka
	float poleWidth = 0.1f;   // Szerokość słupka
	glm::vec3 poleScale(poleWidth, poleHeight, poleWidth);

	// Współrzędne narożników (przyjmując, że środek podwozia to (0,0,0))
	glm::vec3 corners[4] = {
		glm::vec3(1.4f, 0.35f, -0.77f),
		glm::vec3(1.4f, 0.35f, 0.77f)
	};

	for (int i = 0; i < 2; i++) {
		glm::mat4 poleMatrix = glm::translate(Ms, corners[i] + glm::vec3(0.0f, poleHeight / 2, 0.0f));
		poleMatrix = glm::scale(poleMatrix, poleScale);
		glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(poleMatrix));
		Models::cube.drawSolid();
	}

	// Rysowanie słupków w narożnikach DUZE
	float poleHeightDuze = 2.0f;  // Wysokość słupka
	float poleWidthDuze = 0.1f;   // Szerokość słupka
	glm::vec3 poleScaleDuze(poleWidthDuze, poleHeightDuze, poleWidthDuze);

	// Współrzędne narożników (przyjmując, że środek podwozia to (0,0,0))
	glm::vec3 cornersduze[4] = {
		glm::vec3(-1.4f, 0.35f, -0.77f),
		glm::vec3(-1.4f, 0.35f, 0.77f),
	};

	for (int i = 0; i < 2; i++) {
		glm::mat4 poleMatrix = glm::translate(Ms, cornersduze[i] + glm::vec3(0.0f, poleHeightDuze / 2, 0.0f));
		poleMatrix = glm::scale(poleMatrix, poleScaleDuze);
		glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(poleMatrix));
		Models::cube.drawSolid();
	}

	// DUZE
	glm::mat4 walecMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(5.1f, 0.5f, 0.0f));
	walecMatrix = glm::rotate(walecMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	walecMatrix = glm::rotate(walecMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	walecMatrix = glm::scale(walecMatrix, glm::vec3(1.55f, 1.1f, 1.55f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(walecMatrix));
	Models::walec.drawSolid(spLambert, "./model/walec.obj");


	glm::mat4 komin1Matrix = glm::translate(glm::mat4(1.0f), glm::vec3(3.9f, 0.7f, 0.0f));
	komin1Matrix = glm::scale(komin1Matrix, glm::vec3(0.7f, 0.7f, 0.7f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(komin1Matrix));
	Models::walec.drawSolid(spLambert, "./model/walec.obj");

	glm::mat4 komin2Matrix = glm::translate(glm::mat4(1.0f), glm::vec3(2.7f, 0.7f, 0.0f));
	komin2Matrix = glm::scale(komin2Matrix, glm::vec3(0.7f, 0.5f, 0.7f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(komin2Matrix));
	Models::walec.drawSolid(spLambert, "./model/walec.obj");

	float size = 2.2;

	// DUZE KOLA
	glm::vec3 wheelPositions[] = {
		glm::vec3(1.0f, 0.05f, 1.0f),
		glm::vec3(-1.0f, 0.05f, 1.0f),
		glm::vec3(1.0f, 0.05f, -1.0f),
		glm::vec3(-1.0f, 0.05f, -1.0f),
	};

	for (int i = 0; i < 4; i++) {
		glm::mat4 Mk = Ms;
		Mk = glm::translate(Mk, wheelPositions[i]);
		if (i % 2 == 0) {
			Mk = glm::rotate(Mk, skret, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		Mk = glm::rotate(Mk, wheelAngle, glm::vec3(0.0f, 0.0f, 1.0f));
		Mk = glm::scale(Mk, glm::vec3(1.5f, 1.5f, 1.5f));
		createWheelWithSpokes(Mk, wheelAngle, size);

		// POLACZENIE KOL
		if (i == 2 || i == 3) {
			glm::mat4 connector = glm::translate(glm::mat4(1.0f), wheelPositions[i]);
			connector = glm::rotate(connector, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			connector = glm::scale(connector, glm::vec3(0.25f, 0.65f, 0.25f));
			glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(connector));
			Models::walec.drawSolid(spLambert, "./model/walec.obj");
		}
	}


	// small section:
	float smallsize = 1.5;

	glm::vec3 positions[] = {
		glm::vec3(2.4f, -0.405f, 1.0f),
		glm::vec3(3.4f, -0.405f, 1.0f),
		glm::vec3(4.4f, -0.405f, 1.0f),
		glm::vec3(2.4f, -0.405f, -1.0f),
		glm::vec3(3.4f, -0.405f, -1.0f),
		glm::vec3(4.4f, -0.405f, -1.0f)
	};

	for (int i = 0; i < 6; i++) {
		createSmallWheel(Ms, positions[i], skret, wheelAngle, smallsize);

		// POLACZENIE KOL
		if (i >= 3) {
			glm::mat4 smallConnector = glm::translate(glm::mat4(1.0f), positions[i]);
			smallConnector = glm::rotate(smallConnector, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			smallConnector = glm::scale(smallConnector, glm::vec3(0.15f, 0.65f, 0.15f));
			glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(smallConnector));
			Models::walec.drawSolid(spLambert, "./model/walec.obj");
		}
	}


	glm::vec3 belka_position_bottom = glm::vec3(-0.15f, 0.6f, 1.21f);
	glm::vec3 belka_position_left = glm::vec3(0.5f, -0.0f, 1.21f);
	glm::vec3 belka_position_top = glm::vec3(-0.2f, -0.6f, 1.21f);
	glm::vec3 belka_position_right = glm::vec3(-0.7f, -0.0f, 1.21f);
	// Obliczanie nowej pozycji belki
	float belkaRadius = glm::distance(belka_position_top, belka_position_right);
	glm::vec3 belkaCenter = (belka_position_top + belka_position_right + belka_position_bottom + belka_position_left) / 4.0f;
	glm::vec3 newBelkaPosition = glm::vec3(
		belkaCenter.x - belkaRadius * cos(belkaAngle),
		belkaCenter.y - belkaRadius * sin(belkaAngle),
		belkaCenter.z
	);
	glm::mat4 belkaMatrix = glm::translate(glm::mat4(1.0f), newBelkaPosition);
	belkaMatrix = glm::rotate(belkaMatrix, PI / 2.0f, glm::vec3(0.0f, -1.0f, 0.0f));
	//belkaMatrix = glm::rotate(belkaMatrix, PI / 1.0f, glm::vec3(2.0f, 0.0f, 0.0f));  // TODO poprawić rotację
	belkaMatrix = glm::scale(belkaMatrix, glm::vec3(0.48f, 0.48f, 0.48f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(belkaMatrix));
	Models::belka.drawSolid(spLambert, "./model/belka.obj");


	newBelkaPosition = glm::vec3(
		belkaCenter.x - belkaRadius * cos(belkaAngle),
		belkaCenter.y - belkaRadius * sin(belkaAngle),
		belkaCenter.z - 2.47
	);
	belkaMatrix = glm::translate(glm::mat4(1.0f), newBelkaPosition);
	belkaMatrix = glm::rotate(belkaMatrix, PI / 2.0f, glm::vec3(0.0f, -1.0f, 0.0f));
	belkaMatrix = glm::rotate(belkaMatrix, PI / 1.0f, glm::vec3(2.0f, 0.0f, 0.0f));  // TODO poprawić rotację
	belkaMatrix = glm::scale(belkaMatrix, glm::vec3(0.48f, 0.48f, 0.48f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(belkaMatrix));
	Models::belka.drawSolid(spLambert, "./model/belka.obj");

	newBelkaPosition = glm::vec3(
		belkaCenter.x - belkaRadius/2 * cos(belkaAngle) + 3.9,
		belkaCenter.y - belkaRadius/2 * sin(belkaAngle) - 0.3,
		belkaCenter.z - 2.47
	);
	belkaMatrix = glm::translate(glm::mat4(1.0f), newBelkaPosition);
	belkaMatrix = glm::rotate(belkaMatrix, PI / 2.0f, glm::vec3(0.0f, -1.0f, 0.0f));
	belkaMatrix = glm::rotate(belkaMatrix, PI / 1.0f, glm::vec3(2.0f, 0.0f, 0.0f));  // TODO poprawić rotację
	belkaMatrix = glm::scale(belkaMatrix, glm::vec3(0.24f, 0.24f, 0.24f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(belkaMatrix));
	Models::belka.drawSolid(spLambert, "./model/belka.obj");

	newBelkaPosition = glm::vec3(
		belkaCenter.x + belkaRadius / 2 * sin(belkaAngle) + 2.9,
		belkaCenter.y - belkaRadius / 2 * cos(belkaAngle) - 0.3,
		belkaCenter.z - 2.47
	);
	belkaMatrix = glm::translate(glm::mat4(1.0f), newBelkaPosition);
	belkaMatrix = glm::rotate(belkaMatrix, PI / 2.0f, glm::vec3(0.0f, -1.0f, 0.0f));
	belkaMatrix = glm::rotate(belkaMatrix, PI / 1.0f, glm::vec3(2.0f, 0.0f, 0.0f));  // TODO poprawić rotację
	belkaMatrix = glm::scale(belkaMatrix, glm::vec3(0.24f, 0.24f, 0.24f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(belkaMatrix));
	Models::belka.drawSolid(spLambert, "./model/belka.obj");

	newBelkaPosition = glm::vec3(
		belkaCenter.x - belkaRadius / 2 * cos(belkaAngle) + 3.9,
		belkaCenter.y - belkaRadius / 2 * sin(belkaAngle) - 0.4,
		belkaCenter.z
	);
	belkaMatrix = glm::translate(glm::mat4(1.0f), newBelkaPosition);
	belkaMatrix = glm::rotate(belkaMatrix, PI / 2.0f, glm::vec3(0.0f, -1.0f, 0.0f));
	belkaMatrix = glm::rotate(belkaMatrix, PI / 1.0f, glm::vec3(2.0f, 0.0f, 0.0f));  // TODO poprawić rotację
	belkaMatrix = glm::scale(belkaMatrix, glm::vec3(0.24f, 0.24f, 0.24f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(belkaMatrix));
	Models::belka.drawSolid(spLambert, "./model/belka.obj");

	newBelkaPosition = glm::vec3(
		belkaCenter.x + belkaRadius / 2 * sin(belkaAngle) + 2.9,
		belkaCenter.y - belkaRadius / 2 * cos(belkaAngle) - 0.4,
		belkaCenter.z
	);
	belkaMatrix = glm::translate(glm::mat4(1.0f), newBelkaPosition);
	belkaMatrix = glm::rotate(belkaMatrix, PI / 2.0f, glm::vec3(0.0f, -1.0f, 0.0f));
	belkaMatrix = glm::rotate(belkaMatrix, PI / 1.0f, glm::vec3(2.0f, 0.0f, 0.0f));  // TODO poprawić rotację
	belkaMatrix = glm::scale(belkaMatrix, glm::vec3(0.24f, 0.24f, 0.24f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(belkaMatrix));
	Models::belka.drawSolid(spLambert, "./model/belka.obj");

	float belkaLength = 2.0;
	float belkaAngleInRadians = glm::radians(belkaAngle*50);
	float modifiedBelkaAngle = belkaAngleInRadians;
	if (sin(belkaAngleInRadians) >= 0)
	{
		modifiedBelkaAngle = belkaAngleInRadians;
	}
	else
	{
		modifiedBelkaAngle = -belkaAngleInRadians;
	}
	glm::mat4 secondBelkaMatrix = glm::mat4(1.0f);

	glm::vec3 newBelka2Position = glm::vec3(
		belkaCenter.x - belkaRadius*2 * cos(belkaAngle),
		belkaCenter.y,
		belkaCenter.z
	);

	secondBelkaMatrix = glm::translate(secondBelkaMatrix, newBelka2Position);
	//secondBelkaMatrix = glm::translate(secondBelkaMatrix, glm::vec3(modifiedBelkaAngle, 0.0f, 0.0f)); // TODO fix
	secondBelkaMatrix = glm::rotate(secondBelkaMatrix, PI / 2.0f, glm::vec3(0.0f, -1.0f, 0.0f));
	secondBelkaMatrix = glm::rotate(secondBelkaMatrix, PI / 2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	secondBelkaMatrix = glm::rotate(secondBelkaMatrix, modifiedBelkaAngle, glm::vec3(0.5f, 0.0f, 0.0f));
	secondBelkaMatrix = glm::translate(secondBelkaMatrix, -belkaCenter);
	secondBelkaMatrix = glm::scale(secondBelkaMatrix, glm::vec3(0.48f, 0.48f, 0.48f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(secondBelkaMatrix));
	Models::belka.drawSolid(spLambert, "./model/belka.obj");
	// TO DZIAŁA END


	// TORY
	for (int i=0; i<15; i++) {
		glm::mat4 trackMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(7.0f -i * 0.95f, -1.15f, -0.15f));
		trackMatrix = glm::rotate(trackMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.50f, 0.0f));
		trackMatrix = glm::scale(trackMatrix, glm::vec3(1.3f, 1.0f, 1.0f));
		glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(trackMatrix));
		Models::tory.drawSolid(spLambert, "./model/trainTrack2.obj");
	}


	// PODWOZIE
	glm::mat4 baseMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
	baseMatrix = glm::scale(baseMatrix, glm::vec3(2.0f, 1.5f, 2.5f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(baseMatrix));
	Models::podwozie.drawSolid(spLambert, "./model/base.obj");


	//Skopiowanie bufora ukrytego do widocznego. Z reguły ostatnie polecenie w procedurze drawScene.
	glfwSwapBuffers(window);
}


int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}
	Models::belka = Models::Object("./model/belka.obj");
	Models::walec = Models::Object("./model/walec2.obj");
	Models::kolo3 = Models::Object("./model/kolo3.obj");
	Models::tory = Models::Object("./model/trainTrack2.obj");
	Models::podwozie = Models::Object("./model/base.obj");

	window = glfwCreateWindow(1200, 1080, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjujące

	float angle = 0; //Aktualny kąt obrotu obiektu
	float wheelAngle = 0; //Aktualny kąt obrotu kół
	float belkaAngle = 0; // Aktualny kąt obrotu belki

	glfwSetTime(0); //Wyzeruj timer
	//Główna pętla
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		angle += speed * glfwGetTime(); //Oblicz przyrost kąta po obrocie
		wheelAngle += -PI * 2 * glfwGetTime();
		belkaAngle += -PI * 2 * glfwGetTime();
		glfwSetTime(0); //Wyzeruj timer
		drawScene(window, angle, wheelAngle, belkaAngle); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
