//=============================================================================
// ■ VMGS/GameObject/Camera/Camera.hpp
//-----------------------------------------------------------------------------
//   VMGS摄像机类
//=============================================================================

#pragma once

namespace VM76 {

class Camera : public GObject {
public:
	glm::vec3 forward, up;
	glm::mat4 Projection, View, view_camera;
	void update();

public:
	Camera(glm::vec3 wpos, glm::vec3 rotate, glm::mat4 proj);
	~Camera();
};

}
