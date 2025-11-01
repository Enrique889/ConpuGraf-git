//Juarez Huerta Enrique
//Numero de Cuenta: 319279207
//Práctica 9
//24 de Octubre de 2025

#include <string>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SOIL2/SOIL2.h"
#include "stb_image.h"

const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

Camera camera(glm::vec3(0.0f, 2.0f, 10.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

float cycleAngle = 0.0f;
const float cycleRadius = 10.0f;

glm::vec3 sunPos(0.0f, 0.0f, 0.0f);
glm::vec3 moonPos(0.0f, 0.0f, 0.0f);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Enrique Juarez Huerta", nullptr, nullptr);
    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");
    Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

    Model dog((char*)"Models/RedDog.obj");
    Model stove((char*)"Models/uploads_files_5702623_GasStoveWithOven.obj");
    Model fridge((char*)"Models/Samsung_Fridge_low.obj");
    Model cabinet((char*)"Models/Cajonera.obj");
    Model lamp2((char*)"Models/uploads_files_3418524_248664_TolomeoMegaFloorLamp.obj");
    

    float vertices[] = {
        -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f, -0.5f,  0.5f
    };

    GLuint VBO, lampVAO;
    glGenVertexArrays(1, &lampVAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(lampVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        DoMovement();

        sunPos.x = 0.0f;
        sunPos.y = cycleRadius * glm::cos(cycleAngle);
        sunPos.z = cycleRadius * glm::sin(cycleAngle);

        moonPos = -sunPos;

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(
            camera.GetZoom(),
            (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
            0.1f,
            100.0f
        );

        glm::mat4 view = camera.GetViewMatrix();

        shader.Use();

        glUniform3fv(glGetUniformLocation(shader.Program, "viewPos"), 1, &camera.GetPosition()[0]);

        glUniform3fv(glGetUniformLocation(shader.Program, "lights[0].position"), 1, &sunPos[0]);
        glUniform3f(glGetUniformLocation(shader.Program, "lights[0].ambient"), 0.3f, 0.3f, 0.3f);
        glUniform3f(glGetUniformLocation(shader.Program, "lights[0].diffuse"), 1.0f, 1.0f, 0.9f);
        glUniform3f(glGetUniformLocation(shader.Program, "lights[0].specular"), 1.0f, 1.0f, 1.0f);

        glUniform3fv(glGetUniformLocation(shader.Program, "lights[1].position"), 1, &moonPos[0]);
        glUniform3f(glGetUniformLocation(shader.Program, "lights[1].ambient"), 0.1f, 0.1f, 0.15f);
        glUniform3f(glGetUniformLocation(shader.Program, "lights[1].diffuse"), 0.2f, 0.2f, 0.4f);
        glUniform3f(glGetUniformLocation(shader.Program, "lights[1].specular"), 0.3f, 0.3f, 0.3f);

        glUniformMatrix4fv(
            glGetUniformLocation(shader.Program, "projection"),
            1, GL_FALSE,
            glm::value_ptr(projection)
        );
        glUniformMatrix4fv(
            glGetUniformLocation(shader.Program, "view"),
            1, GL_FALSE,
            glm::value_ptr(view)
        );

        glm::vec3 lampMin = lamp2.getBBoxMin();
        glm::vec3 lampMax = lamp2.getBBoxMax();
        glm::vec3 lampCenter = 0.5f * (lampMin + lampMax);

        float lampBaseY = lampMin.y;
        float desiredFloorY = -0.20f;
        float currentBaseAfterCenter = lampBaseY - lampCenter.y;
        float deltaYLamp = desiredFloorY - currentBaseAfterCenter;

        glm::vec3 localOffset(
            -lampCenter.x,
            -lampCenter.y + deltaYLamp,
            -lampCenter.z
        );

        float lampScale = 0.001f;
        glm::vec3 worldPos(1.2f, 0.0f, 1.2f);

        glm::mat4 lampModel = glm::mat4(1.0f);
        lampModel = glm::translate(lampModel, worldPos);
        lampModel = glm::scale(lampModel, glm::vec3(lampScale));
        lampModel = glm::translate(lampModel, localOffset);

        float tipLocalY = (lampMax.y - lampCenter.y + deltaYLamp);
        glm::vec3 spotPos = worldPos + lampScale * glm::vec3(0.0f, tipLocalY, 0.0f);
        glm::vec3 spotDir = glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f));

        glUniform3fv(glGetUniformLocation(shader.Program, "spotLight.position"), 1, &spotPos[0]);
        glUniform3fv(glGetUniformLocation(shader.Program, "spotLight.direction"), 1, &spotDir[0]);
        glUniform3f(glGetUniformLocation(shader.Program, "spotLight.ambient"), 0.8f, 0.8f, 0.5f);
        glUniform3f(glGetUniformLocation(shader.Program, "spotLight.diffuse"), 0.80f, 0.3f, 0.8f);
        glUniform3f(glGetUniformLocation(shader.Program, "spotLight.specular"), 0.80f, 0.3f, 0.7f);
        glUniform1f(glGetUniformLocation(shader.Program, "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(shader.Program, "spotLight.linear"), 0.14f);
        glUniform1f(glGetUniformLocation(shader.Program, "spotLight.quadratic"), 0.07f);
        glUniform1f(glGetUniformLocation(shader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
        glUniform1f(glGetUniformLocation(shader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(17.5f)));

        glm::mat4 model(1.0f);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.2f, -0.20f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        stove.Draw(shader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.350f, -0.20f, 0.0f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 1, 0));
        model = glm::scale(model, glm::vec3(0.001f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        cabinet.Draw(shader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.53f, -0.20f, 0.0f));
        model = glm::scale(model, glm::vec3(0.0250f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        fridge.Draw(shader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.8f, 0.0f, 0.6f));
        model = glm::scale(model, glm::vec3(0.5f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        dog.Draw(shader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.70f, 0.0f, 1.20f));
        model = glm::scale(model, glm::vec3(0.5f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        dog.Draw(shader);

        glUniformMatrix4fv(
            glGetUniformLocation(shader.Program, "model"),
            1, GL_FALSE,
            glm::value_ptr(lampModel)
        );
        lamp2.Draw(shader);

        lampShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        glBindVertexArray(lampVAO);

        glm::mat4 mSol = glm::mat4(1.0f);
        mSol = glm::translate(mSol, sunPos);
        mSol = glm::scale(mSol, glm::vec3(0.3f));
        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mSol));
        glUniform3f(glGetUniformLocation(lampShader.Program, "lampColor"), 1.0f, 1.0f, 0.9f);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glm::mat4 mLuna = glm::mat4(1.0f);
        mLuna = glm::translate(mLuna, moonPos);
        mLuna = glm::scale(mLuna, glm::vec3(0.2f));
        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(mLuna));
        glUniform3f(glGetUniformLocation(lampShader.Program, "lampColor"), 0.5f, 0.5f, 1.0f);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void DoMovement()
{
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
        camera.ProcessKeyboard(RIGHT, deltaTime);

    if (keys[GLFW_KEY_O])
        cycleAngle += 0.5f * deltaTime;
    if (keys[GLFW_KEY_L])
        cycleAngle -= 0.5f * deltaTime;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
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
    GLfloat yOffset = lastY - yPos;

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}
