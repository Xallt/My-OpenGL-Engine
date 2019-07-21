#ifndef COMMON_RAYTRACING_SHADER
#define COMMON_RAYTRACING_SHADER

#include "../shader.h"
#include <string>

namespace rt {
  struct Camera {
    vec3 direction, up, right;
    vec3 position;
  };

  struct Material {
    float ambient, diffuse, specular, shininess, reflection;
  };

  struct Plane {
    vec3 normal, point;
    Material mat;
  };
  struct Sphere {
    vec3 position;
    float radius;
    vec3 color;
    Material mat;
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
public:
  int sphereCount, dirLightCount, pointLightCount;
  RaytracingShader();
  void use();
  void setUniformStruct(string name, rt::Material mat);
  void setUniformStruct(string name, rt::Plane pl);
  void setUniformStruct(string name, rt::Sphere s);
  void setUniformStruct(string name, rt::DirectionalLight l);
  void setUniformStruct(string name, rt::PointLight l);

  void addSphere(rt::Sphere s);
  void addDirectionalLight(rt::DirectionalLight l);
  void addPointLight(rt::PointLight l);
};

#endif