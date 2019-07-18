#include "../include/rasterizing/rst_scene.h"

RasterizingScene::RasterizingScene(Camera* _camera, Window* _window): camera(_camera), window(_window) {}

void RasterizingScene::init() {
  cubeVertices = new float[36*5] {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
      0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
  };
  shader = new ShaderProgram();
  shader->fragmentFromFile("shaders/rasterizing/frag.glsl");
  shader->vertexFromFile("shaders/rasterizing/vert.glsl");
  bool s;
  shader->compile(s);
  tex = new Texture2D("blackboard.png");
  model = new Model(36, cubeVertices, shader, tex);
  model->init();

  cubes.clear();
  movement.clear();

  cout << "Left click to spawn a cube\n";
  cout << "Right click to attract cubes\n";
}
void RasterizingScene::update(float delta) {
  for (int i = 0; i < cubes.size(); ++i) {
    cubes[i] = translate(movement[i] * delta) * cubes[i];
  }
  if (window->isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
    float rx = (float)rand() / RAND_MAX, ry = (float)rand() / RAND_MAX, rz = (float)rand() / RAND_MAX;
    mat4 rot = rotate(rx, vec3(1, 0, 0)) * rotate(rx, vec3(0, 1, 0)) * rotate(rx, vec3(0, 0, 1));
    movement.push_back(camera->direction() + vec3(rx, ry, rz));
    cubes.push_back(translate(camera->direction() * 3.0f) * camera->transform() * rot);
  }
  if (window->isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
    for (int i = 0; i < cubes.size(); ++i) {
      vec3 v = vec3(cubes[i][3]) - camera->position() - camera->direction() * 3.0f;
      movement[i] -= v / 100.0f;
    }
  }
}
void RasterizingScene::render(Camera& camera) {
  for (int i = 0; i < cubes.size(); ++i) {
    model->transform(cubes[i]);
    model->render(camera);
  }
}
void RasterizingScene::free() {
  model->free();
}