#pragma once

#include "material.h"
#include "sceneObject.h"

#include <fstream>

struct render_params {
	int samples_per_pixel; // antialiasing
	int max_recursion_depth; // ray scattering
};

struct camera_params {
	// TODO;
};

class camera {
public:
	double aspect_ratio = 1.0;
	int image_width = 100;
	int samples_per_pixel = 10;
	int max_recursion_depth = 10;
	double gamma = 0.25;

	camera() { init(); }
	camera(double asp, int imw) : aspect_ratio(asp), image_width(imw) { init(); }
	camera(double asp, int imw, render_params& params) : aspect_ratio(asp), image_width(imw) {
		samples_per_pixel = params.samples_per_pixel;
		max_recursion_depth = params.max_recursion_depth;
		init();
	}

	void render(const sceneObject& scene) {
		std::ofstream outfile;
		outfile.open("out.ppm");
		outfile << "P3\n" << image_width << ' ' << image_height << "\n255\n";

		for (int j = 0; j < image_height; j++) {
			std::clog << "\r" << std::round(double(j) / image_height * 100 * 10) / 10 << '%' << std::flush;
			for (int i = 0; i < image_width; i++) {
				color pixel(0, 0, 0);
				// anti-aliasing
				for (int sample = 0; sample < samples_per_pixel; sample++) {
					ray r = get_ray(i, j);
					pixel += ray_color(r, scene);
				}
				write_color(outfile, pixel_samples_scale * pixel);
			}
		}

		std::clog << "\nDone!\n";

		outfile.close();
	}

private:
	int image_height;
	double pixel_samples_scale;
	point3 center;
	point3 pixel_topleft;
	vec3 pixel_delta_u;
	vec3 pixel_delta_v;

	void init() {
		image_height = int(image_width / aspect_ratio);
		image_height = (image_height < 1) ? 1 : image_height;

		pixel_samples_scale = 1.0 / samples_per_pixel;

		center = point3(0, 0, 0);

		double focal_length = 1.0;
		double viewport_height = 2.0;
		double viewport_width = viewport_height * (double(image_width) / image_height);

		vec3 viewport_u = vec3(viewport_width, 0, 0);
		vec3 viewport_v = vec3(0, -viewport_height, 0);

		pixel_delta_u = viewport_u / image_width;
		pixel_delta_v = viewport_v / image_height;

		vec3 viewport_topleft = center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
		pixel_topleft = viewport_topleft + (pixel_delta_u + pixel_delta_v) / 2;
	}

	ray get_ray(int i, int j) const {
		vec3 offset = sample_square();
		vec3 pixel_sample = pixel_topleft + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);

		vec3 origin = center;
		vec3 direction = pixel_sample - origin;

		return ray(origin, direction);
	}

	vec3 sample_square() const {
		return vec3(random_double() - 0.5, random_double() - 0.5, 0);
	}

	color ray_color(const ray& r, const sceneObject& scene) {
		return ray_color(r, scene, max_recursion_depth);
	}

	color ray_color(const ray& r, const sceneObject& scene, int depth) {
		if (depth <= 0) {
			return color(0, 0, 0);
		}

		intersection rec;
		if (scene.intersects(r, interval(1e-3, infinity), rec)) {
			ray scattered;
			color attenuation;
			if (rec.mat->scatter(r, rec, attenuation, scattered)) {
				return attenuation * ray_color(scattered, scene, depth-1);
			}
			return color(0, 0, 0);
		}

		vec3 dir = unit(r.direction());
		double a = (dir.y() + 1) / 2;
		return lerp(color(1, 1, 1), color(0.5, 0.7, 1.0), a);
	}

};