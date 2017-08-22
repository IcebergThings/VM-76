//=============================================================================
// ■ VMGS/Scene/scene.cpp
//=============================================================================

#include "../VMGS.hpp"
#include <thread>

namespace VM76 {

	LoadingScene::LoadingScene(Scene** tobeload) {
		todo = *tobeload;
	}

	bool first_load = true;

	void LoadingScene::render() {
		VMSC::disable_depth_test();

		char load_splash[] = "Loading    ";
		for (int i = 0; i < (VMDE->frame_count / 6) % 4; i++) load_splash[i + 8] = '.';
		TextRenderer::BakeOptions opt = {
			.text = load_splash,
			.width = 0.05,
			.height = 0.1,
			.decoration = TextRenderer::TextDecorationType::NONE,
			.color = glm::vec4(1.0, 1.0, 1.0, 1.0),
		};
		trex->instanceRenderText(
			&opt,
			gui_2d_projection,
			glm::mat4(1.0),
			glm::translate(
				glm::mat4(1.0),
				glm::vec3((1.0 * aspect_ratio - 0.05 * 11) * 0.5, 0.45, 0.0)
			)
		);
		VMSC::enable_depth_test();

		if (first_load) {
			first_load = false;
		} else {
			todo = (Scene*) new Scene_Editor();
			SceneManager::load_scene(todo);
		}
	}

	// 这玩意暂时不需要刷新什么数据
	void LoadingScene::update() {}

}
