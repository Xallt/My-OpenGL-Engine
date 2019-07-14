#include "../common/texture.h"

Texture2D::Texture2D(const char* textureFile) {
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  unsigned char* image = SOIL_load_image(textureFile, &width, &height, 0, SOIL_LOAD_RGB);
  std::cout << SOIL_last_result() << '\n';

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  glGenerateMipmap(GL_TEXTURE_2D);

  SOIL_free_image_data(image);
  glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Texture2D::id() {
  return texture;
}

void Texture2D::bind() {
  glBindTexture(GL_TEXTURE_2D, texture);
}
void Texture2D::unbind() {
  glBindTexture(GL_TEXTURE_2D, 0);
}