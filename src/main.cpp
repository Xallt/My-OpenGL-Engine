#define GLM_ENABLE_EXPERIMENTAL

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "../include/camera.h"
#include "../include/window.h"
#include "../include/rasterizing/rst_scene.h"
#include "../include/raymarching/rm_scene.h"
#include "../include/raytracing/rt_scene.h"

// This way it's more convenient
#define int GLint
#define float GLfloat
#define uint GLuint

// Global vars
double g_time, g_delta;
Camera *g_camera;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int modes) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

void setCallbacks(Window &window) {
  window.setKeyCallback(keyCallback);
}

void mainCycle(Window &window) {
  g_camera = new Camera(glm::radians(45.0), window.aspect());
  g_camera->transform(glm::translate(glm::vec3(0.0, 1.0, 3.0)));
  CameraController controller(g_camera, &window);

  Object* obj = new RaytracingScene();
  obj->init();

  g_time = glfwGetTime();
  g_delta = 0;
  while (!window.shouldClose()) {
    glfwPollEvents();
    controller.update(g_delta);
    obj->update(g_delta);
    
    glClearColor(0.1484375, 0.203125, 0.52734375, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    obj->render(*g_camera);

    window.swapBuffers();

    g_delta = glfwGetTime() - g_time;
    g_time = glfwGetTime();
  }

  obj->free();
  glfwTerminate();
}

int main () {
  bool success;

  // Context declaration
  Window window("OpenGL newbie here", 800, 600);
  window.init(success);
  if (!success) {
    return -1;
  }
  setCallbacks(window);
  mainCycle(window);
  return 0;
}