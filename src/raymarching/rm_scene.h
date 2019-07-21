#ifndef COMMON_RAYMARCHING_SCENE
#define COMMON_RAYMARCHING_SCENE

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>

using namespace glm;

#include "../camera.h"
#include "rm_shader.h"
#include "../object.h"

class RaymarchingScene: public Object {
private:
  uint VBO, VAO;
  RaymarchingShader* shader;
public:
  RaymarchingScene();
  void init();
  void update(float delta);
  void render(Camera &camera);
  void free();
};

#endif