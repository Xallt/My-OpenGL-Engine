#ifndef COMMON_RATERIZING_MODEL
#define COMMON_RATERIZING_MODEL

#include <glm/glm.hpp>
#include "../object.h"
#include "../shader.h"
#include "../texture.h"

using namespace glm;

class Model : public Object {
private:
  uint VAO, VBO;
  vector<vec3> position;
  vector<vec2> texCoord;
  float* verticesData;
  ShaderProgram* shader;
  Texture2D* tex;

  void computeData();
public:
  mat4 _transform = mat4(1.0);
  Model(int vertexCount, float* vertices, ShaderProgram* shader, Texture2D* tex);
  void update(float delta);
  void init();
  void render(Camera& camera);
  void free();

  void enableAttribute(int location, int size, GLenum type, int offset);
  int vertexCount();

  mat4 transform();
  mat4 transform(mat4 t);
};

#endif