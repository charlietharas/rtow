#pragma once

#include "common.h"
class material;

class intersection {
public:
	point3 p;
	vec3 normal;
	shared_ptr<material> mat;
	double t;
	bool faces_out;

	// assume outward_normal is a unit vector
	void set_face(const ray& r, const vec3& outward_normal) {
		faces_out = dot(r.direction(), outward_normal) < 0;
		normal = faces_out ? outward_normal : -outward_normal;
	}
};

class sceneObject {
public:
	virtual ~sceneObject() = default;

	virtual bool intersects(const ray& r, interval ray_t, intersection& rec) const = 0;
};