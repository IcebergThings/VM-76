//=============================================================================
// ■ VMGS/Scene/scene.cpp
//=============================================================================

#include "../VMGS.hpp"
#include <thread>

namespace VM76 {

	LoadingScene::LoadingScene(Scene** tobeload) {
		todo = *tobeload;
	}

	// 这玩意不需要按键回调
	void LoadingScene::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {}

	bool first_load = true;

	void LoadingScene::render() {
		glDisable(GL_DEPTH_TEST);

		char load_splash[] = "Loading    ";
		for (int i = 0; i < (VMDE->frame_count / 6) % 4; i++) load_splash[i + 8] = '.';
		trex->instanceRenderText(
			load_splash, gui_2d_projection,
			glm::mat4(1.0),
			glm::translate(glm::mat4(1.0),
				glm::vec3((1.0 * aspect_ratio - 0.05 * 11) * 0.5,0.45,0.0)),
			0.05, 0.1, TextRenderer::TextDecorationType::NONE
		);
		VMSC::enable_depth_test();

		if (first_load) {
			first_load = false;
		} else {
			todo = (Scene*) new EditorMainScene();
			SceneManager::load_scene(todo);
		}
	}

	// 这玩意暂时不需要刷新什么数据
	void LoadingScene::update() {}

}
