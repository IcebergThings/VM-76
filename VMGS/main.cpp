//=============================================================================
// ■ main.cpp
//-----------------------------------------------------------------------------
//   VMGS的主程序。
//=============================================================================

#include "VMGS.hpp"

namespace VM76 {
	//-------------------------------------------------------------------------
	// ● 变量
	//-------------------------------------------------------------------------
	Scene* editor;
	//-------------------------------------------------------------------------
	// ● 主循环
	//-------------------------------------------------------------------------
	void loop() {
		do {
			::main_draw_start();

			SceneManager::update_scene();
			SceneManager::render_scene();

			::main_draw_end();
		} while (!VMDE->done);
	}
	//-------------------------------------------------------------------------
	// ● 启动
	//-------------------------------------------------------------------------
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
	//-------------------------------------------------------------------------
	// ● 结束
	//-------------------------------------------------------------------------
	void terminate() {
		log("starting to terminate");
		VMDE_Dispose(delete, editor);
		terminate_engine();
		log("terminated successfully");
	}
}

// 入口点
int main(int argc, char** argv) {
	log("Hello! This is VM76. Nice to meet you!");
	VM76::start_game();
}
