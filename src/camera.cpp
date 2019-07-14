#include "../common/camera.h"

Camera::Camera(float fov, float aspect, float near, float far, mat4 transform): 
    _fov(fov), _aspect(aspect), _near(near), _far(far) 
{
  cameraToWorld = transform;
  worldToCamera = inverse(cameraToWorld);
  _projection = perspective(fov, aspect, near, far);
  _cameraSize = vec2(2 * tan(fov / 2) * aspect, 2 * tan(fov / 2));
}
mat4 Camera::projection() {
  return _projection;
}
mat4 Camera::view() {
  return worldToCamera;
}
float Camera::fov() {
  return _fov;
}
float Camera::aspect() {
  return _aspect;
}
float Camera::near() {
  return _near;
}
float Camera::far() {
  return _far;
}

mat4 Camera::transform() {
  return cameraToWorld;
}
void Camera::transform(mat4 tr) {
  cameraToWorld = tr;
  worldToCamera = inverse(cameraToWorld);
}

vec3 Camera::right() {
  return vec3(cameraToWorld[0]);
}
vec3 Camera::up() {
  return vec3(cameraToWorld[1]);
}
vec3 Camera::direction() {
  return -vec3(cameraToWorld[2]);
}
vec3 Camera::position() {
  return vec3(cameraToWorld[3]);
}
vec2 Camera::cameraSize() {
  return _cameraSize;
}

CameraController::CameraController(Camera *camera, Window* window): camera(camera), window(window) {
  glfwSetInputMode(window->pointer(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetInputMode(window->pointer(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
  oldMousePos = vec2((float)window->width() / 2, (float)window->height() / 2);
}
void CameraController::update(double delta) {
  keyMovement(delta);
  mouseMovement(delta);
}

void CameraController::keyMovement(double delta) {
  vec3 movement = vec3(0.0, 0.0, 0.0);
  if (window->isKeyPressed(GLFW_KEY_W)) {
    movement += camera->direction();
  }
  if (window->isKeyPressed(GLFW_KEY_S)) {
    movement += -camera->direction();
  }
  if (window->isKeyPressed(GLFW_KEY_D)) {
    movement += camera->right();
  }
  if (window->isKeyPressed(GLFW_KEY_A)) {
    movement += -camera->right();
  }
  if (length(movement) < 0.1) {
    return;
  }
  movement = normalize(movement);
  movement *= speed * delta;
  if (window->isKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
    movement *= 4;
  }
  camera->transform(translate(movement) * camera->transform());
}
void CameraController::mouseMovement(double delta) {
  float ang = angle(camera->direction(), vec3(0.0, 1.0, 0.0));
  vec2 mousePos = window->getMousePosition(), mouseMove = mousePos - oldMousePos;
  double xc = mouseMove.x / window->width() * camera->cameraSize().x,
          yc = -mouseMove.y / window->height() * camera->cameraSize().y;
  vec3 newCenter = vec3(camera->transform() * vec4(xc, yc, -1.0, 1.0));
  camera->transform(inverse(lookAt(camera->position(), newCenter, vec3(0.0, 1.0, 0.0))));

  oldMousePos = mousePos;
}