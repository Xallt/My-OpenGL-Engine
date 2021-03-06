#include "rt_scene.h"

void RaytracingScene::init() {
  float vertices[] = {
    -1.0, -1.0,
    1.0, -1.0,
    1.0, 1.0,
    -1.0, -1.0,
    -1.0, 1.0,
    1.0, 1.0,
  };

  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (GLvoid*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);

  shader = new RaytracingShader();
  shader->vertexFromFile("shaders/raytracing/vert.glsl");
  shader->fragmentFromFile("shaders/raytracing/frag.glsl");
  bool s;
  shader->compile(s);
}
void RaytracingScene::render(Camera& camera) {
  shader->use();
  shader->setUniform("camera.direction", camera.direction());
  shader->setUniform("camera.up", camera.up() * camera.cameraSize().y);
  shader->setUniform("camera.right", camera.right() * camera.cameraSize().x);
  shader->setUniform("camera.position", camera.position());

  rt::Plane plane = {
    vec3(0, 1, 0), vec3(0, -2, 0),
    {0.2f, 0.7f, 0.4f, 50.0f, 0}
  };

  shader->setUniformStruct("plane", plane);

  shader->addSphere({
    vec3(0, 1, -2),
    1,
    vec3(0, 1, 0),
    {0.2f, 0.7f, 0.4f, 50.0f, 0.2}
  });

  shader->addSphere({
    vec3(5, 4, 0),
    3,
    vec3(1, 0, 0),
    {0.2f, 0.7f, 0.4f, 50.0f, 0.3}
  });
  shader->addSphere({
    vec3(5, 3, -14),
    11,
    vec3(0, 0, 1),
    {0.2f, 0.7f, 0.4f, 50.0f, 0.5f}
  });
  
  shader->addDirectionalLight({
    vec3(-1, -1, -1),
    vec3(1, 1, 1),
    0.8
  });
  shader->addDirectionalLight({
    vec3(1, -1, 1),
    vec3(1, 1, 1),
    0.8
  });

  glBindVertexArray(VAO);

  glDrawArrays(GL_TRIANGLES, 0, 6);

  glBindVertexArray(0);
}
void RaytracingScene::update(float delta) {}
void RaytracingScene::free() {
  glDeleteBuffers(1, &VBO);
  glDeleteVertexArrays(1, &VAO);
}