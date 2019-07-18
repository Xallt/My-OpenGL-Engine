#ifndef COMMON_RAYTRACING_SHADER
#define COMMON_RAYTRACING_SHADER

#include "../shader.h"
#include <string>

namespace rt {
  struct Sphere {
    vec3 position;
    float radius;
    vec3 color;
    float ambient, diffuse, specular, shininess;
  };
  struct DirectionalLight {
    vec3 direction, color;
    float intensity;
  };
  struct PointLight {
    vec3 position, color;
    float intensity;
  };
}
class RaytracingShader: public ShaderProgram {
private:
  int sphereCount, dirLightCount, pointLightCount;
public:
  RaytracingShader();
  void use();
  void addSphere(rt::Sphere s);
  void addDirectionalLight(rt::DirectionalLight l);
  void addPointLight(rt::PointLight l);
};

#endif