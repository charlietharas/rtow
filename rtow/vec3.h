#pragma once

#include "common.h"

class vec3 {
public:
	double v[3];

	vec3() : v{ 0, 0, 0 } {}
	vec3(double v0, double v1, double v2) : v{ v0, v1, v2 } {}

	double x() const { return v[0]; }
	double y() const { return v[1]; }
	double z() const { return v[2]; }

	vec3 operator-() const { return vec3(-v[0], -v[1], -v[2]); }
	double operator[](int i) const { return v[i]; }
	double& operator[](int i) { return v[i]; }

	vec3& operator+=(const vec3& u) {
		v[0] += u[0];
		v[1] += u[1];
		v[2] += u[2];
		return *this;
	}

	vec3& operator*=(double a) {
		v[0] *= a;
		v[1] *= a;
		v[2] *= a;
		return *this;
	}

	vec3& operator/=(double a) {
		v[0] /= a;
		v[1] /= a;
		v[2] /= a;
		return *this;
	}

	double length() const {
		return std::sqrt(length_squared());
	}

	double length_squared() const {
		return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
	}

	bool near_zero() const {
		double epsilon = 1e-8;
		return std::fabs(v[0]) < epsilon && std::fabs(v[1]) < epsilon && std::fabs(v[2]) < epsilon;
	}

	static vec3 random() {
		return vec3(random_double(), random_double(), random_double());
	}

	static vec3 random(double min, double max) {
		return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
	}
};

using point3 = vec3;

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
	return out << v[0] << ' ' << v[1] << ' ' << v[2];
}

inline vec3 operator+(const vec3& u, const vec3& v) {
	return vec3(u[0] + v[0], u[1] + v[1], u[2] + v[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
	return vec3(u[0] - v[0], u[1] - v[1], u[2] - v[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
	return vec3(u[0] * v[0], u[1] * v[1], u[2] * v[2]);
}

inline vec3 operator/(const vec3& u, const vec3& v) {
	return vec3(u[0] / v[0], u[1] / v[1], u[2] / v[2]);
}

inline vec3 operator*(double a, const vec3& v) {
	return vec3(v[0] * a, v[1] * a, v[2] * a);
}

inline vec3 operator*(const vec3& v, double a) {
	return vec3(v[0] * a, v[1] * a, v[2] * a);
}

inline vec3 operator/(const vec3& v, double a) {
	return vec3(v[0] / a, v[1] / a, v[2] / a);
}

inline double dot(const vec3& u, const vec3& v) {
	return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
	return vec3(u[1] * v[2] - u[2] * v[1], u[2] * v[0] - u[0] * v[2], u[0] * v[1] - u[1] * v[0]);
}

inline double dist(const vec3& u, const vec3& v) {
	return (u - v).length();
}

// does not normalize a to [0, 1]
inline vec3 lerp(const vec3& u, const vec3& v, const double a) {
	return (1.0 - a) * u + a * v;
}

inline vec3 unit(const vec3& v) {
	return v / v.length();
}

inline vec3 random_unit_vector() {
	while (true) {
		vec3 p = vec3::random(-1, 1);
		double lensq = p.length_squared();
		if (1e-160 < lensq && lensq <= 1) {
			return p / std::sqrt(lensq);
		}
	}
}

inline vec3 random_on_disk() {
	while (true) {
		vec3 p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
		if (p.length_squared() < 1) {
			return p;
		}
	}
}

inline vec3 random_on_hemisphere(const vec3& normal) {
	vec3 on_unit_sphere = random_unit_vector();
	if (dot(on_unit_sphere, normal) > 0) {
		return on_unit_sphere;
	}
	return -on_unit_sphere;
}

inline vec3 reflect(const vec3& v, const vec3& n) {
	return v - 2 * dot(v, n) * n;
}

inline vec3 refract(const vec3& uv, const vec3& n, double refri) {
	double cos_theta = std::fmin(dot(-uv, n), 1);
	vec3 r_out_perp = refri * (uv + cos_theta * n);
	vec3 r_out_para = -std::sqrt(std::fabs(1 - r_out_perp.length_squared())) * n;
	return r_out_perp + r_out_para;
}