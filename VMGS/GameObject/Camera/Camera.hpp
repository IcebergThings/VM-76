//=============================================================================
// ■ VMGS/Camera/Camera.hpp
//-----------------------------------------------------------------------------
// VMGS摄像机类
//=============================================================================

#ifndef _INCLUDE_CAMERA_H
#define _INCLUDE_CAMERA_H

#include <global.hpp>

class Camera {
public:
	glm::vec3 wpos, rotate;
	glm::vec3 forward, up;
	glm::mat4 Projection, View, view_camera;
	void update();

public:
	Camera(glm::vec3 wpos, glm::vec3 rotate, glm::mat4 proj);
	~Camera();
};

#endif
