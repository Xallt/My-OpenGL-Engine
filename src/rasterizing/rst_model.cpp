#include "rst_model.h"

Model::Model(int vertexCount, float* vertices, ShaderProgram* shader, Texture2D* tex):
  shader(shader), tex(tex) {
  position.clear();
  texCoord.clear();
  for (int i = 0; i < vertexCount; ++i) {
    position.push_back(vec3(vertices[i * 5], vertices[i * 5 + 1], vertices[i * 5 + 2]));
    texCoord.push_back(vec2(vertices[i * 5 + 3], vertices[i * 5 + 4]));
  }
  computeData();
}
void Model::computeData() {
  verticesData = new float[vertexCount() * 5];
  for (int i = 0; i < vertexCount(); ++i) {
    verticesData[i * 5] = position[i].x, verticesData[i * 5 + 1] = position[i].y, verticesData[i * 5 + 2] = position[i].z;
    verticesData[i * 5 + 3] = texCoord[i].x, verticesData[i * 5 + 4] = texCoord[i].y;
  }
}

void Model::update(float delta) {}
void Model::init() {
  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertexCount() * 5 * sizeof(float), verticesData, GL_STATIC_DRAW);
  glBindVertexArray(0);

  enableAttribute(0, 3, GL_FLOAT, 0);
  enableAttribute(1, 2, GL_FLOAT, 3);
}
void Model::render(Camera& camera) {
  tex->bind();
  shader->use();
  shader->setUniform("projection", camera.projection());
  shader->setUniform("view", camera.view());
  shader->setUniform("model", _transform);
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, vertexCount() * 5);
  glBindVertexArray(0);
}
void Model::free() {
  glDeleteBuffers(1, &VBO);
  glDeleteVertexArrays(1, &VAO);
}

void Model::enableAttribute(int location, int size, GLenum type, int offset) {
  glBindVertexArray(VAO);

  glVertexAttribPointer(location, size, type, GL_FALSE, 5 * sizeof(float), (GLvoid*)(offset * sizeof(float)));
  glEnableVertexAttribArray(location);

  glBindVertexArray(0);
}

int Model::vertexCount() {
  return position.size();
}

mat4 Model::transform() {
  return _transform;
}
mat4 Model::transform(mat4 t) {
  _transform = t;
}