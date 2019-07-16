#version 330 core

out vec4 color;
in vec3 RayDirection;

struct DirectionalLight {
  vec3 direction, color;
  float intensity;
};

struct PointLight {
  vec3 position, color;
  float intensity;
};

vec3 lightDirectionAt(DirectionalLight l, vec3 p) {
	return normalize(l.direction);
}
vec3 lightDirectionAt(PointLight l, vec3 p) {
	return normalize(p - l.position);
}

struct Material {
	vec3 color;
	float diffuse, ambient, specular;
	float shininess;
};

struct Camera {
    vec3 direction, up, right;
    vec3 position;
};

uniform Camera camera;
uniform DirectionalLight light;
uniform Material mat;

// ==============
// Utils
float min3(vec3 p) {
	return min(p.x, min(p.y, p.z));
}
// vec3 mod(vec3 p, float m) {
// 	return (p % m + m) % m;
// }

// ==============
// Distance functions
struct DE {
	float dist;
	float ambient;
};
DE to_DE(float dist) {
	DE de;
	de.dist = dist;
	de.ambient = 0;
	return de;
}

DE deSphere(vec3 p, float r) {
	return to_DE(length(p) - r);
}
DE deTetrahedron(vec3 p) {
	return to_DE(max(
		max(- p.x - p.y - p.z, p.x + p.y - p.z),
		max(- p.x + p.y + p.z, p.x - p.y + p.z)
	) / sqrt(3) - sqrt(3) / 3);
}
DE deBox(vec3 p, vec3 size) {
	p = abs(p);
	return to_DE(distance(p, min(p, size)) - max(0, min3(size - p)));
}
DE dePlane(vec3 p, vec3 n, vec3 s) {
	return to_DE(dot(n, p - s) / length(n));
}
DE diSierpinski(vec3 p) {
	vec3 a1 = vec3(1, 1, 1),
			 a2 = vec3(-1, -1, 1),
			 a3 = vec3(-1, 1, -1),
			 a4 = vec3(1, -1, -1);
	const int N = 6;
	vec3 a, pa = vec3(0, 0, 0);
	float d, dd;
	int changes = 0;
	for (int i = 0; i < N; ++i) {
		d = distance(p, a1);
		a = a1;
		dd = distance(p, a2);
		if (dd < d) {
			d = dd;
			a = a2;
		}
		dd = distance(p, a3);
		if (dd < d) {
			d = dd;
			a = a3;
		}
		dd = distance(p, a4);
		if (dd < d) {
			d = dd;
			a = a4;
		}
		if (a != pa && i >= N / 2.) {
			changes += 1;
		}
		p = p * 2 - a;
		pa = a;
	}
	DE de = to_DE(deTetrahedron(p).dist / pow(2, N));
	// de.ambient = pow(float(changes) / (N - 1), 1) * .4;
	return de;
}
DE deMenger( vec3 p )
{
	float d = deBox(p, vec3(1, 1, 1)).dist;
	float res = d;
	int step = 0;
	const int N = 5;
	for (int m = 0; m < N; m++) {
		float s = pow(3., m + 1);
		vec3 r = abs(mod( p * s + 3, 6 ) - 3);
		float dx = max(r.y, r.z);
		float dy = max(r.z, r.x);
		float dz = max(r.x, r.y);

		float c = (1 - min3(vec3(dx, dy, dz))) / s;
		if (c > res) {
			step = m + 1;
			res = c;
		}
	}
	DE de = to_DE(res);
	de.ambient = float(step) / (N + 1);
	return de;
}
DE opUnion(DE a, DE b) {
	a.dist = min(a.dist, b.dist);
	return a;
}

// ==========================
// Definition macros
#define MAX_STEPS 300
#define MIN_DE 0.0001
#define MAX_DISTANCE 100
#define BACKGROUND_COLOR vec3(0.8, 0.8, 1.);

struct RayHit {
	float dist, mind, ambient;
	int steps;
	DE de;
};

DE DistanceField(vec3 p) {
	DE obj = deMenger(p);
	return opUnion(obj, to_DE(p.y + 2));
}

vec3 getNormal(vec3 p) {
	vec2 ff = vec2(0.000001, 0);
	return normalize(vec3(
		DistanceField(p + ff.xyy).dist - DistanceField(p - ff.xyy).dist,
		DistanceField(p + ff.yxy).dist - DistanceField(p - ff.yxy).dist,
		DistanceField(p + ff.yyx).dist - DistanceField(p - ff.yyx).dist
	));
}

RayHit castRay(vec3 ro, vec3 rd) {
	float t = 0, mind = DistanceField(ro).dist, d, amb = 0;
	int steps = 0;
	for (int i = 0; i < MAX_STEPS; ++i) {
		steps = i + 1;
		if (t > MAX_DISTANCE) {
			break;
		}
		d = DistanceField(ro).dist;
		amb += 1 / d;
		mind = min(d, mind);
		if (d < MIN_DE) {
			break;
		}
		ro += d * rd;
		t += d;
	}
	RayHit hit;
	hit.dist = t;
	hit.mind = mind;
	hit.steps = steps;
	hit.de = DistanceField(ro);
	hit.ambient = max(amb / 20, 1);
	return hit;
}

vec3 shadeObject(vec3 ro, vec3 rd, RayHit hit, DE de) {
	vec3 p = ro + rd * hit.dist;
	vec3 n = getNormal(p);
	vec3 res = vec3(0);
	
	// Shadow
	float shadowK = step(MIN_DE, castRay(p + n * MIN_DE * 2, -lightDirectionAt(light, p)).de.dist);

	// Diffuse
	res += max(dot(n, -lightDirectionAt(light, p)), 0) * mat.diffuse * mat.color * light.intensity * light.color * shadowK;

	// Ambient
	res += mat.ambient * light.intensity * mat.color * light.color;

	// Specular
	res += mat.specular * light.intensity * light.color * pow(max(dot(reflect(rd, n), -lightDirectionAt(light, p)), 0), mat.shininess) * shadowK;

	// Depth
	res *= mix(res, res * 0.5, smoothstep(0, 20, distance(p, ro)));

	res *= 1 - de.ambient;

	return res;
}

vec3 shadeBackground(vec3 ro, vec3 rd, vec3 p) {
	return vec3(pow(max(dot(rd, -lightDirectionAt(light, ro)), 0), 80)) + BACKGROUND_COLOR;
}

vec3 getRayColor(vec3 ro, vec3 rd) {
	RayHit hit = castRay(ro, rd);
	if (hit.mind < MIN_DE) {
		return shadeObject(ro, rd, hit, hit.de);
	}
	else {
		return shadeBackground(ro, rd, ro + rd * hit.dist);
	}
}

void main()
{
	color = vec4(getRayColor(camera.position, normalize(RayDirection)), 1);
}