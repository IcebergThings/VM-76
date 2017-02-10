//=============================================================================
// ■ VMGS/Scene/scene.cpp
//=============================================================================

#include "../VMGS.hpp"

namespace VM76 {

	Scene* context = NULL;

	void Scene::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {}
	void Scene::render() {}
	void Scene::update() {}
	void Scene::dispose() {}

	void SceneManager::load_scene(Scene* c) { context = c; }

	void SceneManager::update_scene() { if (context) context->update(); }

	void SceneManager::render_scene() { if (context) context->render(); }

	void SceneManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (context) context->key_callback(window, key, scancode, action, mods);
	}
}
