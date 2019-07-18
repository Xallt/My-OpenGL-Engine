#ifndef COMMON_MODEL
#define COMMON_MODEL

#include <GL/glew.h>

#include <vector>

#include "camera.h"

using namespace glm;

class Object {
public:
  std::vector<Object> children;
  virtual void init();
  virtual void render(Camera& camera);
  virtual void update(float delta);
  virtual void free();
};

#endif