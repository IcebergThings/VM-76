//=============================================================================
// ■ main.cpp
//-----------------------------------------------------------------------------
//   VMGS的主程序。
//=============================================================================

#include "VMGS.hpp"

namespace VM76 {

	TextRenderer* trex;
	glm::mat4 gui_2d_projection;

	void loop() {
		do {
			::main_draw_start();

			SceneManager::update_scene();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			SceneManager::render_scene();

			glDisable(GL_DEPTH_TEST);
			char fps[10];
			sprintf(fps, "FPS: %d", VMDE->fps);
			trex->instanceRenderText(
				fps, gui_2d_projection,
				glm::mat4(1.0),
				glm::translate(glm::mat4(1.0),
					glm::vec3(0.01,0.94,0.0)),
				0.025, 0.05, TextRenderer::TextDecorationType::OUTLINE
			);
			glEnable(GL_DEPTH_TEST);

			::main_draw_end();
		} while (!VMDE->done);
	}

	static Scene* loading;
	static Scene* editor;

	void start_game() {
		::init_engine(860, 540, "VM / 76");
		init_control();

		trex = new TextRenderer();
		float aspectRatio = float(VMDE->width) / float(VMDE->height);
		gui_2d_projection = glm::ortho(0.0, 1.0 * aspectRatio, 0.0, 1.0, -1.0, 1.0);

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
