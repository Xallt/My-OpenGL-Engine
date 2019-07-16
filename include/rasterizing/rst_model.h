#ifndef COMMON_RATERIZING_MODEL
#define COMMON_RATERIZING_MODEL

#include <glm/glm.hpp>
#include "../object.h"

using namespace glm;

class SingleModel : OpenglObject {
private:
  uint VAO, VBO, EBO;
  int vertexCount;
  int verticesSize, indicesSize;
  float* vertices;
  int* indices;
  ShaderProgram shader;
  Texture2D tex;
public:
  mat4 _transform = mat4(1.0);
  SingleModel(int vertexCount, int verticesSize, float* vertices, int indicesSize, int* indices, ShaderProgram &shader, Texture2D tex);
  SingleModel(int vertexCount, int verticesSize, float* vertices, ShaderProgram &shader, Texture2D tex);
  void init();
  void enableAttribute(int location, int size, GLenum type, int offset);
  void render(Camera* camera);
  void free();
  mat4 transform();
  mat4 transform(mat4 t);
};

#endif