//=============================================================================
// ■ VMGS/global.hpp
//=============================================================================

#include <global.hpp>
#include <config.hpp>

#include "GameObject/GameObject.hpp"
#include "Control/control.hpp"
#include "Physics/Physics.hpp"

#ifndef _INCLUDE_VMGS_GLOBAL_H
namespace VM76 {
	#define _INCLUDE_VMGS_GLOBAL_H

	#undef DEBUG_ENVIRONMENT
	#define DEBUG_ENVIRONMENT "VMGS"

	//-------------------------------------------------------------------------
	// ● TODO
	//-------------------------------------------------------------------------
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
		virtual void key_callback(int key, int scancode, int action, int mods);
		virtual void render() = 0; // Graphics update
		virtual void update(); // Tick update
		virtual ~Scene();
	};

	class EditorMainScene : public Scene {
	private:
		Shaders shader_textured;
		Shaders gui;
		Shaders shader_basic;
		Shaders final_composite;
		Shaders deferred_lighting;
		Shaders deferred_composite;
		Res::Texture tile_texture = Res::Texture("../Media/terrain.png", NULL);

		Cube block_pointer = Cube(1);
		Tiles* clist[16];

		int hand_id = 1;

		Axis axe;

		GObject* obj;

		RenderBuffer* postBuffer;

	public:
		Map map = Map(8, 8, 8, 32);

	public:
		EditorMainScene();
		void key_callback(int key, int scancode, int action, int mods);
		void render();
		void GenerateMap();
		void update();
		~EditorMainScene();
	};

	class LoadingScene : public Scene {
	private:
		Scene* todo = NULL;

	public:
		LoadingScene(Scene** tobeload);
		void render();
		void update();
	};

	//-------------------------------------------------------------------------
	// ● Scene Management
	//-------------------------------------------------------------------------
	namespace SceneManager {
		extern Scene* context;
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
