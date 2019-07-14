#ifndef COMMON_RAYMARCHING_SCENE
#define COMMON_RAYMARCHING_SCENE

#include <GL/glew.h>
#include <iostream>

#include "../camera.h"
#include "../shader.h"

class RaymarchingScene {
private:
  uint VBO, VAO;
  ShaderProgram *shader;
public:
  RaymarchingScene();
  void render(Camera &camera);
};

#endif