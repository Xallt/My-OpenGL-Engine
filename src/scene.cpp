#include "../common/raymarching/scene.h"

RaymarchingScene::RaymarchingScene() {
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

  shader = new ShaderProgram();
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
  
  glBindVertexArray(VAO);

  glDrawArrays(GL_TRIANGLES, 0, 6);

  glBindVertexArray(0);
}