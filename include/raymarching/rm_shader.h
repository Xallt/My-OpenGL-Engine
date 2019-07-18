#ifndef COMMON_RAYMARCHING_SHADER
#define COMMON_RAYMARCHING_SHADER

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "../shader.h"

using namespace std;

namespace rm {
  struct DirectionalLight {
    vec3 direction, color;
    float intensity;
  };

  struct PointLight {
    vec3 position, color;
    float intensity;
  };

  struct Material {
    vec3 color;
    float diffuse, ambient, specular;
    float shininess;
  };
}

class RaymarchingShader: public ShaderProgram {
public:
  RaymarchingShader();
  void setUniformStruct(string name, rm::DirectionalLight l);
  void setUniformStruct(string name, rm::PointLight l);
  void setUniformStruct(string name, rm::Material l);
};

#endif