//=============================================================================
// ■ main.cpp
//-----------------------------------------------------------------------------
//   VMGS的主程序。
//=============================================================================

#include "VMGS.hpp"

namespace VM76 {

	void loop() {
		do {
			::main_draw_start();

			SceneManager::update_scene();
			SceneManager::render_scene();

			::main_draw_end();
		} while (!VMDE->done);
	}

	Scene* editor;

	void start_game() {
		::init_engine(860, 540, "VM / 76");
		init_control();

		// GL settings initialize
		glFrontFace(GL_CCW);
		glEnable(GL_CULL_FACE);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glDepthRange(0.0f, 1.0f);
		glClearDepth(1.0f);
		glDepthMask(GL_TRUE);

		editor = new EditorMainScene();
		SceneManager::load_scene(editor);
		glfwSetKeyCallback(window, SceneManager::key_callback);

		loop();
		terminate();
	}

	void terminate() {
		log("starting to terminate");
		VMDE_Dispose(editor);
		terminate_engine();
		log("terminated successfully");
	}
}

int main() {
	log("Hello! This is VM76. Nice to meet you!");
	VM76::start_game();
}
