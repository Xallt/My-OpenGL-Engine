#ifndef COMMON_WINDOW
#define COMMON_WINDOW

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

using namespace glm;

class Window {
private:
  GLFWwindow* window;
  int _width, _height;
  const char* title;
public:
  Window(const char* title, int w, int h);

  void init(bool &success);

  int width();
  int height();
  float aspect();

  GLFWwindow* pointer();

  void setKeyCallback(GLFWkeyfun fun);

  bool shouldClose();
  void swapBuffers();
  bool isKeyPressed(int key);
  bool isMouseButtonPressed(int button);
  vec2 getMousePosition();
};

#endif