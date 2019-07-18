#ifndef COMMON_RAYTRACING_SCENE
#define COMMON_RAYTRACING_SCENE

#include "../object.h"
#include "../shader.h"

class RaytracingScene: public Object {
private:
  uint VAO, VBO;
  ShaderProgram* shader;
public:
  void init();
  void update(float delta);
  void render(Camera& camera);
  void free();
};

#endif