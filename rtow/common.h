#pragma once

#include <cmath>
#include <random>
#include <iostream>
#include <limits>
#include <memory>

using std::make_shared;
using std::shared_ptr;

// utils
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double deg_to_rad(double deg) {
	return deg * pi / 180;
}

inline double random_double() {
	static std::uniform_real_distribution<double> distribution(0, 1);
	static std::mt19937 generator;
	return distribution(generator);
}

inline double random_double(double min, double max) {
	return min + (max - min) * random_double();
}

// common headers
#include "color.h"
#include "interval.h"
#include "vec3.h"
#include "ray.h"