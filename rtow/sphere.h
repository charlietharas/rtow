#pragma once

#include "common.h"
#include "sceneObject.h"

class sphere : public sceneObject {
public:
	sphere(const point3& center, double radius, shared_ptr<material> mat) : center(center), radius(std::fmax(0, radius)), mat(mat) {}

	bool intersects(const ray& r, interval ray_t, intersection& rec) const override {
		vec3 oc = center - r.origin();
		double a = r.direction().length_squared();
		double h = dot(r.direction(), oc);
		double c = oc.length_squared() - radius * radius;
		double disc = h * h - a * c;
		rec.mat = mat;

		if (disc < 0) {
			return false;
		}
		
		double disc_sqrt = std::sqrt(disc);

		// nearest root that lies in the acceptable range
		double root = (h - disc_sqrt) / a;
		if (!ray_t.surrounds(root)) {
			root = (h + disc_sqrt) / a;
			if (!ray_t.surrounds(root)) {
				return false;
			}
		}

		rec.t = root;
		rec.p = r.at(rec.t);
		rec.set_face(r, (rec.p - center) / radius);

		return true;
	}

private:
	point3 center;
	double radius;
	shared_ptr<material> mat;
};