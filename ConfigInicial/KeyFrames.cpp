//Previo 12
//Alumno: Juárez Huerta Enrique
//Num. Cuenta: 319279207
//Fecha de Entrega: 09/Noviembre/2025


#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"


// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void Animation();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,2.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f,  0.0f),
	glm::vec3(0.0f,0.0f, 0.0f)
};

float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	  0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	  0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	  0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	 -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	  0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	 -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	 -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	 -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	 -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	 -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	 -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	 -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	 -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	  0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	  0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	  0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	  0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	  0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	 -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	  0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};


glm::vec3 Light1 = glm::vec3(0);

//--- VARIABLES DE ANIMACIÓN ---
float rotBall = 0.0f;
float rotDog = 0.0f;
int dogAnim = 0;

// Variables de posición del perro
float dogPosX = 0.0f, dogPosY = 0.0f, dogPosZ = 0.0f;

// Nuevas variables de rotación para cada parte (reemplazan a head, tail, FLegs, RLegs)
float rotHead = 0.0f;
float rotTail = 0.0f;
float rotF_LeftLeg = 0.0f;
float rotF_RightLeg = 0.0f;
float rotB_LeftLeg = 0.0f;
float rotB_RightLeg = 0.0f;
//---------------------------------


//KeyFrames
#define MAX_FRAMES 9
int i_max_steps = 2000;
int i_curr_steps = 0;

// Estructura FRAME actualizada para incluir todas las partes
typedef struct _frame {

	float rotDog;
	float rotDogInc;
	float dogPosX;
	float dogPosY;
	float dogPosZ;
	float incX;
	float incY;
	float incZ;
	float rotHead;
	float rotHeadInc;
	// --- Nuevas variables en FRAME ---
	float rotTail;
	float rotTailInc;
	float rotF_LeftLeg;
	float rotF_LeftLegInc;
	float rotF_RightLeg;
	float rotF_RightLegInc;
	float rotB_LeftLeg;
	float rotB_LeftLegInc;
	float rotB_RightLeg;
	float rotB_RightLegInc;
	// -----------------------------------

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

// saveFrame actualizado
void saveFrame(void)
{
	printf("frameindex %d\n", FrameIndex);

	KeyFrame[FrameIndex].dogPosX = dogPosX;
	KeyFrame[FrameIndex].dogPosY = dogPosY;
	KeyFrame[FrameIndex].dogPosZ = dogPosZ;
	KeyFrame[FrameIndex].rotDog = rotDog;
	KeyFrame[FrameIndex].rotHead = rotHead;

	// --- Guardar nuevas variables ---
	KeyFrame[FrameIndex].rotTail = rotTail;
	KeyFrame[FrameIndex].rotF_LeftLeg = rotF_LeftLeg;
	KeyFrame[FrameIndex].rotF_RightLeg = rotF_RightLeg;
	KeyFrame[FrameIndex].rotB_LeftLeg = rotB_LeftLeg;
	KeyFrame[FrameIndex].rotB_RightLeg = rotB_RightLeg;
	// --------------------------------

	FrameIndex++;
}

// resetElements actualizado
void resetElements(void)
{
	dogPosX = KeyFrame[0].dogPosX;
	dogPosY = KeyFrame[0].dogPosY;
	dogPosZ = KeyFrame[0].dogPosZ;
	rotHead = KeyFrame[0].rotHead;
	rotDog = KeyFrame[0].rotDog;

	// --- Resetear nuevas variables ---
	rotTail = KeyFrame[0].rotTail;
	rotF_LeftLeg = KeyFrame[0].rotF_LeftLeg;
	rotF_RightLeg = KeyFrame[0].rotF_RightLeg;
	rotB_LeftLeg = KeyFrame[0].rotB_LeftLeg;
	rotB_RightLeg = KeyFrame[0].rotB_RightLeg;
	// -----------------------------------
}

// interpolation actualizado
void interpolation(void)
{
	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].dogPosX - KeyFrame[playIndex].dogPosX) / i_max_steps;
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].dogPosY - KeyFrame[playIndex].dogPosY) / i_max_steps;
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].dogPosZ - KeyFrame[playIndex].dogPosZ) / i_max_steps;
	KeyFrame[playIndex].rotHeadInc = (KeyFrame[playIndex + 1].rotHead - KeyFrame[playIndex].rotHead) / i_max_steps;
	KeyFrame[playIndex].rotDogInc = (KeyFrame[playIndex + 1].rotDog - KeyFrame[playIndex].rotDog) / i_max_steps;

	// --- Interpolar nuevas variables ---
	KeyFrame[playIndex].rotTailInc = (KeyFrame[playIndex + 1].rotTail - KeyFrame[playIndex].rotTail) / i_max_steps;
	KeyFrame[playIndex].rotF_LeftLegInc = (KeyFrame[playIndex + 1].rotF_LeftLeg - KeyFrame[playIndex].rotF_LeftLeg) / i_max_steps;
	KeyFrame[playIndex].rotF_RightLegInc = (KeyFrame[playIndex + 1].rotF_RightLeg - KeyFrame[playIndex].rotF_RightLeg) / i_max_steps;
	KeyFrame[playIndex].rotB_LeftLegInc = (KeyFrame[playIndex + 1].rotB_LeftLeg - KeyFrame[playIndex].rotB_LeftLeg) / i_max_steps;
	KeyFrame[playIndex].rotB_RightLegInc = (KeyFrame[playIndex + 1].rotB_RightLeg - KeyFrame[playIndex].rotB_RightLeg) / i_max_steps;
	// -----------------------------------
}
// ===== Sistema de animación por Keyframes (tiempo absoluto) =====
struct Pose {
	float posX, posY, posZ;   // traslación del cuerpo
	float yaw;                // rotación del cuerpo sobre Y (equivale a rotDog)
	float head;               // rotación de cabeza
	float tail;               // rotación de cola
	float FL, FR, BL, BR;     // patas: FrontLeft, FrontRight, BackLeft, BackRight
};

enum SegmentFlags : unsigned {
	SEG_NONE = 0,
	SEG_WALK = 1 << 0,   // en este tramo se aplicará ciclo de caminata
	SEG_TURN = 1 << 1,   // tramo de giro
	SEG_SIT = 1 << 2    // tramo de "sentarse"
};

struct Keyframe {
	float t;        // tiempo en segundos (absoluto en la línea de tiempo)
	Pose  pose;     // pose "objetivo" en este instante
	unsigned flags; // bandera para el tramo *entre este KF y el siguiente*
};

// Easing suave (S-curve)
static inline float smooth01(float x) {
	x = glm::clamp(x, 0.0f, 1.0f);
	return x * x * (3.0f - 2.0f * x);
}

// LERP escalar y de Pose
static inline float lerp(float a, float b, float t) { return a + (b - a) * t; }

static Pose lerpPose(const Pose& a, const Pose& b, float t) {
	Pose r;
	r.posX = lerp(a.posX, b.posX, t);
	r.posY = lerp(a.posY, b.posY, t);
	r.posZ = lerp(a.posZ, b.posZ, t);
	// Yaw: simple lerp (si luego quieres shortest-arc, lo cambiamos)
	r.yaw = lerp(a.yaw, b.yaw, t);
	r.head = lerp(a.head, b.head, t);
	r.tail = lerp(a.tail, b.tail, t);
	r.FL = lerp(a.FL, b.FL, t);
	r.FR = lerp(a.FR, b.FR, t);
	r.BL = lerp(a.BL, b.BL, t);
	r.BR = lerp(a.BR, b.BR, t);
	return r;
}

// Variables de reproducción
float animTime = 0.0f;      // tiempo actual
float animSpeed = 1.0f;     // 1.0 = tiempo real, puedes acelerarlo
bool  animLoop = false;    // repetir al final si quieres

// Frecuencia del paso (para ciclo de patas en segmentos "walk")
float walkStepHz = 2.2f;    // ~2.2 ciclos/seg ? ajusta a gusto
float walkAmpDeg = 22.0f;   // amplitud de oscilación de patas

// Línea de tiempo (segundos) y poses destino.
// Marca de tramo = flags del segmento entre este KF y el siguiente.
static const Keyframe KFS[] = {
	// t,   Pose{ posX, posY, posZ, yaw, head, tail,  FL,  FR,  BL,  BR },  flags to next
	{ 0.0f, { 0.0f, 0.0f, 0.0f,   0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f }, SEG_WALK },
	// Camina +Z hasta z=+5
	{ 2.0f, { 0.0f, 0.0f, 5.0f,   0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f }, SEG_TURN },
	// Gira a la derecha (yaw -90°)
	{ 2.8f, { 0.0f, 0.0f, 5.0f, -90.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f }, SEG_WALK },
	// Camina +X hasta x=+5 (ya girado)
	{ 4.8f, { 5.0f, 0.0f, 5.0f, -90.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f }, SEG_SIT  },
	// Sentarse
	{ 6.0f, { 5.0f,-0.06f,5.0f, -90.0f, +8.0f,-25.0f, -10.0f,-10.0f, +35.0f,+35.0f }, SEG_NONE }
};
static const int KFS_COUNT = sizeof(KFS) / sizeof(KFS[0]);


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Enrique Juarez Huerta - Modificado", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);



	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");


	//models
	Model DogBody((char*)"Models/DogBody.obj");
	Model HeadDog((char*)"Models/HeadDog.obj");
	Model DogTail((char*)"Models/TailDog.obj");
	Model F_RightLeg((char*)"Models/F_RightLegDog.obj");
	Model F_LeftLeg((char*)"Models/F_LeftLegDog.obj");
	Model B_RightLeg((char*)"Models/B_RightLegDog.obj");
	Model B_LeftLeg((char*)"Models/B_LeftLegDog.obj");
	Model Piso((char*)"Models/piso.obj");
	Model Ball((char*)"Models/ball.obj");


	//KeyFrames
	// Bucle de inicialización de KeyFrames actualizado
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].dogPosX = 0;
		KeyFrame[i].dogPosY = 0;
		KeyFrame[i].dogPosZ = 0;
		KeyFrame[i].incX = 0;
		KeyFrame[i].incY = 0;
		KeyFrame[i].incZ = 0;
		KeyFrame[i].rotDog = 0;
		KeyFrame[i].rotDogInc = 0;
		KeyFrame[i].rotHead = 0;
		KeyFrame[i].rotHeadInc = 0;

		// --- Inicializar nuevas variables ---
		KeyFrame[i].rotTail = 0;
		KeyFrame[i].rotTailInc = 0;
		KeyFrame[i].rotF_LeftLeg = 0;
		KeyFrame[i].rotF_LeftLegInc = 0;
		KeyFrame[i].rotF_RightLeg = 0;
		KeyFrame[i].rotF_RightLegInc = 0;
		KeyFrame[i].rotB_LeftLeg = 0;
		KeyFrame[i].rotB_LeftLegInc = 0;
		KeyFrame[i].rotB_RightLeg = 0;
		KeyFrame[i].rotB_RightLegInc = 0;
		// ------------------------------------
	}


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);


	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);


	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		Animation();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);


		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp



		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);


		// Point light 1
		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);


		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);


		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));


		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);


		//Carga de modelo 
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

		// --- SECCIÓN DE RENDERIZADO DEL PERRO ACTUALIZADA ---
		// Body (El cuerpo es la base de las transformaciones)
		modelTemp = model = glm::translate(model, glm::vec3(dogPosX, dogPosY, dogPosZ));
		modelTemp = model = glm::rotate(model, glm::radians(rotDog), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DogBody.Draw(lightingShader);

		// Head
		model = modelTemp; // Reinicia a la posición del cuerpo
		model = glm::translate(model, glm::vec3(0.0f, 0.093f, 0.208f));
		model = glm::rotate(model, glm::radians(rotHead), glm::vec3(0.0f, 0.0f, 1.0f)); // Usa rotHead
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		HeadDog.Draw(lightingShader);

		// Tail 
		model = modelTemp; // Reinicia a la posición del cuerpo
		model = glm::translate(model, glm::vec3(0.0f, 0.026f, -0.288f));
		model = glm::rotate(model, glm::radians(rotTail), glm::vec3(0.0f, 0.0f, -1.0f)); // Usa rotTail
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DogTail.Draw(lightingShader);

		// Front Left Leg
		model = modelTemp; // Reinicia a la posición del cuerpo
		model = glm::translate(model, glm::vec3(0.112f, -0.044f, 0.074f));
		model = glm::rotate(model, glm::radians(rotF_LeftLeg), glm::vec3(-1.0f, 0.0f, 0.0f)); // Usa rotF_LeftLeg
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		F_LeftLeg.Draw(lightingShader);

		// Front Right Leg
		model = modelTemp; // Reinicia a la posición del cuerpo
		model = glm::translate(model, glm::vec3(-0.111f, -0.055f, 0.074f));
		// Eje X negativo para rotación frontal consistente
		model = glm::rotate(model, glm::radians(rotF_RightLeg), glm::vec3(-1.0f, 0.0f, 0.0f)); // Usa rotF_RightLeg
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		F_RightLeg.Draw(lightingShader);

		// Back Left Leg
		model = modelTemp; // Reinicia a la posición del cuerpo
		model = glm::translate(model, glm::vec3(0.082f, -0.046, -0.218));
		// Eje X negativo para rotación frontal consistente
		model = glm::rotate(model, glm::radians(rotB_LeftLeg), glm::vec3(-1.0f, 0.0f, 0.0f)); // Usa rotB_LeftLeg
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		B_LeftLeg.Draw(lightingShader);

		// Back Right Leg
		model = modelTemp; // Reinicia a la posición del cuerpo
		model = glm::translate(model, glm::vec3(-0.083f, -0.057f, -0.231f));
		// Eje X negativo para rotación frontal consistente
		model = glm::rotate(model, glm::radians(rotB_RightLeg), glm::vec3(-1.0f, 0.0f, 0.0f)); // Usa rotB_RightLeg
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		B_RightLeg.Draw(lightingShader);
		// --- FIN DE SECCIÓN DEL PERRO ---


		//model = glm::mat4(1);
		//glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		//model = glm::rotate(model, glm::radians(rotBall), glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//Ball.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glBindVertexArray(0);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)

		model = glm::mat4(1);
		model = glm::translate(model, pointLightPositions[0]);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);


		// Swap the screen buffers
		glfwSwapBuffers(window);
	}



	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
	// --- CONTROLES DEL PERRO ACTUALIZADOS ---
	// Posición del cuerpo
	if (keys[GLFW_KEY_H])
	{
		dogPosZ += 0.001f; // Aumenté la velocidad para que sea más notable
	}
	if (keys[GLFW_KEY_Y])
	{
		dogPosZ -= 0.001f;
	}
	if (keys[GLFW_KEY_G])
	{
		dogPosX -= 0.001f;
	}
	if (keys[GLFW_KEY_J])
	{
		dogPosX += 0.001f;
	}
	if (keys[GLFW_KEY_6])
	{
		dogPosY -= 0.001f;
	}
	if (keys[GLFW_KEY_7])
	{
		dogPosY += 0.001f;
	}

	// Rotación del cuerpo
	if (keys[GLFW_KEY_2])
	{
		rotDog += 0.5f;
	}
	if (keys[GLFW_KEY_3])
	{
		rotDog -= 0.5f;
	}

	// Rotación de Cabeza
	if (keys[GLFW_KEY_4])
	{
		rotHead += 0.005f; 
	}
	if (keys[GLFW_KEY_5])
	{
		rotHead -= 0.005f;
	}

	// Rotación de Cola
	if (keys[GLFW_KEY_U])
	{
		rotTail += 0.005f;
	}
	if (keys[GLFW_KEY_I])
	{
		rotTail -= 0.005f;
	}

	// Rotación Pata Delantera Derecha 
	if (keys[GLFW_KEY_O])
	{
		rotF_RightLeg += 0.005f;
	}
	if (keys[GLFW_KEY_P])
	{
		rotF_RightLeg -= 0.005f;
	}

	// Rotación Pata Delantera Izquierda
	if (keys[GLFW_KEY_Z])
	{
		rotF_LeftLeg += 0.005f;
	}
	if (keys[GLFW_KEY_X])
	{
		rotF_LeftLeg -= 0.005f;
	}

	// Rotación Pata Trasera Izquierda
	if (keys[GLFW_KEY_C])
	{
		rotB_LeftLeg += 0.005f;
	}
	if (keys[GLFW_KEY_V])
	{
		rotB_LeftLeg -= 0.005f;
	}

	// Rotación Pata Trasera Derecha
	if (keys[GLFW_KEY_B])
	{
		rotB_RightLeg += 0.005f;
	}
	if (keys[GLFW_KEY_N])
	{
		rotB_RightLeg -= 0.005f;
	}
	// --- FIN DE CONTROLES DEL PERRO ---


	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}

	// Controles de luz (Comentados porque usaban las mismas teclas que el perro)
	/*
	if (keys[GLFW_KEY_T])
	{
		pointLightPositions[0].x += 0.01f;
	}
	if (keys[GLFW_KEY_G])
	{
		pointLightPositions[0].x -= 0.01f;
	}
	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[0].y += 0.01f;
	}
	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[0].y -= 0.01f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[0].z -= 0.1f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[0].z += 0.01f;
	}
	*/
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// --- Controles de Animación (K y L) ---
	// NOTA: Tuve que mover la lógica de presionar K y L
	// al bloque 'if (action == GLFW_PRESS)' para que solo se activen una vez.
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_L) // Iniciar/detener animación
		{
			if (play == false && (FrameIndex > 1))
			{
				printf("Iniciando animacion...\n");
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
				printf("Animacion detenida.\n");
			}
		}

		if (key == GLFW_KEY_K) // Guardar Keyframe
		{
			if (FrameIndex < MAX_FRAMES)
			{
				printf("Guardando frame %d\n", FrameIndex);
				saveFrame();
			}
			else
			{
				printf("Max frames reached!\n");
			}
		}

		if (key == GLFW_KEY_SPACE) // Activar luz
		{
			active = !active;
			if (active)
			{
				Light1 = glm::vec3(0.2f, 0.8f, 1.0f);
			}
			else
			{
				Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
			}
		}
	}
	// --- Fin Controles de Animación ---


	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}
}

// Función Animation actualizada
void Animation() {
	// Avanza el tiempo de animación
	animTime += deltaTime * animSpeed;

	// Mantén dentro del rango [primerKF, últimoKF]
	const float tBeg = KFS[0].t;
	const float tEnd = KFS[KFS_COUNT - 1].t;

	if (animTime >= tEnd) {
		if (animLoop) {
			animTime = fmod(animTime - tBeg, (tEnd - tBeg)) + tBeg;
		}
		else {
			animTime = tEnd;
		}
	}
	if (animTime <= tBeg) animTime = tBeg;

	// Encuentra el segmento activo [i, i+1]
	int seg = KFS_COUNT - 2;
	for (int i = 0; i < KFS_COUNT - 1; ++i) {
		if (animTime >= KFS[i].t && animTime <= KFS[i + 1].t) {
			seg = i;
			break;
		}
	}

	const Keyframe& A = KFS[seg];
	const Keyframe& B = KFS[seg + 1];

	const float segDur = glm::max(0.0001f, B.t - A.t);
	float u = (animTime - A.t) / segDur;   // 0..1 lineal
	float s = smooth01(u);                 // easing suave

	// Interpolación base entre poses A y B
	Pose base = lerpPose(A.pose, B.pose, s);

	// Si el tramo es de caminar, superponemos ciclo de patas (procedural)
	if (A.flags & SEG_WALK) {
		float phase = animTime * (2.0f * 3.1415926f * walkStepHz);
		// Patrón cruzado: FL?BR y FR?BL
		float swing = walkAmpDeg * sinf(phase);
		base.FL = swing;          // Front Left
		base.BR = swing;          // Back Right
		base.FR = -swing;         // Front Right
		base.BL = -swing;         // Back Left

		// opcional: leve cabeceo/cola al caminar
		base.head = 0.5f * sinf(phase * 0.5f);
		base.tail = -6.0f + 3.0f * sinf(phase * 0.5f);
	}

	// Si el tramo es de giro, puedes amortiguar patas a neutro (ya lo hace el KF)
	// Si es de sentarse, usamos exactamente la pose B a medida que progresa (ya interpolado)

	// === Volcado a TUS variables globales actuales ===
	dogPosX = base.posX;
	dogPosY = base.posY;
	dogPosZ = base.posZ;

	rotDog = base.yaw;
	rotHead = base.head;
	rotTail = base.tail;

	rotF_LeftLeg = base.FL;
	rotF_RightLeg = base.FR;
	rotB_LeftLeg = base.BL;
	rotB_RightLeg = base.BR;
}


void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}