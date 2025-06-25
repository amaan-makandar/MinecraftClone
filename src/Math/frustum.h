#pragma once
#include "glm/glm.hpp"
#include "Camera/Camera.h"
#include <iostream>

/*
	- world-space coords of chunk AABB:
		- (Cx, 0, Cz)
		- (Cx + width, 0, Cz)
		- (Cx, 0, Cz + width)
		- (Cx + width, 0, Cz + width)
		- same each but with y = height
		
		- all the info you need is (Cx, 0, Cz), (Cx + width, height, Cz + width)
			- take all combos of pts

	- clip-space coords of frustum
		- (-1, -1, near, 1)
		- (-1, 1, near, 1)
		- (1, -1, near, 1)
		- (1, 1, near, 1)

	- invert proj and view matrices to get world-space coords of frustum
	- construct planes from frustum coords
	- cross product 
*/

struct Plane {
	glm::vec3 normal;
	glm::vec3 point;

	void print() {
		std::cout << "pt: " << point.x << "," << point.y << "," << point.z << "\n";
	}
};


struct AABB {
	glm::vec3 base_pt;
	glm::vec3 upper_pt;
};


struct Frustum {
	Plane top;
	Plane bottom;

	Plane right;
	Plane left;

	Plane far;
	Plane near;

	void print() {
		std::cout << "==============\n";
		top.print();
		bottom.print();
		right.print();
		left.print();
		far.print();
		near.print();
		std::cout << "==============\n";
	}
};

glm::vec3 ndc_to_world(glm::mat4& inv_pv, glm::vec3 ndc) {
	glm::vec4 ndc_w = {ndc.x, ndc.y, ndc.z, 1.0f};
	glm::vec4 res = inv_pv * ndc_w;
	res /= res.w;

	return { res.x, res.y, res.z };
}

Frustum get_frustum_from_camera(Camera& camera) {

	Frustum frustum;
	// (PV)^(-1)
	// multiply by NDC edges to get frustum pts
	glm::mat4 inv_pv = glm::inverse(camera.getPerspectiveMatrix() * camera.getViewMatrix());
	float near_z = camera.get_near_z();
	float far_z = camera.get_far_z();
	
	glm::vec3 a = ndc_to_world(inv_pv, { -1.0f, -1.0f, -1.0f });
	glm::vec3 b = ndc_to_world(inv_pv, { 1.0f, -1.0f, -1.0f });
	glm::vec3 c = ndc_to_world(inv_pv, { -1.0f, 1.0f, -1.0f });
	glm::vec3 d = ndc_to_world(inv_pv, { 1.0f, 1.0f, -1.0f });
	glm::vec3 e = ndc_to_world(inv_pv, { -1.0f, -1.0f, 1.0f });
	glm::vec3 f = ndc_to_world(inv_pv, { 1.0f, -1.0f, 1.0f });
	glm::vec3 g = ndc_to_world(inv_pv, { -1.0f, 1.0f, 1.0f });
	glm::vec3 h = ndc_to_world(inv_pv, { 1.0f, 1.0f, 1.0f });

	frustum.near = { glm::cross(c - a, b - a), a };
	frustum.far = { -frustum.near.normal, e };
	frustum.left = { glm::cross(g - e, a - e), e };
	frustum.right = { glm::cross(f - b, d - b), b };
	frustum.top = { glm::cross(h - g, c - g), g };
	frustum.left = { glm::cross(d - e, a - e), d };

	return frustum;
}