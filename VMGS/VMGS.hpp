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

		extern TextRenderer* trex;
		extern glm::mat4 gui_2d_projection;
		extern float aspect_ratio;

	//-------------------------------------------------------------------------
	// ● Scenes
	//-------------------------------------------------------------------------
		class Scene : public Object {
		public:
			virtual void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
			virtual void render() = 0; // Graphics update
			virtual void update(); // Tick update
			virtual void dispose();
		};

		class EditorMainScene : public Scene {
		private:
			Shaders* shader_textured = NULL;
			Shaders* gui = NULL;
			Shaders* shader_basic = NULL;
			Res::Texture* tile_texture = NULL;

			Cube* block_pointer;
			Tiles* clist[16];

			int hand_id = 1;

			Axis* axe;

			GObject* obj;

		public:
			Map* map;

		public:
			EditorMainScene();
			void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
			void render();
			void GenerateMap();
			void update();
			void dispose();
		};

		class LoadingScene : public Scene {
		private:
			Scene* todo = NULL;

		public:
			LoadingScene(Scene* tobeload);
			void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
			void render();
			void update();
			void dispose();
		};

	//-------------------------------------------------------------------------
	// ● Scene Management
	//-------------------------------------------------------------------------
		namespace SceneManager {
			extern bool render_debug_info;
			void load_scene(Scene* c);
			void render_scene();
			void update_scene();
			void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		};

	//-------------------------------------------------------------------------
	// ● Game Logic
	//-------------------------------------------------------------------------
		void init_logic();
		void update_logic();
		void render();
	}

#endif
