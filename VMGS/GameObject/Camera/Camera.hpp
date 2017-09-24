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
	void render(RenderHierarchy* scene_root);

	virtual ~Camera();
};

class CameraNoneRender : public Camera {
public:
	CameraNoneRender(glm::vec3 wpos, glm::vec3 rotate, glm::mat4 proj);
};

//-------------------------------------------------------------------------
// ● 缓冲区设定
//-------------------------------------------------------------------------
enum BuffersIndex {
	BufferAlbedo,
	BufferNormal,
	BufferLighting,
	BufferComposite
};

class CameraDeferred : public Camera {
public:
	RenderBuffer* gbuffers;
	void render(RenderHierarchy* scene_root);

	static RenderBuffer* create_gbuffers();

	CameraDeferred(glm::vec3 wpos, glm::vec3 rotate, glm::mat4 proj);
};

extern Camera* ActiveCamera;

}
