//=============================================================================
// ■ main.cpp
//-----------------------------------------------------------------------------
//   VMGS的主程序。
//=============================================================================

#include "VMGS.hpp"

namespace VM76 {

	TextRenderer* trex;

	void loop() {
		do {
			::main_draw_start();

			SceneManager::update_scene();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			SceneManager::render_scene();

			::main_draw_end();
		} while (!VMDE->done);
	}

	static Scene* loading;
	static Scene* editor;

	void start_game() {
		::init_engine(860, 540, "VM / 76");
		init_control();

		trex = new TextRenderer();

		// GL settings initialize
		glFrontFace(GL_CCW);
		glEnable(GL_CULL_FACE);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glDepthRange(0.0f, 1.0f);
		glClearDepth(1.0f);
		glDepthMask(GL_TRUE);

		loading = new LoadingScene(editor);
		SceneManager::load_scene(loading);
		glfwSetKeyCallback(window, SceneManager::key_callback);

		loop();
		terminate();
	}

	void terminate() {
		log("starting to terminate");
		VMDE_Dispose(delete, editor);
		VMDE_Dispose(delete, trex);
		terminate_engine();
		log("terminated successfully");
	}
}

int main() {
	log("Hello! This is VM76. Nice to meet you!");
	VM76::start_game();
}
