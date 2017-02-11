//=============================================================================
// ■ VMGS/Scene/scene.cpp
//=============================================================================

#include "../VMGS.hpp"

namespace VM76 {
	namespace SceneManager {
		Scene* context = NULL;
	}

	void Scene::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {}
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
			glDisable(GL_DEPTH_TEST);
			char fps[10];
			sprintf(fps, "FPS: %d", VMDE->fps);
			trex->instanceRenderText(
				fps, gui_2d_projection,
				glm::mat4(1.0),
				glm::translate(glm::mat4(1.0), glm::vec3(0.01, 0.94, 0.0)),
				0.025, 0.05, TextRenderer::TextDecorationType::OUTLINE
			);
			glEnable(GL_DEPTH_TEST);
		}
	}

	void SceneManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_F3 && action == GLFW_PRESS) {
			render_debug_info = !render_debug_info;
			return;
		}
		if (context) context->key_callback(window, key, scancode, action, mods);
	}
}
