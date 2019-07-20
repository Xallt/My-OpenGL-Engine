#version 330 core

in vec2 Position;
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
  float ambient, diffuse, specular, shininess, reflection;
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

vec3 lDirAt(DirectionalLight l, vec3 p) {
  return normalize(l.direction);
}
vec3 lDirAt(PointLight l, vec3 p) {
  return normalize(p - l.position);
}
struct RayHit {
  vec3 position;
  vec3 normal;
  int type, index;
};
vec3 ambientAt(vec3 ro, vec3 rd, RayHit hit) {
  vec3 k = vec3(0);
  for (int i = 0; i < DirLightCount; ++i) {
    DirectionalLight l = DirLights[i];
    k += l.color * l.intensity;
  }
  for (int i = 0; i < PointLightCount; ++i) {
    PointLight l = PointLights[i];
    k += l.color * l.intensity;
  }
  return k;
}
vec3 diffuseAt(vec3 ro, vec3 rd, RayHit hit) {
  vec3 k = vec3(0);
  for (int i = 0; i < DirLightCount; ++i) {
    DirectionalLight l = DirLights[i];
    k += l.color * max(dot(hit.normal, -lDirAt(l, hit.position)), 0) * l.intensity;
  }
  for (int i = 0; i < PointLightCount; ++i) {
    PointLight l = PointLights[i];
    k += l.color * max(dot(hit.normal, -lDirAt(l, hit.position)), 0) * l.intensity;
  }
  return k;
}
vec3 specularAt(vec3 ro, vec3 rd, RayHit hit, float shininess) {
  vec3 k = vec3(0);
  for (int i = 0; i < DirLightCount; ++i) {
    DirectionalLight l = DirLights[i];
    k += l.color * pow(max(dot(reflect(rd, hit.normal), -lDirAt(l, hit.position)), 0), shininess) * l.intensity;
  }
  for (int i = 0; i < PointLightCount; ++i) {
    PointLight l = PointLights[i];
    k += l.color * pow(max(dot(reflect(rd, hit.normal), -lDirAt(l, hit.position)), 0), shininess) * l.intensity;
  }
  return k;
}

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

struct ColorRes {
  vec3 color, spec;
};
ColorRes colorRes(vec3 color, vec3 spec) {
  ColorRes cr;
  cr.color = color;
  cr.spec = spec;
  return cr;
}

ColorRes colorSphere(vec3 ro, vec3 rd, RayHit hit) {
  Sphere s = Spheres[hit.index];
  vec3 n = hit.normal, p = hit.position;
  vec3 k = vec3(0), spec = vec3(0);
  
  // Ambient
  k += ambientAt(ro, rd, hit) * s.ambient;

  // Diffuse
  k += diffuseAt(ro, rd, hit) * s.diffuse;

  // Specular
  spec += specularAt(ro, rd, hit, s.shininess) * s.specular;
  
  return colorRes(k * s.color, spec);
}
ColorRes colorPlane(vec3 ro, vec3 rd, RayHit hit) {
  float size = 3;
  bool x = mod(hit.position.x / size, 2) > 1, z = mod(hit.position.z / size, 2) > 1;
  float k = float(x ^^ z);
  return colorRes(vec3(k * .6 + .4), vec3(0));
}

ColorRes colorBackground(vec3 ro, vec3 rd) {
  return colorRes(vec3(0), vec3(0));
}

#define TRACE_DEPTH 5
vec3 shadeRay(vec3 ro, vec3 rd) {
  rd = normalize(rd);
  vec3 r = vec3(1);
  vec3 res = vec3(0);
  int st = 0;
  for (int i = 0; i < TRACE_DEPTH; ++i) {
    st = i;
    RayHit hit = castRay(ro, rd);
    ColorRes col;
    float refl;
    if (hit.type == 2) { 
      col = colorPlane(ro, rd, hit);
      refl = 0;
    }
    if (hit.type == 1) {
      col = colorSphere(ro, rd, hit);
      refl = Spheres[hit.index].reflection;
    }
    if (hit.type == -1) {
      col = colorBackground(ro, rd);
      refl = 0;
    }
    if (i == TRACE_DEPTH - 1) {
      refl = 0;
    }
    res += r * (col.color * (1 - refl) + col.spec);
    if (refl == 0) {
      break;
    }
    r *= refl;

    ro = hit.position;
    rd = reflect(rd, hit.normal);
  }
  return res;
}

vec3 uvToRay(vec2 uv) {
  return camera.direction + uv.x * camera.right + uv.y * camera.up;
}

void main()
{
  vec2 off = vec2(1, -1) * 0.002;
  color = vec4(
    (shadeRay(camera.position, uvToRay(Position + off.xx)) + 
     shadeRay(camera.position, uvToRay(Position + off.xy)) + 
     shadeRay(camera.position, uvToRay(Position + off.yx)) + 
     shadeRay(camera.position, uvToRay(Position + off.yy))) / 4, 
  1);
}