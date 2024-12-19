#pragma once

#include "common.h"
#include "sceneObject.h"

#include <vector>

class sceneObjectList : public sceneObject {
public:
	std::vector<shared_ptr<sceneObject>> objects;

	sceneObjectList() {}
	sceneObjectList(shared_ptr<sceneObject> object) { add(object); }

	void clear() { objects.clear(); }
	void add(shared_ptr<sceneObject> object) { objects.push_back(object); }

	bool intersects(const ray& r, interval ray_t, intersection& rec) const override {
		intersection temp_intersection;
		bool hit = false;
		double closest = ray_t.max;

		for (const auto& object : objects) {
			if (object->intersects(r, interval(ray_t.min, closest), temp_intersection)) {
				hit = true;
				closest = temp_intersection.t;
				rec = temp_intersection;
			}
		}

		return hit;
	}
};