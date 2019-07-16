#ifndef COMMON_CAMERA
#define COMMON_CAMERA

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/constants.hpp>

#include "window.h"

using namespace glm;

class Camera {
protected:
  mat4 cameraToWorld, worldToCamera;
  mat4 _projection;
  float _fov, _aspect, _near, _far;
  vec2 _cameraSize;
public:
  Camera(float fov, float aspect, float near = 0.1, float far = 100.0, mat4 transform = mat4(1.0));
  mat4 projection();
  mat4 view();

  float fov();
  float aspect();
  float near();
  float far();

  mat4 transform();
  void transform(mat4 tr);

  vec3 right();
  vec3 up();
  vec3 direction();
  vec3 position();

  vec2 cameraSize();
};

// WASD for movement, Shift to speed up
// Mouse control enabled for looking around
class CameraController {
private:
  Camera* camera;
  Window* window;
  vec2 oldMousePos;
  void keyMovement(double delta);
  void mouseMovement(double delta);
public:
  double speed = 1;
  CameraController(Camera *camera, Window* window);
  void update(double delta);
};

#endif