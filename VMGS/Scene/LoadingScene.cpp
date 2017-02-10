//=============================================================================
// ■ VMGS/Scene/scene.cpp
//=============================================================================

#include "../VMGS.hpp"

namespace VM76 {

	LoadingScene::LoadingScene(Scene* tobeload) {
		float aspectRatio = float(VMDE->width) / float(VMDE->height);
		trex = new TextRenderer();
		gui_2d_projection = glm::ortho(0.0, 1.0 * aspectRatio, 0.0, 1.0, -1.0, 1.0);
		todo = tobeload;
	}

	// 这玩意不需要按键回调
	void LoadingScene::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {}

	bool first_load = true;

	void LoadingScene::render() {
		glDisable(GL_DEPTH_TEST);
		float aspectRatio = float(VMDE->width) / float(VMDE->height);
		trex->instanceRenderText(
			"Loading ...", gui_2d_projection,
			glm::mat4(1.0),
			glm::translate(glm::mat4(1.0),
				glm::vec3((1.0 * aspectRatio - 0.05 * 11) * 0.5,0.45,0.0)),
			0.05, 0.1, TextRenderer::TextDecorationType::OUTLINE
		);
		glEnable(GL_DEPTH_TEST);

		if (!first_load) {
			// Load what we need to load
			todo = new EditorMainScene();

			dispose();
			SceneManager::load_scene(todo);
		}

		first_load = false;
	}

	// 这玩意暂时不需要刷新什么数据
	void LoadingScene::update() {}

	void LoadingScene::dispose() {
		VMDE_Dispose(trex);
	}

}
