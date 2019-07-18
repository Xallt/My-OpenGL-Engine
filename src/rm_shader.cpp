#include "../include/raymarching/rm_shader.h"

RaymarchingShader::RaymarchingShader() {}

void RaymarchingShader::setUniformStruct(string name, DirectionalLight l) {
  setUniform(name + ".direction", l.direction);
  setUniform(name + ".color", l.color);
  setUniform(name + ".intensity", l.intensity);
}

void RaymarchingShader::setUniformStruct(string name, PointLight l) {
  setUniform(name + ".position", l.position);
  setUniform(name + ".color", l.color);
  setUniform(name + ".intensity", l.intensity);
}

void RaymarchingShader::setUniformStruct(string name, Material m) {
  setUniform(name + ".color", m.color);
  setUniform(name + ".diffuse", m.diffuse);
  setUniform(name + ".specular", m.specular);
  setUniform(name + ".ambient", m.ambient);
  setUniform(name + ".shininess", m.shininess);
}