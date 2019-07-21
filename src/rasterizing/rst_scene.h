#ifndef COMMON_RASTERIZING_SCENE
#define COMMON_RASTERIZING_SCENE

#include "../object.h"
#include "rst_model.h"
#include "../shader.h"
#include "../texture.h"
#include "glm/gtc/random.hpp"

class RasterizingScene: public Object {
private:
  Model* model;
  ShaderProgram *shader;
  Camera* camera;
  Window* window;
  Texture2D* tex;
  float* cubeVertices;
  vector<mat4> cubes;
  vector<vec3> movement;
  mat4 cameraTransform;
public:
  RasterizingScene(Camera* camera, Window* window);
  void init();
  void update(float delta);
  void render(Camera& camera);
  void free();
};

#endif