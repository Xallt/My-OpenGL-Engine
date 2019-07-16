#include "../include/rasterizing/rst_model.h"

SingleModel::SingleModel(int vertexCount, int verticesSize, float* vertices, int indicesSize, int* indices, ShaderProgram &shader, Texture2D tex):
  vertexCount(vertexCount), verticesSize(verticesSize), vertices(vertices), indicesSize(indicesSize), indices(indices), shader(shader), tex(tex) {}
SingleModel::SingleModel(int vertexCount, int verticesSize, float* vertices, ShaderProgram &shader, Texture2D tex):
  vertexCount(vertexCount), verticesSize(verticesSize), vertices(vertices), shader(shader), tex(tex) {
  indicesSize = vertexCount;
  indices = new int[indicesSize];
  for (int i = 0; i < indicesSize; ++i) {
    indices[i] = i;
  }
}
void SingleModel::init() {
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glGenVertexArrays(1, &VAO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, verticesSize * sizeof(float), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize * sizeof(int), indices, GL_STATIC_DRAW);

  glBindVertexArray(0);
}
void SingleModel::enableAttribute(int location, int size, GLenum type, int offset) {
  glBindVertexArray(VAO);

  glVertexAttribPointer(location, size, type, GL_FALSE, verticesSize / vertexCount * sizeof(float), (GLvoid*)(offset * sizeof(float)));
  glEnableVertexAttribArray(location);

  glBindVertexArray(0);
}
void SingleModel::render(Camera* camera) {
  tex.bind();
  shader.use();
  shader.setUniform("projection", camera->projection());
  shader.setUniform("view", camera->view());
  shader.setUniform("model", _transform);

  glBindVertexArray(VAO);

  glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);
}
void SingleModel::free() {
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteVertexArrays(1, &VAO);
}
mat4 SingleModel::transform() {
  return _transform;
}
mat4 SingleModel::transform(mat4 t) {
  _transform = t;
}