#include "../include/window.h"

Window::Window(const char* title, int w, int h): title(title), _width(w), _height(h) {}

void Window::init(bool &success) {
  if (glfwInit() != GLFW_TRUE) {
    std::cout << "ERROR: Failed to initialize GLFW\n";
    success = false;
    return;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  window = glfwCreateWindow(_width, _height, title, nullptr, nullptr);
  if (!window) {
    std::cout << "ERROR: Failed to initialize window\n";
    glfwTerminate();
    success = false;
    return;
  }
  glfwMakeContextCurrent(window);

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    std::cout << "Failed to initialize GLEW\n";
    glfwTerminate();
    success = false;
    return;
  }

  glViewport(0, 0, _width, _height);
  glEnable(GL_DEPTH_TEST);
  success = true;
}

int Window::width() {
  return _width;
}
int Window::height() {
  return _height;
}
float Window::aspect() {
  return (float)_width / _height;
}

GLFWwindow* Window::pointer() {
  return window;
}

void Window::setKeyCallback(GLFWkeyfun fun) {
  glfwSetKeyCallback(window, fun);
}

bool Window::shouldClose() {
  return glfwWindowShouldClose(window);
}
void Window::swapBuffers() {
  glfwSwapBuffers(window);
}
bool Window::isKeyPressed(int key) {
  return glfwGetKey(window, key) == GLFW_PRESS;
}
vec2 Window::getMousePosition() {
  double x, y;
  glfwGetCursorPos(window, &x, &y);
  return glm::vec2(x, y);
}