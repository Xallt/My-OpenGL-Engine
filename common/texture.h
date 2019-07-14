#ifndef COMMON_TEXTURE
#define COMMON_TEXTURE

#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <iostream>

class Texture2D {
private:
  int width, height;
  GLuint texture;
public:
  Texture2D(const char* textureFile);

  GLuint id();

  void bind();
  void unbind();
};

#endif