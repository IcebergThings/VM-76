//=============================================================================
// ■ VMGS/global.hpp
//=============================================================================

#include <global.hpp>
#include <config.hpp>

#include "GameObject/GameObject.hpp"
#include "Control/control.hpp"

#ifndef _INCLUDE_VMGS_GLOBAL_H
	#define _INCLUDE_VMGS_GLOBAL_H

	#undef DEBUG_ENVIRONMENT
	#define DEBUG_ENVIRONMENT "VMGS"

	//-------------------------------------------------------------------------
	// ● TODO
	//-------------------------------------------------------------------------
	namespace VM76 {
		void loop();
		void start_game();
		void terminate();
		void init_tiles();

	//-------------------------------------------------------------------------
	// ● Scene Management
	//-------------------------------------------------------------------------
		class Scene : public Object {
		public:
			virtual void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
			virtual void render(); // Graphics update
			virtual void update(); // Tick update
			virtual void dispose();
		};

		class LoadingScene : public Scene {
		private:
			glm::mat4 gui_2d_projection;
			TextRenderer* trex = NULL;
			Scene* todo = NULL;

		public:
			LoadingScene(Scene* tobeload);
			void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
			void render();
			void update();
			void dispose();
		};

		class EditorMainScene : public Scene {
		private:
			Shaders* shader_textured = NULL;
			Shaders* gui = NULL;
			Shaders* shader_basic = NULL;
			Res::Texture* tile_texture = NULL;

			TextRenderer* trex = NULL;

			Cube* block_pointer;
			Tiles* clist[16];
			Map* map;

			glm::mat4 gui_2d_projection;

			int hand_id = 1;

			Axis* axe;

			GObject* obj;

		public:
			EditorMainScene();
			void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
			void render();
			void update();
			void dispose();
		};

		class SceneManager : public Object {
		public:
			static void load_scene(Scene* c);
			static void render_scene();
			static void update_scene();
			static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		};

	//-------------------------------------------------------------------------
	// ● Game Logic
	//-------------------------------------------------------------------------
		void init_logic();
		void update_logic();
		void render();
	}

#endif
