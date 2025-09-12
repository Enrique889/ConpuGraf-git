//Juárez Huerta Enrique
//Número de Cuenta: 319279207
//Práctica 5
//Fecha de Entrega: 12/Sept/2025
#include<iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// ShadersS
#include "Shader.h"

void Inputs(GLFWwindow *window);


const GLint WIDTH = 1200, HEIGHT = 800;

//For Keyboard
float	movX = 0.0f,
movY = 0.0f,
movZ = -5.0f,
rot = 0.0f;

//For model
float	hombro = 0.0f,
codo = 0.0f,
muneca = 0.0f,
dedo1A=0.0f,
dedo1B = 0.0f,
dedo2A = 0.0f,
dedo2B = 0.0f,
dedo3A = 0.0f,
dedo3B = 0.0f,
dedo4A = 0.0f,
dedo4B = 0.0f,
dedo5A = 0.0f,
dedo5B = 0.0f
;


int main() {
	glfwInit();
	//Verificación de compatibilidad 
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Enrique Juarez Huerta", nullptr, nullptr);

	int screenWidth, screenHeight;

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	//Verificación de errores de creacion  ventana
	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificación de errores de inicialización de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}


	// Define las dimensiones del viewport
	glViewport(0, 0, screenWidth, screenHeight);


	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST);

	// enable alpha support
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Build and compile our shader program
	Shader ourShader("Shader/core.vs", "Shader/core.frag");


	// Set up vertex data (and buffer(s)) and attribute pointers
	// use with Orthographic Projection


	

	// use with Perspective Projection
	float vertices[] = {
		-0.5f, -0.5f, 0.5f, 
		0.5f, -0.5f, 0.5f,  
		0.5f,  0.5f, 0.5f,  
		0.5f,  0.5f, 0.5f,  
		-0.5f,  0.5f, 0.5f, 
		-0.5f, -0.5f, 0.5f, 
		
	    -0.5f, -0.5f,-0.5f, 
		 0.5f, -0.5f,-0.5f, 
		 0.5f,  0.5f,-0.5f, 
		 0.5f,  0.5f,-0.5f, 
	    -0.5f,  0.5f,-0.5f, 
	    -0.5f, -0.5f,-0.5f, 
		
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  -0.5f, 0.5f,
      
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f, 
		0.5f, -0.5f,  0.5f, 
		0.5f, -0.5f,  0.5f, 
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,
		
		-0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f, 
		0.5f,  0.5f,  0.5f, 
		0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};




	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3* sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

	
	glm::mat4 projection=glm::mat4(1);

	projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);//FOV, Radio de aspecto,znear,zfar
	glm::vec3 color= glm::vec3(0.0f, 0.0f, 1.0f);
	while (!glfwWindowShouldClose(window))
	{
		
		Inputs(window);
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);


		
		ourShader.Use();
		glm::mat4 model=glm::mat4(1);
		glm::mat4 view=glm::mat4(1);
		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp
		glm::mat4 modelTemp2 = glm::mat4(1.0f); //Temp



		//View set up 
		view = glm::translate(view, glm::vec3(movX,movY, movZ));
		view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		
		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");
		GLint uniformColor = ourShader.uniformColor;

		glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	

		glBindVertexArray(VAO);
		
		//Model Bicep
		model = glm::rotate(model, glm::radians(hombro), glm::vec3(0.0f, 0.0, 1.0f)); //hombro
		modelTemp = model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 1.0f, 1.0f));
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//A

		//Model Antebrazo
		model = glm::translate(modelTemp, glm::vec3(1.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(codo), glm::vec3(0.0f, 1.0f, 0.0f));
		modelTemp = model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//A

		//Model palma
		model = glm::translate(modelTemp, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(muneca), glm::vec3(1.0f, 0.0f, 0.0f));
		modelTemp = model = glm::translate(model, glm::vec3(0.250f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.50f, 1.0f, 1.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//A

		glm::mat4 basePalma = modelTemp;

		//dedo1A
		glm::mat4 d1 = glm::translate(basePalma, glm::vec3(0.250f, 0.350f, 0.375f));
		d1 = glm::rotate(d1, glm::radians(dedo1A), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 d1Temp = d1 = glm::translate(d1, glm::vec3(0.50f, 0.0f, 0.0f));
		d1 = glm::scale(d1, glm::vec3(1.0f, 0.30f, 0.25f));
		color = glm::vec3(0.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(d1));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//dedo1B
		d1 = glm::translate(d1Temp, glm::vec3(0.50f, 0.0f, 0.0f));
		d1 = glm::rotate(d1, glm::radians(dedo1B), glm::vec3(0.0f, 0.0f, 1.0f));
		d1Temp = d1 = glm::translate(d1, glm::vec3(0.50f, 0.0f, 0.0f));
		d1 = glm::scale(d1, glm::vec3(1.0f, 0.30f, 0.25f));
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(d1));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//dedo2A
		glm::mat4 modelD3 = glm::translate(basePalma, glm::vec3(0.250f, 0.350f, 0.125f));
		modelD3 = glm::rotate(modelD3, glm::radians(dedo2A), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 modelD3Temp = modelD3 = glm::translate(modelD3, glm::vec3(0.50f, 0.0f, 0.0f));
		modelD3 = glm::scale(modelD3, glm::vec3(1.0f, 0.30f, 0.25f));
		color = glm::vec3(0.7f, 0.7f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelD3));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//dedo2B
		modelD3 = glm::translate(modelD3Temp, glm::vec3(0.50f, 0.0f, 0.0f));
		modelD3 = glm::rotate(modelD3, glm::radians(dedo2B), glm::vec3(0.0f, 0.0f, 1.0f));
		modelD3Temp = modelD3 = glm::translate(modelD3, glm::vec3(0.50f, 0.0f, 0.0f));
		modelD3 = glm::scale(modelD3, glm::vec3(1.0f, 0.30f, 0.25f));
		color = glm::vec3(0.7f, 0.0f, 0.7f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelD3));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//dedo3A
		glm::mat4 modelD4 = glm::translate(basePalma, glm::vec3(0.250f, 0.350f, -0.125f));
		modelD4 = glm::rotate(modelD4, glm::radians(dedo3A), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 modelD4Temp = modelD4 = glm::translate(modelD4, glm::vec3(0.50f, 0.0f, 0.0f));
		modelD4 = glm::scale(modelD4, glm::vec3(1.0f, 0.30f, 0.25f));
		color = glm::vec3(0.0f, 0.7f, 0.7f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelD4));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//dedo3B
		modelD4 = glm::translate(modelD4Temp, glm::vec3(0.50f, 0.0f, 0.0f));
		modelD4 = glm::rotate(modelD4, glm::radians(dedo3B), glm::vec3(0.0f, 0.0f, 1.0f));
		modelD4Temp = modelD4 = glm::translate(modelD4, glm::vec3(0.50f, 0.0f, 0.0f));
		modelD4 = glm::scale(modelD4, glm::vec3(1.0f, 0.30f, 0.25f));
		color = glm::vec3(0.3f, 0.5f, 0.9f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelD4));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//dedo4A
		glm::mat4 modelD5 = glm::translate(basePalma, glm::vec3(0.250f, 0.350f, -0.375f));
		modelD5 = glm::rotate(modelD5, glm::radians(dedo4A), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 modelD5Temp = modelD5 = glm::translate(modelD5, glm::vec3(0.50f, 0.0f, 0.0f));
		modelD5 = glm::scale(modelD5, glm::vec3(1.0f, 0.30f, 0.25f));
		color = glm::vec3(0.9f, 0.5f, 0.3f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelD5));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//dedo4B
		modelD5 = glm::translate(modelD5Temp, glm::vec3(0.50f, 0.0f, 0.0f));
		modelD5 = glm::rotate(modelD5, glm::radians(dedo4B), glm::vec3(0.0f, 0.0f, 1.0f));
		modelD5Temp = modelD5 = glm::translate(modelD5, glm::vec3(0.50f, 0.0f, 0.0f));
		modelD5 = glm::scale(modelD5, glm::vec3(1.0f, 0.30f, 0.25f));
		color = glm::vec3(0.2f, 0.9f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelD5));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//dedo5A
		glm::mat4 modelD6 = glm::translate(basePalma,glm::vec3(0.18f, -0.505f, 0.305f));
		modelD6 = glm::rotate(modelD6, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelD6 = glm::rotate(modelD6, glm::radians(dedo5A), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 modelD6Temp = modelD6 = glm::translate(modelD6, glm::vec3(0.50f, 0.0f, 0.0f));
		modelD6 = glm::scale(modelD6, glm::vec3(0.50f, 0.30f, 0.25f));
		color = glm::vec3(0.8f, 0.8f, 0.2f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelD6));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//dedo5B
		modelD6 = glm::translate(modelD6Temp, glm::vec3(0.10f, 0.0f, 0.0f));
		modelD6 = glm::rotate(modelD6, glm::radians(dedo5B), glm::vec3(0.0f, 1.0f, 0.0f));
		modelD6Temp = modelD6 = glm::translate(modelD6, glm::vec3(0.45f, 0.0f, 0.0f));
		modelD6 = glm::scale(modelD6, glm::vec3(0.50f, 0.30f, 0.25f));
		color = glm::vec3(0.2f, 0.9f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelD6));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);

		
		// Swap the screen buffers
		glfwSwapBuffers(window);
	
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);


	glfwTerminate();
	return EXIT_SUCCESS;
 }

void Inputs(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		movX -= 0.0008f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movX += 0.0008f;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		movY -= 0.0008f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		movY += 0.0008f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movZ += 0.0008f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movZ -= 0.0008f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		rot -= 0.18f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		rot += 0.18f;
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && hombro < 90.0f)
		hombro += 0.018f;
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && hombro > -90.0f)
		hombro -= 0.018f;
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && codo < 0.0f)
		codo += 0.018f;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && codo > -120.0f)
		codo -= 0.018f;
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && muneca < 90.0f)
		muneca += 0.018f;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && muneca > -180.0f)
		muneca -= 0.018f;
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS && dedo1A < 0.0f)
		dedo1A += 0.018f;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS && dedo1A > -55.0f)
		dedo1A -= 0.018f;
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS && dedo1B < 0.0f)
		dedo1B += 0.018f;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && dedo1B > -155.0f)
		dedo1B -= 0.018f;
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS && dedo2A < 0.0f)
		dedo2A += 0.018f;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS && dedo2A > -55.0f)
		dedo2A -= 0.018f;
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS && dedo2B < 0.0f)
		dedo2B += 0.018f;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && dedo2B > -155.0f)
		dedo2B -= 0.018f;
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS && dedo3A < 0.0f)
		dedo3A += 0.018f;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS && dedo3A > -55.0f)
		dedo3A -= 0.018f;
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS && dedo3B < 0.0f)
		dedo3B += 0.018f;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && dedo3B > -155.0f)
		dedo3B -= 0.018f;
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS && dedo4A < 0.0f)
		dedo4A += 0.018f;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS && dedo4A > -55.0f)
		dedo4A -= 0.018f;
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS && dedo4B < 0.0f)
		dedo4B += 0.018f;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && dedo4B > -155.0f)
		dedo4B -= 0.018f;
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS && dedo5A > 0.0f)
		dedo5A -= 0.028f;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS && dedo5A < 90.0f)
		dedo5A += 0.028f;
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS && dedo5B >0.0f)
		dedo5B -= 0.028f;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && dedo5B < 135.0f)
		dedo5B += 0.028f;
}


