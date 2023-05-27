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
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

float pcpz = 0.0f;
float pcpx = 0.0f;
float pcfy = 0.0f;
float pcfx = 0.0f;

float mcpz = 0.0f;
float mcpx = 0.0f;
float mcfy = 0.0f;
float mcfx = 0.0f;


Models::Torus carWheel(0.3, 0.1, 12, 12);
//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

//Procedura obsługi klawiatury
void key_callback(GLFWwindow* window, int key,
	int scancode, int action, int mods) {

	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_W) pcpz += 0.1f;
		if (key == GLFW_KEY_S) mcpz -= 0.1f;
		if (key == GLFW_KEY_A) mcpx -= 0.1f;
		if (key == GLFW_KEY_D) pcpx += 0.1f;
		if (key == GLFW_KEY_UP) pcfy += 0.1f;
		if (key == GLFW_KEY_DOWN) mcfy -= 0.1f;
		if (key == GLFW_KEY_LEFT) mcfx -= 0.1f;
		if (key == GLFW_KEY_RIGHT) pcfx += 0.1f;
	}

	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_W) pcpz = 0.0f;
		if (key == GLFW_KEY_S) mcpz = 0.0f;
		if (key == GLFW_KEY_A) mcpx = 0.0f;
		if (key == GLFW_KEY_D) pcpx = 0.0f;
		if (key == GLFW_KEY_UP) pcfy = 0.0f;
		if (key == GLFW_KEY_DOWN) mcfy = 0.0f;
		if (key == GLFW_KEY_LEFT) mcfx = 0.0f;
		if (key == GLFW_KEY_RIGHT) pcfx = 0.0f;
	}
}


//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	initShaders();
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(0, 0, 0, 1);//Ustaw czarny kolor czyszczenia ekranu
	glEnable(GL_DEPTH_TEST); //Włącz test głębokości pikseli
	glfwSetKeyCallback(window, key_callback); //Zarejestruj procedurę obsługi klawiatury
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
	carWheel.drawSolid();

	glm::mat4 spokeMatrix1 = wheelMatrix;
	spokeMatrix1 = glm::rotate(spokeMatrix1, PI / 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	spokeMatrix1 = glm::scale(spokeMatrix1, glm::vec3(size * 0.02f, size * 0.02f, size * 0.3f));
	spokeMatrix1 = glm::translate(spokeMatrix1, glm::vec3(0.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(spokeMatrix1));
	Models::cube.drawSolid();

	glm::mat4 spokeMatrix2 = wheelMatrix;
	spokeMatrix2 = glm::rotate(spokeMatrix2, PI / 2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	spokeMatrix2 = glm::scale(spokeMatrix2, glm::vec3(size * 0.02f, size * 0.02f, size * 0.3f));
	spokeMatrix2 = glm::translate(spokeMatrix2, glm::vec3(0.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(spokeMatrix2));
	Models::cube.drawSolid();

	glm::mat4 spokeMatrix3 = wheelMatrix;
	spokeMatrix3 = glm::rotate(spokeMatrix3, PI / 1.37f, glm::vec3(0.0f, 1.0f, 1.0f)); // TODO
	spokeMatrix3 = glm::scale(spokeMatrix3, glm::vec3(size * 0.02f, size * 0.02f, size * 0.3f));
	spokeMatrix3 = glm::translate(spokeMatrix3, glm::vec3(0.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(spokeMatrix3));
	Models::cube.drawSolid();
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
	cameraPos.z += pcpz;
	cameraPos.z += mcpz;
	cameraPos.x += mcpx;
	cameraPos.x += pcpx;
	cameraFront.y += pcfy;
	cameraFront.y += mcfy;
	cameraFront.x += mcfx;
	cameraFront.x += pcfx;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 50.0f); // TODO
	glm::mat4 V = glm::lookAt(cameraPos, cameraFront, glm::vec3(0.0f, 1.0f, 0.0f)); // TODO
	//glm::mat4 V = glm::lookAt(cameraPos, cameraPos + cameraFront, glm::vec3(0.0f, 1.0f, 0.0f)); // TODO

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

	// Rysowanie podwozia
	glm::mat4 Mp = glm::scale(Ms, glm::vec3(1.5f, 0.125f, 1.0f));
	glUniform4f(spLambert->u("color"), 1, 1, 1, 1);
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(Mp));
	Models::cube.drawSolid();

	// Rysowanie drugiego podwozia
	glm::mat4 Mp2 = glm::translate(Mp, glm::vec3(0.0f, 7.5f, 0.0f));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(Mp2));
	Models::cube.drawSolid();

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

	glm::mat4 walecMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(4.55f, 0.5f, 0.0f));
	walecMatrix = glm::rotate(walecMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	walecMatrix = glm::rotate(walecMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	walecMatrix = glm::scale(walecMatrix, glm::vec3(1.5f, 1.0f, 1.5f));
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

	//Models::kolo2.drawSolid(spLambert, "./model/kolo2.obj");

	float size = 2.2;

	//// Generowanie koła 1 z szprychami
	glm::mat4 Mk1 = Ms;
	Mk1 = glm::translate(Mk1, glm::vec3(0.75f, 0.0f, 1.0f));
	Mk1 = glm::rotate(Mk1, skret, glm::vec3(0.0f, 1.0f, 0.0f));
	Mk1 = glm::rotate(Mk1, wheelAngle, glm::vec3(0.0f, 0.0f, 1.0f));
	createWheelWithSpokes(Mk1, wheelAngle, size);

	//// Generowanie koła 2 z szprychami
	glm::mat4 Mk2 = Ms;
	Mk2 = glm::translate(Mk2, glm::vec3(0.75f, 0.0f, -1.0f));
	Mk2 = glm::rotate(Mk2, skret, glm::vec3(0.0f, 1.0f, 0.0f));
	Mk2 = glm::rotate(Mk2, wheelAngle, glm::vec3(0.0f, 0.0f, 1.0f));
	createWheelWithSpokes(Mk2, wheelAngle, size);

	////Koło3
	glm::mat4 Mk3 = Ms;
	Mk3 = glm::translate(Mk3, glm::vec3(-1.0f, 0.0f, 1.0f));
	Mk3 = glm::rotate(Mk3, wheelAngle, glm::vec3(0.0f, 0.0f, 1.0f));
	createWheelWithSpokes(Mk3, wheelAngle, size);

	////Koło4
	glm::mat4 Mk4 = Ms;
	Mk4 = glm::translate(Mk4, glm::vec3(-1.0f, 0.0f, -1.0f));
	Mk4 = glm::rotate(Mk4, wheelAngle, glm::vec3(0.0f, 0.0f, 1.0f));
	createWheelWithSpokes(Mk4, wheelAngle, size);


	// small section:
	float smallsize = 1.1;

	glm::vec3 positions[] = {
		glm::vec3(2.4f, -0.4f, 1.0f),
		glm::vec3(3.4f, -0.4f, 1.0f),
		glm::vec3(4.4f, -0.4f, 1.0f),
		glm::vec3(2.4f, -0.4f, -1.0f),
		glm::vec3(3.4f, -0.4f, -1.0f),
		glm::vec3(4.4f, -0.4f, -1.0f)
	};

	for (int i = 0; i < 6; i++) {
		createSmallWheel(Ms, positions[i], skret, wheelAngle, smallsize);
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
	//Models::object = Models::Object("./model/cube.obj"); // walec
	//Models::cubexd = Models::Object("./model/cube.obj"); // walec
	window = glfwCreateWindow(500, 500, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

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
