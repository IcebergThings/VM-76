//=============================================================================
// ■ VMGS/Scene/scene.cpp
//=============================================================================

#include "../VMGS.hpp"

namespace VM76 {
	namespace SceneManager {
		Scene* context = NULL;
	}

	void Scene::update() {}
	Scene::~Scene() {}

	bool SceneManager::render_debug_info = true;

	void SceneManager::load_scene(Scene* c) {
		context = c;
	}

	void SceneManager::update_scene() {
		if (context) context->update();
	}

	void SceneManager::render_scene() {
		if (context) context->render();
		// Render FPS
		if (render_debug_info) {
			VMSC::disable_depth_test();
			char fps[40];
			sprintf(fps, "FPS: %d, %f.4ms", VMDE->fps, VMDE->frame_time);
			TextRenderer::BakeOptions opt = {
				.text = fps,
				.width = 0.025,
				.height = 0.05,
				.decoration = TextRenderer::TextDecorationType::OUTLINE,
				.color = glm::vec4(1.0, 1.0, 1.0, 1.0),
			};
			trex->instanceRenderText(
				&opt,
				gui_2d_projection,
				glm::mat4(1.0),
				glm::translate(glm::mat4(1.0), glm::vec3(0.01, 0.94, 0.0))
			);
			VMSC::enable_depth_test();
		}
	}

	void SceneManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_PAUSE && action == GLFW_PRESS && (mods & GLFW_MOD_CONTROL)) {
			abort();
		}
		if (key == GLFW_KEY_F3 && action == GLFW_PRESS) {
			render_debug_info = !render_debug_info;
			return;
		}
		if (context) {
			context->event_key(key, action);
			if (action == GLFW_PRESS) {
				context->event_keydown(key, mods);
			}
		}
	}
}
