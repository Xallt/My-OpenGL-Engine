#include "../include/raytracing/rt_shader.h"

RaytracingShader::RaytracingShader(): 
  sphereCount(0), dirLightCount(0), pointLightCount(0) {}

void RaytracingShader::addSphere(rt::Sphere s) {
  string name = "Spheres[" + to_string(sphereCount) + "]";
  setUniform(name + ".position", s.position);
  setUniform(name + ".radius", s.radius);
  setUniform(name + ".shininess", s.shininess);
  setUniform(name + ".color", s.color);
  setUniform("SphereCount", ++sphereCount);
}
void RaytracingShader::addDirectionalLight(rt::DirectionalLight l) {
  string name = "DirLights[" + to_string(dirLightCount) + "]";
  setUniform(name + ".direction", l.direction);
  setUniform(name + ".color", l.color);
  setUniform(name + ".intensity", l.intensity);
  setUniform("DirLightCount", ++dirLightCount);
}
void RaytracingShader::addPointLight(rt::PointLight l) {
  string name = "PointLights[" + to_string(pointLightCount) + "]";
  setUniform(name + ".position", l.position);
  setUniform(name + ".color", l.color);
  setUniform(name + ".intensity", l.intensity);
  setUniform("PointLightCount", ++pointLightCount);
}
void RaytracingShader::use() {
  ShaderProgram::use();
  sphereCount = dirLightCount = pointLightCount = 0;
}