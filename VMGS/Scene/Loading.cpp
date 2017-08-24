//=============================================================================
// ■ VMGS/Scene/scene.cpp
//=============================================================================

#include "../VMGS.hpp"

namespace VM76 {
	//-------------------------------------------------------------------------
	// ● 构造
	//    loader : 加载用函数。会在另一个线程上执行此函数，加载完后应设置completion_sign为true。
	//-------------------------------------------------------------------------
	Scene_Loading::Scene_Loading(void (*loader)(bool* completion_sign)) {
		loader_completed = false;
		loader_thread = thread(loader, &loader_completed);
	}
	//-------------------------------------------------------------------------
	// ● 更新
	//-------------------------------------------------------------------------
	void Scene_Loading::update() {
		Scene::update();
		if (loader_completed) {
			loader_thread.join();
			SceneManager::jump<Scene_Editor>();
		}
	}
	//-------------------------------------------------------------------------
	// ● 绘制
	//-------------------------------------------------------------------------
	void Scene_Loading::render() {
		Scene::render();

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
	}
}
