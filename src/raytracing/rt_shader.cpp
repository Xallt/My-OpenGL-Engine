#include "rt_shader.h"

RaytracingShader::RaytracingShader(): 
  sphereCount(0), dirLightCount(0), pointLightCount(0) {}

void RaytracingShader::setUniformStruct(string name, rt::Sphere s) {
  setUniform(name + ".position", s.position);
  setUniform(name + ".radius", s.radius);
  setUniform(name + ".color", s.color);
  setUniformStruct(name + ".mat", s.mat);
}
void RaytracingShader::setUniformStruct(string name, rt::Material mat) {
  setUniform(name + ".shininess", mat.shininess);
  setUniform(name + ".diffuse", mat.diffuse);
  setUniform(name + ".ambient", mat.ambient);
  setUniform(name + ".specular", mat.specular);
  setUniform(name + ".reflection", mat.reflection);
}
void RaytracingShader::setUniformStruct(string name, rt::DirectionalLight l) {
  setUniform(name + ".direction", l.direction);
  setUniform(name + ".color", l.color);
  setUniform(name + ".intensity", l.intensity);
}
void RaytracingShader::setUniformStruct(string name, rt::PointLight l) {
  setUniform(name + ".position", l.position);
  setUniform(name + ".color", l.color);
  setUniform(name + ".intensity", l.intensity);
}
void RaytracingShader::setUniformStruct(string name, rt::Plane pl) {
  setUniform(name + ".normal", pl.normal);
  setUniform(name + ".point", pl.point);
  setUniformStruct(name + ".mat", pl.mat);
}
void RaytracingShader::addSphere(rt::Sphere s) {
  string name = "Spheres[" + to_string(sphereCount) + "]";
  setUniformStruct(name, s);
  setUniform("SphereCount", ++sphereCount);
}
void RaytracingShader::addDirectionalLight(rt::DirectionalLight l) {
  string name = "DirLights[" + to_string(dirLightCount) + "]";
  setUniformStruct(name, l);
  setUniform("DirLightCount", ++dirLightCount);
}
void RaytracingShader::addPointLight(rt::PointLight l) {
  string name = "PointLights[" + to_string(pointLightCount) + "]";
  setUniformStruct(name, l);
  setUniform("PointLightCount", ++pointLightCount);
}
void RaytracingShader::use() {
  ShaderProgram::use();
  sphereCount = dirLightCount = pointLightCount = 0;
}