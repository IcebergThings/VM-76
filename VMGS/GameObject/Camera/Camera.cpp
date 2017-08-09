//=============================================================================
// ■ VMGS/Camera/Camera.hpp
//-----------------------------------------------------------------------------
// VMGS摄像机类
//=============================================================================

#include "Camera.hpp"

void Camera::update() {
	glm::mat4 r = glm::rotate(glm::mat4(1.0), rotate.y, glm::vec3(0.0, 1.0, 0.0));
	r = glm::rotate(r, rotate.x, glm::vec3(1.0, 0.0, 0.0));
	r = glm::rotate(r, rotate.z, glm::vec3(0.0, 0.0, 1.0));

	forward = glm::mat3(r) * glm::vec3(0.0, 0.0, 1.0);
	up = glm::mat3(r) * glm::vec3(0.0, 1.0, 0.0);

	View = glm::lookAt(wpos, wpos + forward, up);
}

Camera::Camera(glm::vec3 wpos, glm::vec3 rotate, glm::mat4 proj) {
	this->wpos = wpos;
	this->rotate = rotate;
	this->Projection = proj;

	update();
}

Camera::~Camera() {}
