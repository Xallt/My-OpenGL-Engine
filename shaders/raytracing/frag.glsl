#version 330 core

in vec3 RayDirection;
out vec4 color;
struct Camera {
    vec3 direction, up, right;
    vec3 position;
};
uniform Camera camera;

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
uniform int SphereCount;
uniform Sphere Spheres[20];

uniform int DirLightCount;
uniform DirectionalLight DirLights[20];

uniform int PointLightCount;
uniform PointLight PointLights[20];

struct RayHit {
  vec3 position;
  vec3 normal;
  int type, index;
};

vec3 sphereComp(vec3 ro, vec3 rd, Sphere s) {
  return vec3(
    dot(rd, rd),
    2 * dot(ro - s.position, rd),
    dot(ro - s.position, ro - s.position) - s.radius * s.radius
  );
}

#define INF 100000
RayHit castRay(vec3 ro, vec3 rd) {
  RayHit hit;
  hit.normal = vec3(0);
  hit.position = ro + rd * INF;
  hit.type = -1;
  if (rd.y * ro.y <= 0) {
    hit.type = 2;
    hit.position = ro - ro.y / rd.y * rd;
    hit.normal = vec3(0, 1, 0);
  }
  for (int i = 0; i < SphereCount; ++i) {
    Sphere s = Spheres[i];
    vec3 comp = sphereComp(ro, rd, s);
    float d = comp.y * comp.y - 4 * comp.x * comp.z;
    if (!(d >= 0 && comp.y < 0 && comp.z > 0)) {
      continue;
    }
    float k = (-comp.y - sqrt(d)) / 2 * comp.x;
    if (k < distance(ro, hit.position)) {
      hit.position = ro + k * rd;
      hit.type = 1;
      hit.normal = normalize(hit.position - s.position);
      hit.index = i;
    }
  }
  return hit;
}

vec3 shadeRay(vec3 ro, vec3 rd) {
  RayHit hit = castRay(ro, rd);
  if (hit.type == 2) {
    float size = 3;
    bool x = mod(hit.position.x / size, 2) > 1, z = mod(hit.position.z / size, 2) > 1;
    float k = float(x ^^ z);
    return vec3(k * .6 + .4);
  }
  if (hit.type == 1) {
    return Spheres[hit.index].color;
  }
  return vec3(0);
}

void main()
{
  color = vec4(shadeRay(camera.position, RayDirection), 1);
}