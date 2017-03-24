//=============================================================================
// ■ VMGS/Scene/EditorMainScene.cpp
//-----------------------------------------------------------------------------
//   编辑器场景。
//=============================================================================

#include "../VMGS.hpp"

namespace VM76 {
	//-------------------------------------------------------------------------
	// ● 场景开始
	//-------------------------------------------------------------------------
	EditorMainScene::EditorMainScene() {
		obj = new GObject();

		shader_textured.add_file(GL_VERTEX_SHADER, "../Media/shaders/gbuffers_textured.vsh");
		shader_textured.add_file(GL_FRAGMENT_SHADER, "../Media/shaders/gbuffers_textured.fsh");
		shader_textured.link_program();
		shader_basic.add_file(GL_VERTEX_SHADER, "../Media/shaders/gbuffers_basic.vsh");
		shader_basic.add_file(GL_FRAGMENT_SHADER, "../Media/shaders/gbuffers_basic.fsh");
		shader_basic.link_program();
		gui.add_file(GL_VERTEX_SHADER, "../Media/shaders/gui.vsh");
		gui.add_file(GL_FRAGMENT_SHADER, "../Media/shaders/gui.fsh");
		gui.link_program();
		post_processing.add_file(GL_VERTEX_SHADER, "../Media/shaders/PostProcessing.vsh");
		post_processing.add_file(GL_FRAGMENT_SHADER, "../Media/shaders/PostProcessing.fsh");
		post_processing.link_program();
		postBuffer = new RenderBuffer(VMDE->width, VMDE->height);

		projection = glm::perspective(1.3f, aspect_ratio, 0.1f, 1000.0f);
		view = glm::lookAt(
			glm::vec3(0.0, 2.6, 0.0),
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec3(0.0, 1.0, 0.0)
		);

		// Set up hand block indicator's matrix
		glm::mat4 block_display = glm::translate(
			glm::mat4(1.0),
			glm::vec3(0.02, 0.06, 0.2)
		);
		block_display = glm::scale(block_display, glm::vec3(0.1f));
		block_display = glm::rotate(block_display,
			VMath::PIf / 4.0f,
			glm::vec3(1.0, 0.0, 0.0)
		);
		block_display = glm::rotate(block_display,
			VMath::PIf / 4.0f,
			glm::vec3(0.0, 1.0, 0.0)
		);

		TiledMap::init_cinstances(clist);
		for (int i = 0; i < 16; i++) {
			clist[i]->mat[0] = new glm::mat4[1]; clist[i]->mat[0][0] = block_display;
			clist[i]->mat[1] = new glm::mat4[1]; clist[i]->mat[1][0] = block_display;
			clist[i]->mat[2] = new glm::mat4[1]; clist[i]->mat[2][0] = block_display;
			clist[i]->mat[3] = new glm::mat4[1]; clist[i]->mat[3][0] = block_display;
			clist[i]->mat[4] = new glm::mat4[1]; clist[i]->mat[4][0] = block_display;
			clist[i]->mat[5] = new glm::mat4[1]; clist[i]->mat[5][0] = block_display;
			clist[i]->update_instance(1,1,1,1,1,1);
		}
		block_pointer.obj->data.mat_c = 1;
	}
	//-------------------------------------------------------------------------
	// ● 按键回调
	//-------------------------------------------------------------------------
	void EditorMainScene::key_callback(
		GLFWwindow* window, int key, int scancode, int action, int mods
	) {
		#define PRESS(n) key == n && action == GLFW_PRESS
		if (PRESS(GLFW_KEY_A)) obj->move(glm::vec3(-1.0, 0.0, 0.0));
		if (PRESS(GLFW_KEY_D)) obj->move(glm::vec3(1.0, 0.0, 0.0));
		if (PRESS(GLFW_KEY_W)) obj->move(glm::vec3(0.0, 0.0, -1.0));
		if (PRESS(GLFW_KEY_S)) obj->move(glm::vec3(0.0, 0.0, 1.0));
		if (PRESS(GLFW_KEY_UP)) obj->move(glm::vec3(0.0, 1.0, 0.0));
		if (PRESS(GLFW_KEY_DOWN)) obj->move(glm::vec3(0.0, -1.0, 0.0));

		if (PRESS(GLFW_KEY_0)) hand_id = 0;
		if (PRESS(GLFW_KEY_0)) hand_id = 0;
		if (PRESS(GLFW_KEY_1)) hand_id = 1;
		if (PRESS(GLFW_KEY_2)) hand_id = 2;
		if (PRESS(GLFW_KEY_3)) hand_id = 3;
		if (PRESS(GLFW_KEY_4)) hand_id = 4;
		if (PRESS(GLFW_KEY_5)) hand_id = 5;
		if (PRESS(GLFW_KEY_6)) hand_id = 6;
		if (PRESS(GLFW_KEY_7)) hand_id = 7;
		if (PRESS(GLFW_KEY_8)) hand_id = 8;
		if (PRESS(GLFW_KEY_9)) hand_id = 9;

		if (PRESS(GLFW_KEY_SPACE)) {
			map.place_block(obj->pos, hand_id);
		}

		static Audio::Channel_Vorbis* loop = NULL;
		static Audio::Channel_Triangle* triangle = NULL;
		static Audio::Channel_Sine* sine = NULL;
		if (PRESS(GLFW_KEY_SEMICOLON)) {
			Audio::play_sound("../Media/soft-ping.ogg", false);
		}
		if (PRESS(GLFW_KEY_APOSTROPHE)) {
			if (loop) {
				Audio::stop(loop);
				loop = NULL;
			} else {
				loop = Audio::play_sound("../Media/loop-test.ogg", true, .1f);
			}
		}
		if (PRESS(GLFW_KEY_LEFT_BRACKET)) {
			if (triangle) {
				Audio::stop(triangle);
				triangle = NULL;
			} else {
				triangle = new Audio::Channel_Triangle(440);
				Audio::play_channel(triangle);
			}
		}
		if (PRESS(GLFW_KEY_RIGHT_BRACKET)) {
			if (sine) {
				Audio::stop(sine);
				sine = NULL;
			} else {
				sine = new Audio::Channel_Sine(440);
				Audio::play_channel(sine);
			}
		}
		#undef PRESS
	}
	//-------------------------------------------------------------------------
	// ● 刷新
	//-------------------------------------------------------------------------
	void EditorMainScene::update() {
		update_control();
	}
	//-------------------------------------------------------------------------
	// ● 渲染
	//-------------------------------------------------------------------------
	void EditorMainScene::render() {
		shader_textured.use();

		// Setup uniforms
		shader_textured.set_float("brightness", VMDE->state.brightness);
		shader_textured.set_texture("colortex0", &tile_texture, 0);

		postBuffer->bind();
		RenderBuffer::clearColorDepth(0.5, 0.7, 1.0, 0.0);

		// Textured blocks rendering
		shader_textured.ProjectionView(projection, view);
		map.render();

		// Setup uniforms
		// Non textured rendering
		shader_basic.use();
		shader_basic.set_float("opaque", 0.5);
		shader_textured.ProjectionView(projection, view);
		block_pointer.mat[0] = obj->transform();
		block_pointer.update_instance(1);
		block_pointer.render();

		axe.render();

		postBuffer->unbind();
		post_processing.use();
		post_processing.set_texture("colortex", postBuffer->texture_buffer, 0);
		PostProcessingManager::Blit2D();

		// GUI rendering
		gui.use();
		gui.set_texture("atlastex", &tile_texture, 0);
		gui.ProjectionView(gui_2d_projection, glm::mat4(1.0));
		glDisable(GL_DEPTH_TEST);
		if (hand_id > 0) clist[hand_id - 1]->render();

		if (SceneManager::render_debug_info) {
			char info[64];
			sprintf(info, "Hand ID: %d  Pointer ID: %d",
				hand_id,
				map.map->tidQuery(obj->pos.x, obj->pos.y, obj->pos.z)
			);
			trex->instanceRenderText(
				info, gui_2d_projection,
				glm::mat4(1.0),
				glm::translate(glm::mat4(1.0), glm::vec3(0.01, 0.88, 0.0)),
				0.025, 0.05, TextRenderer::TextDecorationType::OUTLINE
			);
			sprintf(info, "Pointer pos: (%.0f, %.0f, %.0f)",
				obj->pos.x, obj->pos.y, obj->pos.z
			);
			trex->instanceRenderText(
				info, gui_2d_projection,
				glm::mat4(1.0),
				glm::translate(glm::mat4(1.0), glm::vec3(0.01, 0.82, 0.0)),
				0.025, 0.05, TextRenderer::TextDecorationType::OUTLINE
			);
		}
		glEnable(GL_DEPTH_TEST);
	}
	//-------------------------------------------------------------------------
	// ● 释放
	//-------------------------------------------------------------------------
	EditorMainScene::~EditorMainScene() {
		for (int i = 0; i < 16; i++) VMDE_Dispose(delete, clist[i]);
		VMDE_Dispose(delete, trex);
	}
}
