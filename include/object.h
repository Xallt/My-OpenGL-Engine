#ifndef COMMON_MODEL
#define COMMON_MODEL

#include <GL/glew.h>

#include <vector>

#include "camera.h"
#include "shader.h"
#include "texture.h"

using namespace glm;

class OpenglObject {
public:
  std::vector<OpenglObject> children;
  virtual void init();
  virtual void render(Camera camera);
  virtual void update(double delta);
  virtual void free();
};

#endif