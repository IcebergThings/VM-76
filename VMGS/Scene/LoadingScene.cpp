//=============================================================================
// ■ VMGS/Scene/scene.cpp
//=============================================================================

#include "../VMGS.hpp"
#include <thread>

namespace VM76 {

	LoadingScene::LoadingScene(Scene* tobeload) {
		float aspectRatio = float(VMDE->width) / float(VMDE->height);
		gui_2d_projection = glm::ortho(0.0, 1.0 * aspectRatio, 0.0, 1.0, -1.0, 1.0);
		todo = tobeload;
	}

	// 这玩意不需要按键回调
	void LoadingScene::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {}

	bool first_load = true;
	bool sec_load = false;
	bool load_end = false;

	std::thread load_thr;

	void thread_load_task(EditorMainScene* todo, bool* theend) {
		todo->map = new Map(4,4,4,64); // Maximum capability of the card
		*theend = true;
	}

	void LoadingScene::render() {
		glDisable(GL_DEPTH_TEST);
		float aspectRatio = float(VMDE->width) / float(VMDE->height);

		char load_splash[] = "Loading    ";
		for (int i = 0; i < (VMDE->frame_count / 6) % 4; i++) load_splash[i + 8] = '.';
		trex->instanceRenderText(
			load_splash, gui_2d_projection,
			glm::mat4(1.0),
			glm::translate(glm::mat4(1.0),
				glm::vec3((1.0 * aspectRatio - 0.05 * 11) * 0.5,0.45,0.0)),
			0.05, 0.1, TextRenderer::TextDecorationType::OUTLINE
		);
		glEnable(GL_DEPTH_TEST);

		if (first_load) {
			sec_load = true;
			first_load = false;
		}
		if (sec_load) {
			// Load what we need to load
			todo = new EditorMainScene();
			load_thr = std::thread(thread_load_task, (EditorMainScene*) todo, &load_end);
			load_thr.detach();
			sec_load = false;
		}

		if (load_end) {
			SceneManager::load_scene(todo);
		}
	}

	// 这玩意暂时不需要刷新什么数据
	void LoadingScene::update() {}

	void LoadingScene::dispose() {}

}
