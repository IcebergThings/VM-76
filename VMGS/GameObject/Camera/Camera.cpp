//=============================================================================
// ■ VMGS/Camera/Camera.cpp
//-----------------------------------------------------------------------------
// VMGS摄像机类
//=============================================================================

#include "../../VMGS.hpp"

namespace VM76 {

Camera* ActiveCamera = NULL;

void Camera::update() {
	glm::mat4 r = glm::rotate(glm::mat4(1.0), rotation.y, glm::vec3(0.0, 1.0, 0.0));
	r = glm::rotate(r, rotation.x, glm::vec3(1.0, 0.0, 0.0));
	r = glm::rotate(r, rotation.z, glm::vec3(0.0, 0.0, 1.0));

	forward = glm::mat3(r) * glm::vec3(0.0, 0.0, 1.0);
	up = glm::mat3(r) * glm::vec3(0.0, 1.0, 0.0);

	View = glm::lookAt(pos, pos + forward, up);
	view_camera = glm::lookAt(glm::vec3(0.0), forward, up);
}

CameraNoneRender::CameraNoneRender(glm::vec3 wpos, glm::vec3 rotate, glm::mat4 proj) {
	this->pos = wpos;
	this->rotation = rotate;
	this->Projection = proj;

	update();
}

void Camera::render(RenderHierarchy* scene_root) { }

Camera::~Camera() {}

}
