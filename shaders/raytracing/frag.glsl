#version 330 core

in vec2 Position;
out vec4 glColor;
struct Camera {
    vec3 direction, up, right;
    vec3 position;
};
uniform Camera camera;

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
uniform Plane plane;

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
RayHit emptyHit() {
  RayHit hit;
  hit.type = -1;
  return hit;
}

#define INF 100000
#define EPS 0.00001
#define BACKGROUND vec3(0.02734375, 0.1015625, 0.1484375)

// Returns 3 quadratic equation components for checking ray&sphere intersection
vec3 sphereComp(vec3 ro, vec3 rd, Sphere s) {
  return vec3(
    dot(rd, rd),
    2 * dot(ro - s.position, rd),
    dot(ro - s.position, ro - s.position) - s.radius * s.radius
  );
}

// Returns result of Ray Tracing in a struct
RayHit castRay(vec3 ro, vec3 rd) {
  RayHit hit = emptyHit();
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
    if (hit.type == -1 || k < distance(ro, hit.position)) {
      hit.position = ro + k * rd;
      hit.type = 1;
      hit.normal = normalize(hit.position - s.position);
      hit.index = i;
    }
  }
  return hit;
}

// Ambient light at a given ray hit
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
// Diffuse light at a given ray hit
vec3 diffuseAt(vec3 ro, vec3 rd, RayHit hit) {
  vec3 k = vec3(0);
  for (int i = 0; i < DirLightCount; ++i) {
    DirectionalLight l = DirLights[i];
    if (castRay(hit.position + EPS * hit.normal, normalize(-l.direction)).type == -1) {
      k += l.color * max(dot(hit.normal, normalize(-l.direction)), 0) * l.intensity;
    }
  }
  for (int i = 0; i < PointLightCount; ++i) {
    PointLight l = PointLights[i];
    if (castRay(hit.position + EPS * hit.normal, normalize(l.position - hit.position)).type == -1) {
      k += l.color * max(dot(hit.normal, normalize(l.position - hit.position)), 0) * l.intensity;
    }
  }
  return k;
}
// Specular light at a given ray hit
vec3 specularAt(vec3 ro, vec3 rd, RayHit hit, float shininess) {
  vec3 k = vec3(0);
  for (int i = 0; i < DirLightCount; ++i) {
    DirectionalLight l = DirLights[i];
    if (castRay(hit.position + EPS * hit.normal + EPS * hit.normal, normalize(-l.direction)).type == -1) {
      k += l.color * pow(max(dot(reflect(rd, hit.normal), normalize(-l.direction)), 0), shininess) * l.intensity;
    }
  }
  for (int i = 0; i < PointLightCount; ++i) {
    PointLight l = PointLights[i];
    if (castRay(hit.position + EPS * hit.normal, normalize(l.position - hit.position)).type == -1) {
      k += l.color * pow(max(dot(reflect(rd, hit.normal), normalize(l.position - hit.position)), 0), shininess) * l.intensity;
    }
  }
  return k;
}

// Struct representing the color of the material and color reflected from the scene
struct ColorRes {
  vec3 color, spec;
};
ColorRes colorRes(vec3 color, vec3 spec) {
  ColorRes cr;
  cr.color = color;
  cr.spec = spec;
  return cr;
}

// Coloring functions for every type of object (and the background)
ColorRes colorSphere(vec3 ro, vec3 rd, RayHit hit) {
  Sphere s = Spheres[hit.index];
  vec3 n = hit.normal, p = hit.position;
  vec3 k = vec3(0), spec = vec3(0);
  
  // Ambient
  k += ambientAt(ro, rd, hit) * s.mat.ambient;

  // Diffuse
  k += diffuseAt(ro, rd, hit) * s.mat.diffuse;

  // Specular
  spec += specularAt(ro, rd, hit, s.mat.shininess) * s.mat.specular;
  
  return colorRes(k * s.color, spec);
}
ColorRes colorPlane(vec3 ro, vec3 rd, RayHit hit) {
  float size = 3;
  bool x = mod(hit.position.x / size, 2) > 1, z = mod(hit.position.z / size, 2) > 1;
  float k = float(x ^^ z);
  vec3 col = vec3(k * .6 + .4);
  return colorRes(
    (diffuseAt(ro, rd, hit) * plane.mat.diffuse + ambientAt(ro, rd, hit) * plane.mat.ambient) * col, 
    specularAt(ro, rd, hit, plane.mat.shininess) * plane.mat.specular
  );
}
ColorRes colorBackground(vec3 ro, vec3 rd) {
  vec3 light = vec3(0);
  for (int i = 0; i < DirLightCount; ++i) {
    DirectionalLight l = DirLights[i];
    light += pow(max(dot(rd, normalize(-l.direction)), 0), 80) * l.intensity * l.color;
  }
  return colorRes(BACKGROUND, light);
}

// Color correction (if needed)
vec3 shadeRes(vec3 ro, vec3 rd, vec3 color, RayHit hit) {
  return color;
}

#define TRACE_DEPTH 5
vec3 colorRay(vec3 ro, vec3 rd) {
  rd = normalize(rd);
  vec3 r = vec3(1);
  vec3 res = vec3(0);
  int st = 0;
  RayHit first = castRay(ro, rd);
  for (int i = 0; i < TRACE_DEPTH; ++i) {
    st = i;
    RayHit hit = castRay(ro, rd);
    ColorRes col;
    float refl;
    if (hit.type == 2) { 
      col = colorPlane(ro, rd, hit);
      refl = plane.mat.reflection;
    }
    if (hit.type == 1) {
      col = colorSphere(ro, rd, hit);
      refl = Spheres[hit.index].mat.reflection;
    }
    if (hit.type == -1) {
      col = colorBackground(ro, rd);
      refl = 0;
    }
    if (i == TRACE_DEPTH - 1) {
      refl = 0;
    }
    res += r * (shadeRes(ro, rd, col.color, hit) * (1 - refl) + col.spec);
    if (refl == 0) {
      break;
    }
    r *= refl;

    ro = hit.position;
    rd = reflect(rd, hit.normal);
  }
  if (first.type == -1) {
    return res;
  }
  else {
    return shadeRes(ro, rd, res, first);
  }
}

vec3 uvToRay(vec2 uv) {
  return camera.direction + uv.x * camera.right + uv.y * camera.up;
}

void main()
{
  vec2 off = vec2(1, -1) * 0.001;
  // The simplest atialiasing out there
  glColor = vec4(
    (colorRay(camera.position, uvToRay(Position + off.xx)) + 
     colorRay(camera.position, uvToRay(Position + off.xy)) + 
     colorRay(camera.position, uvToRay(Position + off.yx)) + 
     colorRay(camera.position, uvToRay(Position + off.yy))) / 4, 
  1);
}