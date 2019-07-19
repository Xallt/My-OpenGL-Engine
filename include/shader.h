#ifndef COMMON_SHADER
#define COMMON_SHADER

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace glm;
using namespace std;

class ShaderProgram {
protected:
  string readFile(const char* filename);
  char errorLog[512];
  int _success;
  string vertexString, fragmentString;
  const char *vertexPointer, *fragmentPointer;
  uint shaderProgram;
  uint compileVertexShader(bool &success);
  uint compileFragmentShader(bool &success);
  int loc(string name);
public:
  ShaderProgram();

  void vertexFromFile(const char* vertexFile);
  void fragmentFromFile(const char* fragmentFile);

  void vertexFromString(string vertexString);
  void fragmentFromString(string fragmentString);

  void compile(bool &success);

  uint id();
  void use();

  void setUniform(string name, float x);
  void setUniform(string name, int x);
  void setUniform(string name, vec4 v);
  void setUniform(string name, vec3 v);
  void setUniform(string name, vec2 v);
  void setUniform(string name, mat4 v);
};

#endif