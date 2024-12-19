#include "color.h"
#include "interval.h"
#include "vec3.h"

inline double linear_to_gamma(double linear) {
	if (linear > 0) {
		return std::sqrt(linear);
	}
	return 0;
}

void write_color(std::ostream& out, const color& p) {
	double r = p.x();
	double g = p.y();
	double b = p.z();

	r = linear_to_gamma(r);
	g = linear_to_gamma(g);
	b = linear_to_gamma(b);

	static const interval intensity(0, 0.999);
	int rbyte = int(256 * intensity.clamp(r));
	int gbyte = int(256 * intensity.clamp(g));
	int bbyte = int(256 * intensity.clamp(b));

	out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}