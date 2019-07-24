#define GLM_ENABLE_EXPERIMENTAL

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "camera.h"
#include "window.h"
#include "rasterizing/rst_scene.h"
#include "raymarching/rm_scene.h"
#include "raytracing/rt_scene.h"

// Global vars
double g_time, g_delta;
Camera *cam;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int modes) {
  // Close window on ESCAPE
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

void setCallbacks(Window &window) {
  window.setKeyCallback(keyCallback);
}

void mainCycle(Window &window, Camera* cam, Object* obj) {
  CameraController controller(cam, &window);
  g_time = glfwGetTime();
  g_delta = 0;
  float elapsed = 0;
  int iter = 0;

  obj->init();
  while (!window.shouldClose()) {
    glfwPollEvents();
    controller.update(g_delta);
    obj->update(g_delta);
    
    // Draw
    glClearColor(0.1484375, 0.203125, 0.52734375, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    obj->render(*cam);
    window.swapBuffers();

    g_delta = glfwGetTime() - g_time;
    g_time = glfwGetTime();
    elapsed += g_delta;
    ++iter;
  }

  cout << "FPS: ~" << (float)iter / elapsed << '\n';
  obj->free();
  glfwTerminate();
}

int main () {
  bool success;

  // Context declaration
  Window window("OpenGL newbie here", 1200, 900);
  window.init(success);
  if (!success) {
    return -1;
  }

  Camera* cam = new Camera(glm::radians(45.0), window.aspect());
  cam->transform(glm::translate(glm::vec3(0.0, 1.0, 3.0)));

  // Setting the main scene
  Object* obj = new RaytracingScene();

  setCallbacks(window);
  mainCycle(window, cam, obj);
  return 0;
}