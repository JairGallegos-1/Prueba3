			/*---------------------------------------------------------*/
			/* ----------------Proyecto Final --------------------------*/
			/*-----------------2021-1 ---------------------------*/
			/*------------- Alumno: Gallegos Santiago Jair de Jesús ---------------*/
			/*------------- Alumno: Guevara Hernández Jorge Alejandro ---------------*/
			/*------------- Alumno: Rivera Salinas Oscar Eduardo ---------------*/



#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include<time.h>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>
#include <MMSystem.h>

//#pragma comment(lib, "winmm.lib")

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//----------------
GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;

void myData(void);

void myData()
{
	GLfloat vertices[] = {
		//Position				//Color
		-0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V0 - Frontal
		0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V1
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V5
		-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V4

		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V2 - Trasera
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 0.0f,	//V3
		-0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V7
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V6

		-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	//V4 - Izq
		-0.5f, 0.5f, -0.5f,		0.0f, 0.0f, 1.0f,	//V7
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	//V3
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	//V0

		0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	//V5 - Der
		0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	//V1
		0.5f, -0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	//V2
		0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	//V6

		-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 1.0f,	//V4 - Sup
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 1.0f,	//V5
		0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	//V6
		-0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	//V7

		-0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	//V0 - Inf
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,	//V3
		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	//V2
		0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	//V1
	};

	unsigned int indices[] =	//I am not using index for this session
	{
		0
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	//Para trabajar con indices (Element Buffer Object)
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);



}

//--------------------

// camera
Camera camera(glm::vec3(300.0f, 80.0f, 500.0f));
float MovementSpeed = 0.1f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
bool juego = false,
	balon1 = true,
	mono1 = false,
	mono2 = false,
	nada = false,
	recorrido = false,
	subeHeli = true;

float giroMono1 = -90;
float rotaMonita = 0;
float avanzaMona = -70.0;
float giroMono2 = 0.0,
	nocheX = 1.0f,
	nocheY = 1.0f,
	nocheZ = 1.0f,
	heliX = 250.0f,
	heliY = 50.0f,
	eliZ = -650.0f,
	eliGiro = 270.0f;

float	luzRoja = 0.5f,
luzVerde = 0.2f,
luzAzul = 0.7f,
r = 0.0f,
g = 0.0f,
b = 0.0f;

float dirluz = -1.0f;
bool luz_casa = false;
//
//glm::vec3 lightPosition(100.0f, 1.0f, -100.0f);
//glm::vec3 lightDirection(0.0f, -1.0f, 0.0f);


glm::vec3 lightPosition(1.0f, 1.0f, 1.0f);
glm::vec3 lightDirection(-1, -1, -2.0);

glm::vec3 lightPosition2(1.0f, 1.0f, 1.0f);
glm::vec3 lightDirection2(-1, 1, -2.0);


// posiciones
float x = 0.0f;
float y = 0.0f;
float movAuto_z = 0.0f;
float movAuto_x = 0.0f;
bool animacion = false;
int distancia = 4, contador = 0;

float balonX = 305.0f,
balonY = 1.0f,
balonZ = 358.0f,
movMono1X = 90 * distancia,
movMono1Y = 9;

bool arribaMona = false,
abajoMona = true,
helicoptero = false;


//Keyframes (Manipulación y dibujo)
float	posX = 0.0f,
		posY = 0.0f,
		posZ = 0.0f,
		rotRodIzq = 0.0f,
		giroMonito = 0.0f;
float	incX = 0.0f,
		incY = 0.0f,
		incZ = 0.0f,
		rotInc = 0.0f,
		giroMonitoInc = 0.0f,
		movBrazoIzq = 0.0f,
		movBrazoIzqInc = 0.0f,

		movBrazoDer = 0.0f,
		movBrazoDerInc = 0.0f,

		movCabeza = 0.0f,
		movCabezaInc = 0.0f,

		movPierDer = 0.0f,
		movPierDerInc = 0.0f
	;


bool  animacionPerro = false;
bool  animacionPatos = false;
float orienta = 0.0f;
float orientaA = 90.0f;
float orientaPato = 90.0f;
bool  recorrido1 = true;
bool  recorrido2 = false;
bool  recorrido3 = false;
bool  recorrido4 = true;
bool  recorrido5 = false;
bool  recorrido6 = true;
bool  recorrido7 = false;
bool  recorrido8 = false;
bool  recorrido9 = false;
bool  recorrido10 = false;
float movAnimalx = 0.0f;
float movAnimalz = 0.0f;
float movPatosx = 0.0f;

bool  animacionCarro = false;
bool  animacionesReset = false;




#define MAX_FRAMES 9
int i_max_steps = 60;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float rotRodIzq;
	float giroMonito;
	float movBrazoIzq;
	float movBrazoDer;
	float movCabeza;
	float movPierDer;
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false,
	noche = false;
int playIndex = 0;

void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;

	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;
	KeyFrame[FrameIndex].giroMonito = giroMonito;

	KeyFrame[FrameIndex].movBrazoIzq = movBrazoIzq; //---------

	KeyFrame[FrameIndex].movBrazoDer = movBrazoDer; //---------

	KeyFrame[FrameIndex].movCabeza = movCabeza; //---------

	KeyFrame[FrameIndex].movPierDer = movPierDer; //---------




	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotRodIzq = KeyFrame[0].rotRodIzq;
	giroMonito = KeyFrame[0].giroMonito;
	
	movBrazoIzq = KeyFrame[0].movBrazoIzq;//-------

	movBrazoDer = KeyFrame[0].movBrazoDer;//-------

	movCabeza = KeyFrame[0].movCabeza;//-------

	movPierDer = KeyFrame[0].movPierDer;//-------
}

void interpolation(void)
{
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	giroMonitoInc = (KeyFrame[playIndex + 1].giroMonito - KeyFrame[playIndex].giroMonito) / i_max_steps;


	movBrazoIzqInc = (KeyFrame[playIndex + 1].movBrazoIzq - KeyFrame[playIndex].movBrazoIzq) / i_max_steps; //-------------------

	movBrazoDerInc = (KeyFrame[playIndex + 1].movBrazoDer - KeyFrame[playIndex].movBrazoDer) / i_max_steps; //-------------------

	movCabezaInc = (KeyFrame[playIndex + 1].movCabeza - KeyFrame[playIndex].movCabeza) / i_max_steps; //-------------------

	movPierDerInc = (KeyFrame[playIndex + 1].movPierDer - KeyFrame[playIndex].movPierDer) / i_max_steps; //-------------------


}

int i = 0;


void sonido() {

	sndPlaySound("musica.wav", SND_ASYNC);
}



void sonido();






void animate(void)
{

	if (animacionesReset) {
		movAnimalx = 0;
		movAnimalz = 0;
		movAuto_x = 0;
		movAuto_z = 0;
		movPatosx = 0;
		recorrido6 = true;
		animacionesReset = false;

		heliX = 250.0f,
		heliY = 50.0f,
		eliZ = -650.0f,
		eliGiro = 270.0f;
		helicoptero = false;

	}


	
	if (juego) {
		
			if (i <= 110 && balon1) {
					i++;
					balonX += 0.5;
					balonY += 0.14;
					balonZ -= 0.5;
					
			}

			else if (i>=110 && !mono2) {
				balon1 = false;
				mono1 = true;
				i = 0;
			}

			else if (mono1 && i <= 100) {
				giroMono1 += 2.0;
				i++;

			}

			else if (i >= 100 && !mono2) {
				mono1 = false;
				mono2 = true;
				i = 0;
			}

			else if (mono2 && i <= 130) {
				i++;
				giroMono2 += 0.5;
				balonX += 1.0;
				balonY -= 0.1;
				balonZ += 0.2;

			}

			else{

				i = 0;
				juego = false;
				balon1 = true;
				mono1 = false;
				mono2 = false;
				giroMono1 = -90.0f;
				giroMono2 = 0.0f;
				balonX = 305.0f;
				balonY = 1.0f;
				balonZ = 358.0f;
			} 
			


	}



	if (animacionPerro)
	{
		if (recorrido1)
		{
			movAnimalx += 2.5f;
			orientaA = 90.0f;
			if (movAnimalx > 170.0f)
			{
				recorrido1 = false;
				recorrido2 = true;
			}
		}
		if (recorrido2)
		{
			movAnimalz -= 2.2f;
			orientaA = 180.0f;
			if (movAnimalz < -200.0f)
			{
				recorrido2 = false;
				recorrido3 = true;
			}
		}
		if (recorrido3)
		{
			movAnimalx -= 2.2f;
			orientaA = -90.0f;
			if (movAnimalx < -50.0f)
			{
				recorrido3 = false;
				recorrido1 = true;
			}
		}
	}

	if (animacionPatos)
	{
		if (recorrido4)
		{
			movPatosx += 2.5f;
			orientaPato = 90.0f;
			if (movPatosx > 250.0f)
			{
				recorrido4 = false;
				recorrido5 = true;
			}
		}
		if (recorrido5)
		{
			movPatosx -= 2.5f;
			orientaPato = -90.0f;
			if (movPatosx < -300.0f)
			{
				recorrido5 = false;
				recorrido4 = true;
			}
		}
	}


	if (nada && avanzaMona >= -300 && abajoMona) {

		avanzaMona -= 0.2;
		rotaMonita += 1.0;
		if (rotaMonita >= 30) {
			abajoMona = false;
			arribaMona = true;
		} 
	}


	else if (nada && avanzaMona >= -300 && arribaMona) {

		avanzaMona -= 0.2;
		rotaMonita -= 1.0;
		if (rotaMonita <= -30) {

			abajoMona = true;
			arribaMona = false;
		}

	}

	else nada = false;

	//------------------------




	if (animacionCarro)
	{
		if (recorrido6)
		{
			movAuto_z += 2.0f;
			orienta = 0.0f;
			if (movAuto_z > 590.0f)
			{
				recorrido6 = false;
				recorrido7 = true;
			}
		}

		if (recorrido7)
		{
			movAuto_z += 1.0f;
			orienta = 90.0f;
			if (movAuto_z > 630.0f)
			{
				recorrido7 = false;
				recorrido8 = true;
			}
		}

		if (recorrido8)
		{
			movAuto_z += 1.0f;
			orienta = 180.0f;
			if (movAuto_z > 670.0f)
			{
				recorrido8 = false;
				recorrido9 = true;
			}
		}

		if (recorrido9)
		{
			movAuto_z += 1.0f;
			orienta = 270.0f;
			if (movAuto_z > 710.0f)
			{
				recorrido9 = false;
				recorrido10 = true;
			}
		}

		if (recorrido10)
		{
			movAuto_z += 1.0f;
			movAuto_x -= 1.2f;
			orienta = 0.0f;
			if (movAuto_z > 750.0f)
			{
				recorrido10 = false;
			}
		}
	}


	if (recorrido) {

		if (contador <= 120) {
			contador++;
			deltaTime -= 0.4f;
			camera.ProcessKeyboard(FORWARD, (float)deltaTime);
		}

		else if (contador <= 190) {
			contador++;
			deltaTime -= 0.2f;
			camera.ProcessKeyboard(LEFT, (float)deltaTime);
			camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
		}

		else if (contador <= 270) {
			contador++;
			deltaTime -= 0.2f;
			camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
		}

		else if (contador <= 330) {
			contador++;
			deltaTime -= 0.2f;
			camera.ProcessKeyboard(RIGHT, (float)deltaTime);
		}
	}
	


	if (helicoptero) {

		if (heliY <= 200.0f && subeHeli) {
			heliY += 1.0f;
			if (eliGiro <= 360) {
				eliGiro += 2.0f;
			}

		}

		else if (eliZ <= 100) {
			subeHeli = false;
			eliZ += 2.0f;

		}

		else if (heliY >= 50 && !subeHeli) {

			heliY -= 1.0f;
			if (eliGiro >= 270) {
				eliGiro -= 2.0f;
			}
		}


	}



}






int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 10", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	//Shader staticShader("Shaders/lightVertex.vs", "Shaders/lightFragment.fs");
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");

	vector<std::string> faces
	{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models
	// -----------
	Model piso("resources/objects/piso/piso.obj");
	Model pisoModel("resources/objects/piso/piso.obj");
	Model casa("resources/objects/casa/obj/Farmhouse_OBJ.obj");
	Model agua("resources/objects/agua/Ocean.obj");
	Model palmera("resources/objects/palmera/10446_Palm_Tree_v1_max2010_iteration-2.obj");
	Model mono("resources/objects/mono/LegoMan.obj");
	Model balon("resources/objects/balon/Ball.obj");
	Model sombrilla("resources/objects/sombrilla/12984_beach_umbrella_v1_L2.obj");
	Model toalla("resources/objects/toalla/13519_Beach_Towels_v2_L2.obj");
	Model porteria("resources/objects/porteria/football goal.obj");
	Model perro("resources/objects/Perro/Dog.obj");
	Model pato("resources/objects/Pato/duck_obj.obj");
	Model ascera("resources/objects/Calle/road.obj");
	Model silla("resources/objects/silla/beach_chair_V.obj");
	Model comida1("resources/objects/comida1/city_stall.obj");
	Model mona("resources/objects/mona/D0207034A.obj");
	Model carro("resources/objects/Lambo/Carroceria.obj");
	Model llanta("resources/objects/Lambo/Wheel.obj");
	Model acera("resources/objects/Calle/road.obj");
	Model asfalto("resources/objects/Calle2/road.obj");
	Model mansion("resources/objects/Mansion/Farm_house.obj");
	Model pasto("resources/objects/Pasto/road.obj");
	Model casaFinal("resources/objects/casaFinal/casas.obj");
	Model heli("resources/objects/heli/AW101.obj");


	

	//Inicialización de KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].giroMonito = 0;
	}

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);
		
		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		staticShader.use();
		
			// don't forget to enable shader before setting uniforms
			//staticShader.use();
			////Setup Advanced Lights
			/*staticShader.setVec3("viewPos", camera.Position);
			staticShader.setVec3("dirLight.direction", lightDirection);
			staticShader.setVec3("dirLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
			staticShader.setVec3("dirLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
			staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));

			staticShader.setVec3("pointLight[0].position", lightPosition);
			staticShader.setVec3("pointLight[0].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
			staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
			staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
			staticShader.setFloat("pointLight[0].constant", 0.8f);
			staticShader.setFloat("pointLight[0].linear", 0.009f);
			staticShader.setFloat("pointLight[0].quadratic", 0.032f);

			staticShader.setVec3("pointLight[1].position", glm::vec3(1.0, 1.0f, 1.0f));
			staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
			staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
			staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
			staticShader.setFloat("pointLight[1].constant", 1.0f);
			staticShader.setFloat("pointLight[1].linear", 0.009f);
			staticShader.setFloat("pointLight[1].quadratic", 0.032f);

			staticShader.setFloat("material_shininess", 32.0f);*/

			staticShader.setVec3("viewPos", camera.Position);
			staticShader.setVec3("dirLight.direction", lightDirection);
			staticShader.setVec3("dirLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
			staticShader.setVec3("dirLight.diffuse", glm::vec3(nocheX, nocheY, nocheZ));
			//staticShader.setVec3("dirLight.diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
			staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));

			staticShader.setVec3("pointLight[0].position", lightPosition);
			staticShader.setVec3("pointLight[0].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
			staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
			staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
			staticShader.setFloat("pointLight[0].constant", 0.8f);
			staticShader.setFloat("pointLight[0].linear", 0.009f);
			staticShader.setFloat("pointLight[0].quadratic", 0.032f);

			staticShader.setVec3("pointLight[1].position", glm::vec3(1.0, 100.0f, 500.0f));
			staticShader.setVec3("pointLight[1].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
			staticShader.setVec3("pointLight[1].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
			staticShader.setVec3("pointLight[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
			staticShader.setFloat("pointLight[1].constant", 1.0f);
			staticShader.setFloat("pointLight[1].linear", 0.009f);
			staticShader.setFloat("pointLight[1].quadratic", 0.032f);

			staticShader.setFloat("material_shininess", 32.0f);

	

		


		


		//segunda luz
		if (luz_casa) {

			staticShader.use();

			staticShader.setVec3("viewPos", camera.Position);
			staticShader.setVec3("dirLight.direction", lightDirection2);
			staticShader.setVec3("dirLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
			staticShader.setVec3("dirLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
			staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));

			staticShader.setVec3("pointLight[0].position", lightPosition);
			staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
			staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
			staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
			staticShader.setFloat("pointLight[0].constant", 0.8f);
			staticShader.setFloat("pointLight[0].linear", 0.009f);
			staticShader.setFloat("pointLight[0].quadratic", 0.032f);

			staticShader.setVec3("pointLight[1].position", glm::vec3(0.0, 0.0f, 0.0f));
			staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
			staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
			staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
			staticShader.setFloat("pointLight[1].constant", 1.0f);
			staticShader.setFloat("pointLight[1].linear", 0.009f);
			staticShader.setFloat("pointLight[1].quadratic", 0.032f);

			staticShader.setFloat("material_shininess", 32.0f);

		}

		
		//------------------------




		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//// Light
		glm::vec3 lightColor = glm::vec3(1.0f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);
		
		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);

		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);

		//MAR

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 10.0f, 500.0f));
		model = glm::scale(model, glm::vec3(250.0f, 200.0f, 50.0f));
		//tmp = model = glm::rotate(model, glm::radians(giroMonito), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		agua.Draw(staticShader);

		//-----------------------------

		//sombrilla
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 200.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0));
		staticShader.setMat4("model", model);
		sombrilla.Draw(staticShader);
		//--------------------

		// ---------- Toallas

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 225.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0));
		staticShader.setMat4("model", model);
		toalla.Draw(staticShader);
		//---------

		//sombrilla
		model = glm::translate(glm::mat4(1.0f), glm::vec3(80.0f, 1.0f, 200.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0));
		staticShader.setMat4("model", model);
		sombrilla.Draw(staticShader);
		//--------------------

		// ---------- Toallas

		model = glm::translate(glm::mat4(1.0f), glm::vec3(80.0f, 1.0f, 225.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0));
		staticShader.setMat4("model", model);
		toalla.Draw(staticShader);
		//---------

		//sombrilla
		model = glm::translate(glm::mat4(1.0f), glm::vec3(160.0f, 1.0f, 200.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0));
		staticShader.setMat4("model", model);
		sombrilla.Draw(staticShader);
		//--------------------

		// ---------- Toallas

		model = glm::translate(glm::mat4(1.0f), glm::vec3(160.0f, 1.0f, 225.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0));
		staticShader.setMat4("model", model);
		toalla.Draw(staticShader);
		//---------

		//-------silla

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-110.0f, 1.0f, 225.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		silla.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-90.0f, 1.0f, 225.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		silla.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-70.0f, 1.0f, 225.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		silla.Draw(staticShader);



		//----------

		//--- puesto de comida1

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-350.0f, 1.0f, 225.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		comida1.Draw(staticShader);

		//vendedor
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-370, 9, 225));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		mono.Draw(staticShader);


		//Comensales

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-340, 9, 225));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		mono.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-300, 9, 225));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		mono.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-260, 9, 225));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		mono.Draw(staticShader);
		//----


		// Personas jugando

		//jugando

		//izq
		model = glm::translate(glm::mat4(1.0f), glm::vec3(300, 9, 300));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		mono.Draw(staticShader);

		//centro
		model = glm::translate(glm::mat4(1.0f), glm::vec3(280.0, 9, 330));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		mono.Draw(staticShader);


		//der
		model = glm::translate(glm::mat4(1.0f), glm::vec3(300, 9, 360));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		mono.Draw(staticShader);



		//der
		model = glm::translate(glm::mat4(1.0f), glm::vec3(movMono1X, movMono1Y, 300));
		model = glm::rotate(model, glm::radians(giroMono1), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(giroMono2), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		mono.Draw(staticShader);

		//centro
		model = glm::translate(glm::mat4(1.0f), glm::vec3(110.0*distancia, 9, 360));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		mono.Draw(staticShader);


		//izq
		model = glm::translate(glm::mat4(1.0f), glm::vec3(90.0*distancia, 9, 360));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		mono.Draw(staticShader);

		//balon
		//model = glm::translate(glm::mat4(1.0f), glm::vec3(305, 1, 358));
		model = glm::translate(glm::mat4(1.0f), glm::vec3(balonX, balonY, balonZ));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));
		staticShader.setMat4("model", model);
		balon.Draw(staticShader);



		//Porteria1

		model = glm::translate(glm::mat4(1.0f), glm::vec3(250.0f, 1.0f, 320.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		porteria.Draw(staticShader);

		//Porteria2
		model = glm::translate(glm::mat4(1.0f), glm::vec3(480.0f, 1.0f, 340.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		porteria.Draw(staticShader);




		//----------------



		//fILAS DE CASAS Y PALMERAS

		tmp = model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 1, -300));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		staticShader.setMat4("model", model);
		casa.Draw(staticShader);


		model = glm::translate(tmp, glm::vec3(-300.0, 0, 0));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		staticShader.setMat4("model", model);
		casa.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(300.0, 0, 0));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		staticShader.setMat4("model", model);
		casa.Draw(staticShader);


		model = glm::translate(tmp, glm::vec3(-150.0, 0, 0));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.09f, 0.09f, 0.09f));
		staticShader.setMat4("model", model);
		palmera.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(150.0, 0, 0));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.09f, 0.09f, 0.09f));
		staticShader.setMat4("model", model);
		palmera.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------



		//-----------------------------------------------------------------------------------------------------------------

		//fILAS DE CASAS Y PALMERAS

		tmp = model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 1, -100));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		staticShader.setMat4("model", model);
		casa.Draw(staticShader);


		model = glm::translate(tmp, glm::vec3(-300.0, 0, 0));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		staticShader.setMat4("model", model);
		casa.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(300.0, 0, 0));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		staticShader.setMat4("model", model);
		casa.Draw(staticShader);


		model = glm::translate(tmp, glm::vec3(-150.0, 0, 0));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.09f, 0.09f, 0.09f));
		staticShader.setMat4("model", model);
		palmera.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(150.0, 0, 0));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.09f, 0.09f, 0.09f));
		staticShader.setMat4("model", model);
		palmera.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------

		//Perro
		model = glm::translate(glm::mat4(1.0f), glm::vec3(movAnimalx, 1.0f, 0.0f + movAnimalz));
		model = glm::scale(model, glm::vec3(0.125f, 0.125f, 0.125f));
		model = glm::rotate(model, glm::radians(orientaA), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		perro.Draw(staticShader);

		//Pato del perro
		model = glm::translate(glm::mat4(1.0f), glm::vec3(30.0f + movAnimalx, 1.0f, 0.0f + movAnimalz));
		model = glm::scale(model, glm::vec3(0.075f, 0.075f, 0.075f));
		model = glm::rotate(model, glm::radians(orientaA), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		pato.Draw(staticShader);

		//Pato
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f + movPatosx, 1.0f, 90.0f));
		model = glm::scale(model, glm::vec3(0.075f, 0.075f, 0.075f));
		model = glm::rotate(model, glm::radians(orientaPato), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		pato.Draw(staticShader);

		//Pato
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-110.0f + movPatosx, 1.0f, 60.0f));
		model = glm::scale(model, glm::vec3(0.075f, 0.075f, 0.075f));
		model = glm::rotate(model, glm::radians(orientaPato), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		pato.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f + movPatosx, 1.0f, 75.0f));
		model = glm::scale(model, glm::vec3(0.075f, 0.075f, 0.075f));
		model = glm::rotate(model, glm::radians(orientaPato), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		pato.Draw(staticShader);

		//Pato
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-110.0f + movPatosx, 1.0f, 45.0f));
		model = glm::scale(model, glm::vec3(0.075f, 0.075f, 0.075f));
		model = glm::rotate(model, glm::radians(orientaPato), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		pato.Draw(staticShader);

	

		//Ascera
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-459.0f, -1.0f, -530.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		ascera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-375.0f, -1.0f, -530.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		ascera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-291.0f, -1.0f, -530.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		ascera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-207.0f, -1.0f, -530.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		ascera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-123.0f, -1.0f, -530.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		ascera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-39.0f, -1.0f, -530.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		ascera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(45.0f, -1.0f, -530.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		ascera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(129.0f, -1.0f, -530.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		ascera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(213.0f, -1.0f, -530.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		ascera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(297.0f, -1.0f, -530.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		ascera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(381.0f, -1.0f, -530.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		ascera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(465.0f, -1.0f, -530.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		ascera.Draw(staticShader);

		// Personas en la playa

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0, 1, 225));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		mono.Draw(staticShader);

		//-----
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-110.0f, 10.0f, 230.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		mono.Draw(staticShader);



		model = glm::translate(glm::mat4(1.0f), glm::vec3(-90.0f, 10.0f, 230.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		mono.Draw(staticShader);



		model = glm::translate(glm::mat4(1.0f), glm::vec3(-70.0f, 10.0f, 230.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		staticShader.setMat4("model", model);
		mono.Draw(staticShader);
		//----------Monita nadadora-------------------
		

		model = glm::translate(glm::mat4(1.0f), glm::vec3(avanzaMona, 15.0f, 500.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::rotate(model, glm::radians(rotaMonita), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		mona.Draw(staticShader);

		//------------------------------------------

		//Carro
		model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(609.0f + movAuto_x, 1.0f, -471.0f + movAuto_z));
		tmp = model = glm::rotate(model, glm::radians(orienta), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		carro.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(8.5f, 2.5f, 12.9f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Izq delantera

		model = glm::translate(tmp, glm::vec3(-8.5f, 2.5f, 12.9f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Der delantera

		model = glm::translate(tmp, glm::vec3(-8.5f, 2.5f, -14.5f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Der trasera

		model = glm::translate(tmp, glm::vec3(8.5f, 2.5f, -14.5f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Izq trasera

		//Acera 1
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-459.0f, -1.0f, -530.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		acera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-375.0f, -1.0f, -530.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		acera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-291.0f, -1.0f, -530.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		acera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-207.0f, -1.0f, -530.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		acera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-123.0f, -1.0f, -530.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		acera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-39.0f, -1.0f, -530.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		acera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(45.0f, -1.0f, -530.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		acera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(129.0f, -1.0f, -530.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		acera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(213.0f, -1.0f, -530.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		acera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(297.0f, -1.0f, -530.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		acera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(381.0f, -1.0f, -530.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		acera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(465.0f, -1.0f, -530.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		acera.Draw(staticShader);

		//Asfalto
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-417.0f, -1.0f, -655.0f));
		model = glm::scale(model, glm::vec3(20.0f, 1.0f, 20.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		asfalto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-249.0f, -1.0f, -655.0f));
		model = glm::scale(model, glm::vec3(20.0f, 1.0f, 20.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		asfalto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-81.0f, -1.0f, -655.0f));
		model = glm::scale(model, glm::vec3(20.0f, 1.0f, 20.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		asfalto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(87.0f, -1.0f, -655.0f));
		model = glm::scale(model, glm::vec3(20.0f, 1.0f, 20.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		asfalto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(255.0f, -1.0f, -655.0f));
		model = glm::scale(model, glm::vec3(20.0f, 1.0f, 20.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		asfalto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(423.0f, -1.0f, -655.0f));
		model = glm::scale(model, glm::vec3(20.0f, 1.0f, 20.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		asfalto.Draw(staticShader);

		//Acera 2
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-459.0f, -1.0f, -781.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		acera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-375.0f, -1.0f, -781.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		acera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-291.0f, -1.0f, -781.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		acera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-207.0f, -1.0f, -781.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		acera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-123.0f, -1.0f, -781.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		acera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-39.0f, -1.0f, -781.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		acera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(45.0f, -1.0f, -781.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		acera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(129.0f, -1.0f, -781.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		acera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(213.0f, -1.0f, -781.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		acera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(297.0f, -1.0f, -781.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		acera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(381.0f, -1.0f, -781.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		acera.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(465.0f, -1.0f, -781.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		acera.Draw(staticShader);

		//Pasto
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-417.0f, -1.0f, -907.0f));
		model = glm::scale(model, glm::vec3(20.0f, 1.0f, 20.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		pasto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-249.0f, -1.0f, -907.0f));
		model = glm::scale(model, glm::vec3(20.0f, 1.0f, 20.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		pasto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-81.0f, -1.0f, -907.0f));
		model = glm::scale(model, glm::vec3(20.0f, 2.0f, 20.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		pasto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(87.0f, -1.0f, -907.0f));
		model = glm::scale(model, glm::vec3(20.0f, 1.0f, 20.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		pasto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(255.0f, -1.0f, -907.0f));
		model = glm::scale(model, glm::vec3(20.0f, 1.0f, 20.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		pasto.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(423.0f, -1.0f, -907.0f));
		model = glm::scale(model, glm::vec3(20.0f, 1.0f, 20.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		pasto.Draw(staticShader);


		//Mansiones
		model = glm::translate(glm::mat4(1.0f), glm::vec3(350.0f, 100.0f, -910.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		mansion.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, 100.0f, -910.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		mansion.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, 1.0f, -900.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.5f, 1.0f));
		//model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		casaFinal.Draw(staticShader);

		//--------
		
		model = glm::translate(glm::mat4(1.0f), glm::vec3(heliX, heliY, eliZ));
		//model = glm::translate(glm::mat4(1.0f), glm::vec3(250.0f, 50.0f, -650.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(eliGiro), glm::vec3(0.0f, 1.0f, 0.0));
		//270.0f
		staticShader.setMat4("model", model);
		heli.Draw(staticShader);


		
		// -------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario
		// -------------------------------------------------------------------------------------------------------------------------

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime * 8);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime*8);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime * 8);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime * 8);
	//To Configure Model
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		posZ++;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		posZ--;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		posX--;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		posX++;

	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		posY--;
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		posY++;

	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {

		sonido();
	}


	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		if(luz_casa) luz_casa = false;
		else luz_casa = true;
		
	}

	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {

		if (noche) {

			noche = false;
			nocheX = 0.2f;
			nocheY = 0.2f;
			nocheZ = 0.2f;
		}
			
			
		else {

			noche = true;
			nocheX = 1.0f;
			nocheY = 1.0f;
			nocheZ = 1.0f;
		}
	}
		



	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
		animacionesReset = true;

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		animacionPerro = true;

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		animacionPatos = true;



	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		animacionCarro = true;


	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		nada = true;


	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		juego = true;

	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		helicoptero = true;

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) 

		recorrido = true;



	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//To Save a KeyFrame
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}