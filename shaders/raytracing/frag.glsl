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
  float shininess;
};

#define SPHERES 1
Sphere getSphere(int index) {
  Sphere s;
  s.position = vec3(0, 2, 0);
  s.radius = 1;
  s.shininess = 20;
  return s;
};

struct Ray {
  vec3 origin;
  vec3 direction;
};
struct RayHit {
  vec3 position;
  vec3 normal;
  int type;
};

vec3 sphereComp(Ray ray, Sphere s) {
  return vec3(
    dot(ray.direction, ray.direction),
    2 * dot(ray.origin - s.position, ray.direction),
    dot(ray.origin - s.position, ray.origin - s.position) - s.radius * s.radius
  );
}

#define INF 100000
RayHit castRay(Ray ray) {
  RayHit hit;
  hit.normal = vec3(0);
  hit.position = ray.origin + ray.direction * INF;
  hit.type = -1;
  if (ray.direction.y * ray.origin.y <= 0) {
    hit.type = 2;
    hit.position = ray.origin - ray.origin.y / ray.direction.y * ray.direction;
    hit.normal = vec3(0, 1, 0);
  }
  for (int i = 0; i < SPHERES; ++i) {
    Sphere s = getSphere(i);
    vec3 comp = sphereComp(ray, s);
    float d = comp.y * comp.y - 4 * comp.x * comp.z;
    if (!(d >= 0 && comp.y < 0 && comp.z > 0)) {
      continue;
    }
    float k = (-comp.y - sqrt(d)) / 2 * comp.x;
    if (k < distance(ray.origin, hit.position)) {
      hit.position = ray.origin + k * ray.direction;
      hit.type = 1;
      hit.normal = normalize(hit.position - s.position);
    }
  }
  return hit;
}

void main()
{
  Ray ray;
  ray.origin = camera.position;
  ray.direction = normalize(RayDirection);
  RayHit hit = castRay(ray);
  color = vec4(hit.normal, 1);
}