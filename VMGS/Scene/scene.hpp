//=============================================================================
// ■ scene.hpp
//-----------------------------------------------------------------------------
//   场景类
//=============================================================================

#pragma once

namespace VM76 {
	//-------------------------------------------------------------------------
	// ● 所有场景类的基类
	//-------------------------------------------------------------------------
	class Scene {
	public:
		virtual void update();
		virtual void render();
		virtual void event_button(Input::Button button, Input::ButtonAction action);
		virtual ~Scene() {};
	};
	//-------------------------------------------------------------------------
	// ● 场景管理器
	//-------------------------------------------------------------------------
	namespace SceneManager {
		extern Scene* scene;
		extern bool debug_info_visible;
		void init();
		// 直接跳转到指定的场景
		// 用法：SceneManager::jump<场景类>(构造参数)
		template <class T, class... Args> void jump(Args&&... args) {
			XE(delete, scene);
			scene = new T(forward<Args>(args)...);
		}
		void update_scene();
		void render_scene();
		void render_debug_info();
	};
	//-------------------------------------------------------------------------
	// ● Scene_Editor
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

		RenderHierarchy* scene_node;

	public:
		PhyEngine* physics;
		PhysicsMap* phy_map;
		Map map = Map(8, 8, 8, 32);

	public:
		Scene_Editor();
		void event_button(Input::Button button, Input::ButtonAction action);
		void update();
		void render();
		void GenerateMap();
		~Scene_Editor();
	};

	//-------------------------------------------------------------------------
	// ● Scene_Loading
	//-------------------------------------------------------------------------
	class Scene_Loading : public Scene {
	private:
		thread loader_thread;
		bool loader_completed;
	public:
		Scene_Loading(void (*loader)(bool* completion_flag));
		void update();
		void render();
	};
}
