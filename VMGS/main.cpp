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
	TextRenderer* trex;
	static Scene* loading;
	static Scene_Editor* editor;
	float aspect_ratio;
	glm::mat4 gui_2d_projection;
	//-------------------------------------------------------------------------
	// ● 主循环
	//-------------------------------------------------------------------------
	void loop() {
		while (!VMDE->done) {
			::main_draw_start();
			SceneManager::update_scene();

			RenderBuffer::clearBuffer(glm::vec4(.0f), true, true, false);
			VMSC::enable_blend();
			SceneManager::render_scene();
			::main_draw_end();
		}
	}
	//-------------------------------------------------------------------------
	// ● 启动
	//-------------------------------------------------------------------------
	void start_game() {
		::init_engine(860, 540, "VM / 76");

		trex = new TextRenderer();
		aspect_ratio = (float) VMDE->width / VMDE->height;
		gui_2d_projection = glm::ortho(0.0, 1.0 * aspect_ratio, 0.0, 1.0, -1.0, 1.0);

		loading = new LoadingScene((Scene**) &editor);
		SceneManager::load_scene(loading);
		glfwSetKeyCallback(window, SceneManager::key_callback);

		loop();
		terminate();
	}
	//-------------------------------------------------------------------------
	// ● 结束
	//-------------------------------------------------------------------------
	void terminate() {
		log("starting to terminate");
		Scene_Editor* editor =(Scene_Editor*) SceneManager::context;
		VMDE_Dispose(delete, editor);
		VMDE_Dispose(delete, loading);
		VMDE_Dispose(delete, trex);
		terminate_engine();
		log("terminated successfully");
	}
}

// 入口点
int main(int argc, char** argv) {
	log("Hello! This is VM76. Nice to meet you!");
	VM76::start_game();
}
