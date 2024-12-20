#include "camera.h"
#include "color.h"
#include "common.h"
#include "material.h"
#include "sceneObject.h"
#include "sceneObjectList.h"
#include "sphere.h"

#include <fstream>

void display() {
	system("FoxViewPPM.exe out.ppm");
}

int main() {
	// scene
	sceneObjectList scene;
	shared_ptr<material> mat_ground = make_shared<lambertian>(color(0.8, 0.8, 0));
	shared_ptr<material> mat_matte1 = make_shared<lambertian>(color(0.1, 0.2, 0.5));
	shared_ptr<material> mat_metal1 = make_shared<metal>(color(0.8, 0.8, 0.8), 0.3);
	shared_ptr<material> mat_metal2 = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);
	shared_ptr<material> mat_glass1 = make_shared<dielectric>(1.5);
	shared_ptr<material> mat_glass2 = make_shared<dielectric>(1 / 1.5);

	scene.add(make_shared<sphere>(point3(0, 0, -1.2), 0.5, mat_matte1));
	scene.add(make_shared<sphere>(point3(-1, 0, -1), 0.5, mat_glass1));
	scene.add(make_shared<sphere>(point3(-1, 0, -1), 0.4, mat_glass2));
	scene.add(make_shared<sphere>(point3(1, 0, -1), 0.5, mat_metal2));
	scene.add(make_shared<sphere>(point3(0, -100.5, -1), 100, mat_ground));

	// camera
	double aspect_ratio = 16.0 / 9.0;
	int image_width = 400;
	render_params renp{ 10, 20 };
	camera_params camp{ 90, point3(-2, 2, 1), point3(0, 0, -1), vec3(0, 1, 0) };
	camera cam(aspect_ratio, image_width, renp, camp);
	
	// tasks
	cam.render(scene);
	display();
}