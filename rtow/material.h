#pragma once

#include "sceneObject.h"

class material {
public:
	virtual ~material() = default;

	virtual bool scatter(const ray& r_in, const intersection& rec, color& attenuation, ray& scattered) const {
		return false;
	}
};

class lambertian : public material {
public:
	lambertian(const color& albedo) : albedo(albedo) {}
	lambertian(const color& albedo, double scatter_prob) : albedo(albedo), p(scatter_prob) {}

	bool scatter(const ray& r_in, const intersection& rec, color& attenuation, ray& scattered) const override {
		vec3 scatter_dir = rec.normal + random_unit_vector();

		if (scatter_dir.near_zero()) {
			scatter_dir = rec.normal;
		}

		scattered = ray(rec.p, scatter_dir);
		attenuation = albedo / p;
		return true;
	}
private:
	color albedo;
	double p = 1;
};

class metal : public material {
public:
	metal(const color& albedo) : albedo(albedo) {}
	metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}
	
	bool scatter(const ray& r_in, const intersection& rec, color& attenuation, ray& scattered) const override {
		vec3 reflected = reflect(r_in.direction(), rec.normal);
		reflected = unit(reflected) + fuzz * random_unit_vector();
		
		scattered = ray(rec.p, reflected);
		attenuation = albedo;
		return dot(scattered.direction(), rec.normal) > 0;
	}

private:
	color albedo;
	double fuzz = 1;
};

class dielectric : public material {
public:
	dielectric(double refri) : refraction_index(refri) {}

	bool scatter(const ray& r_in, const intersection& rec, color& attenuation, ray& scattered) const override {
		attenuation = color(1, 1, 1);
		double refri = rec.faces_out ? (1 / refraction_index) : refraction_index;

		vec3 ud = unit(r_in.direction());
		double cos_theta = std::fmin(dot(-ud, rec.normal), 1);
		double sin_theta = std::sqrt(1 - cos_theta * cos_theta);

		vec3 dir;
		if (refri * sin_theta > 1 || reflectance(cos_theta, refri) > random_double()) {
			dir = reflect(ud, rec.normal);
		}
		else {
			// TODO refraction is broken
			dir = refract(ud, rec.normal, refri);
		}

		scattered = ray(rec.p, dir);
		return true;
	}
private:
	double refraction_index;

	static double reflectance(double cos, double refri) {
		double r0 = (1 - refri) / (1 + refri);
		r0 *= r0;
		return r0 + (1 - r0) * std::pow((1 - cos), 5);
	}
};