//=============================================================================
// ■ scene.hpp
//-----------------------------------------------------------------------------
//   场景类
//=============================================================================

#ifndef VMGS_SCENE_SCENE_HPP_
#define VMGS_SCENE_SCENE_HPP_

namespace VM76 {
	//-------------------------------------------------------------------------
	// ● 所有场景类的基类
	//-------------------------------------------------------------------------
	class Scene {
	public:
		virtual void update() = 0; // Tick update
		virtual void render() = 0; // Graphics update
		virtual void event_key(int key, int action) {};
		virtual void event_keydown(int key, int mods) {};
		virtual ~Scene() {};
	};
	//-------------------------------------------------------------------------
	// ● 场景管理器
	//-------------------------------------------------------------------------
	namespace SceneManager {
		extern Scene* context;
		extern bool render_debug_info;
		void load_scene(Scene* c);
		void render_scene();
		void update_scene();
		// callbacks for GLFW
		void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	};
	//-------------------------------------------------------------------------
	// ● Scenes
	//-------------------------------------------------------------------------
	class Scene_Editor : public Scene {
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
		Camera* cam;

		int hand_id = 1;

		Axis axe;

		GObject* obj;

		RenderBuffer* postBuffer;

	public:
		PhyEngine* physics;
		PhysicsMap* phy_map;
		Map map = Map(8, 8, 8, 32);

	public:
		Scene_Editor();
		void event_key(int key, int action);
		void event_keydown(int key, int mods);
		void update();
		void render();
		void GenerateMap();
		~Scene_Editor();
	};

	class Scene_Loading : public Scene {
	private:
		Scene* todo = NULL;

	public:
		Scene_Loading(Scene** tobeload);
		void update();
		void render();
	};
}

#endif
