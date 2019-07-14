#version 330 core

struct Camera {
    vec3 direction, up, right;
    vec3 position;
};
uniform Camera camera;
out vec4 color;
in vec3 RayDirection;

// ==========================
// Definition macros
#define MAX_STEPS 100
#define MIN_DE 0.001
#define MAX_DISTANCE 100

const vec3 lightDirection = normalize(vec3(-1.0));

struct RayHit {
	float dist, mind;
	int steps;
};

float DistanceField(vec3 p) {
	return length(p) - 1;
}

vec3 getNormal(vec3 p) {
	vec2 ff = vec2(0.0001, 0);
	return normalize(vec3(
		DistanceField(p + ff.xyy) - DistanceField(p - ff.xyy),
		DistanceField(p + ff.yxy) - DistanceField(p - ff.yxy),
		DistanceField(p + ff.yyx) - DistanceField(p - ff.yyx)
	));
}

RayHit castRay(vec3 ro, vec3 rd) {
	float t = 0, mind = DistanceField(ro), d;
	int steps = 0;
	for (int i = 0; i < MAX_STEPS; ++i) {
		steps = i + 1;
		if (t > MAX_DISTANCE) {
			break;
		}
		d = DistanceField(ro);
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
	return hit;
}

vec3 shadeObject(vec3 ro, vec3 rd, vec3 p) {
	vec3 n = getNormal(p);
	float light = max(dot(n, -lightDirection), 0) + .02;
	return vec3(light);
}

vec3 getRayColor(vec3 ro, vec3 rd) {
	RayHit hit = castRay(ro, rd);
	if (hit.mind < MIN_DE) {
		return shadeObject(ro, rd, ro + rd * hit.dist);
	}
	else {
		return vec3(0.);
	}
}

void main()
{
	color = vec4(getRayColor(camera.position, normalize(RayDirection)), 1);
}