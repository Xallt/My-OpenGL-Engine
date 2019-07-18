#include "../include/raymarching/rm_scene.h"

RaymarchingScene::RaymarchingScene() {}

void RaymarchingScene::init() {
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

  shader = new RaymarchingShader();
  shader->vertexFromFile("shaders/raymarching/vert.glsl");
  shader->fragmentFromFile("shaders/raymarching/frag.glsl");
  bool s;
  shader->compile(s);
}
void RaymarchingScene::render(Camera &camera) {
  shader->use();
  shader->setUniform("camera.direction", camera.direction());
  shader->setUniform("camera.up", camera.up() * camera.cameraSize().y);
  shader->setUniform("camera.right", camera.right() * camera.cameraSize().x);
  shader->setUniform("camera.position", camera.position());
  shader->setUniform("Time", glfwGetTime());

  rm::DirectionalLight l = {
    vec3(-2),
    vec3(1),
    1.0
  };

  shader->setUniformStruct("light", l);
  shader->setUniformStruct("mat", {
    vec3(0, 0.3, 1),
    0.8, 0.3, 0.5,
    100
  });

  glBindVertexArray(VAO);

  glDrawArrays(GL_TRIANGLES, 0, 6);

  glBindVertexArray(0);
}
void RaymarchingScene::update(float delta) {}
void RaymarchingScene::free() {
  glDeleteBuffers(1, &VBO);
  glDeleteVertexArrays(1, &VAO);
}