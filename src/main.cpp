#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "shader.h"
#include "mesh.h"

static const int SCREEN_WIDTH = 1280;
static const int SCREEN_HEIGHT = 720;

static const float FOV = 45.0f;

void FramebufferSizeCallback(GLFWwindow * Window, int Width, int Height);

float DeltaTime = 0.0f;
float LastFrame = 0.0f;

int main(int argc, char **argv) {
   // Initialize GLFW
   glfwInit();
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

   // Create a window with GLFW
   GLFWwindow * Window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL Renderer", 0, 0);
   if (!Window) {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return -1;
   }

   glfwMakeContextCurrent(Window);
   glfwSetFramebufferSizeCallback(Window, FramebufferSizeCallback);

   // Load OpenGL function pointers
   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cout << "Failed to initialize GLAD" << std::endl;
      return -1;
   }

   glEnable(GL_DEPTH_TEST);

   // Load shader
   Shader TextureShader("assets/shaders/textured.vs", "assets/shaders/textured.fs");

   // Load mesh
   Mesh Mesh;
   Mesh.LoadOBJ("assets/obj/backpack.obj");
   Mesh.LoadTexture("assets/textures/backpack.jpg");

   // Camera
   glm::vec3 CameraPosition = glm::vec3(0.0f, 0.0f, 8.0f);
   glm::vec3 CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
   glm::vec3 CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

   //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

   // Main loop
   while (!glfwWindowShouldClose(Window)) {
      float CurrentFrame = glfwGetTime();
      DeltaTime = CurrentFrame - LastFrame;
      LastFrame = CurrentFrame;

      // Process input
      if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
         glfwSetWindowShouldClose(Window, true);

      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      TextureShader.Activate();

      // View/Projection
      glm::mat4 ViewMatrix = glm::lookAt(CameraPosition, CameraPosition + CameraFront, CameraUp);
      glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(FOV), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

      TextureShader.SetMat4("View", ViewMatrix);
      TextureShader.SetMat4("Projection", ProjectionMatrix);

      // Model
      glm::mat4 ModelMatrix = glm::mat4(1.0f);
      ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));

      TextureShader.SetMat4("Model", ModelMatrix);

      // Draw mesh
      Mesh.Draw(TextureShader);

      // Swap buffers
      glfwSwapBuffers(Window);
      glfwPollEvents();
   }

   // Clear allocated resources
   glfwTerminate();

   return 0;
}

void FramebufferSizeCallback(GLFWwindow * Window, int Width, int Height) {
   glViewport(0, 0, Width, Height);
}