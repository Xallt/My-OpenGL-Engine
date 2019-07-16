#include "../include/shader.h"

ShaderProgram::ShaderProgram() {}

string ShaderProgram::readFile(const char* filename) {
  ifstream file(filename);
  stringstream sstr;
  sstr << file.rdbuf();
  file.close();
  return sstr.str();
}
uint ShaderProgram::compileVertexShader(bool &success) {
  uint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexPointer, NULL);
  glCompileShader(vertexShader);
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &_success);
  if (_success != GL_TRUE) {
    glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
    std::cout << "ERROR: Failed to compile vertex shader\n" << errorLog << '\n';
    success = false;
    return 0;
  }
  success = true;
  return vertexShader;
}
uint ShaderProgram::compileFragmentShader(bool &success) {
  uint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentPointer, NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &_success);
  if (_success != GL_TRUE) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
    std::cout << "ERROR: Failed to compile fragment shader\n" << errorLog << '\n';
    success = false;
    return 0;
  }
  success = true;
  return fragmentShader;
}
int ShaderProgram::loc(string name) {
  return glGetUniformLocation(id(), name.c_str());
}

void ShaderProgram::vertexFromFile(const char* vertexFile) {
  vertexString = readFile(vertexFile);
  vertexPointer = vertexString.c_str();
}
void ShaderProgram::fragmentFromFile(const char* fragmentFile) {
  fragmentString = readFile(fragmentFile);
  fragmentPointer = fragmentString.c_str();
}

void ShaderProgram::vertexFromString(string vertexString) {
  this->vertexString = vertexString;
  vertexPointer = vertexString.c_str();
}
void ShaderProgram::fragmentFromString(string fragmentString) {
  this->fragmentString = fragmentString;
  fragmentPointer = fragmentString.c_str();
}

void ShaderProgram::compile(bool &success) {
  int vertexShader = compileVertexShader(success);
  if (!success) {
    return;
  }
  int fragmentShader = compileFragmentShader(success);
  if (!success) {
    return;
  }
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &_success);
  if (_success != GL_TRUE) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, errorLog);
    std::cout << "ERROR; Failed to link the shader program\n" << errorLog << '\n';
    success = false;
    return;
  }
  success = true;

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  cout << "Shaders compiled\n";
}
uint ShaderProgram::id() {
  return shaderProgram;
}
void ShaderProgram::use() {
  glUseProgram(id());
}

void ShaderProgram::setUniform(string name, float x) {
  glUniform1f(loc(name), x);
}
void ShaderProgram::setUniform(string name, vec4 v) {
  glUniform4f(loc(name), v.x, v.y, v.z, v.w);
}
void ShaderProgram::setUniform(string name, vec3 v) {
  glUniform3f(loc(name), v.x, v.y, v.z);
}
void ShaderProgram::setUniform(string name, vec2 v) {
  glUniform2f(loc(name), v.x, v.y);
}
void ShaderProgram::setUniform(string name, mat4 v) {
  glUniformMatrix4fv(loc(name), 1, GL_FALSE, value_ptr(v));
}