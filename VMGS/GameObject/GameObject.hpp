//=============================================================================
// ■ GameObject.hpp
//=============================================================================

#pragma once
#include <vector>
#include <algorithm>

namespace VM76 {
	class GObject {
	public:
		glm::vec3 pos;
		glm::vec3 rotation;
		glm::vec3 scale;

	public:
		glm::mat4 transform();
		void move(glm::vec3 vector);
		inline void move_to(glm::vec3 vector) {
			this->pos = vector;
		}

		GObject(
			glm::vec3 position = glm::vec3(0.0),
			glm::vec3 rotation = glm::vec3(0.0),
			glm::vec3 scale = glm::vec3(1.0)
		);
	};

	enum RenderStage {
		Render_NONE,                 // Does not render
		GBuffers_Solid,             // in Hierarchy
		GBuffers_Cutout,            // in Hierarchy
		Deferred_Lighting_Opaque,   // in Hierarchy
		Render_Skybox,              // in Hierarchy
		GBuffers_NoLighting_Opaque, // in Hierarchy & Camera
		ImageEffect_Opaque,         // in Camera
		GBuffers_Transperant,       // in Hierarchy
		ImageEffect_Final           // in Camera
	};

	class RenderHierarchy : public GObject {
	public:
		RenderHierarchy* father;
		std::vector<RenderHierarchy*> children = {};

		RenderStage stage;
		RenderObject* robj;

		void sort_child();

		void push_back(RenderHierarchy* robj);
		void push_back(RenderStage stage, RenderObject* robj);
		void render(RenderStage currentStage);

		RenderHierarchy(RenderHierarchy* father, RenderStage stage = Render_NONE, RenderObject* obj = NULL);
	};
}

#include "Cube/cube.hpp"
#include "GLAxis/axis.hpp"
#include "Tile/tile.hpp"
#include "TiledMap/tiled_map.hpp"
#include "Camera/Camera.hpp"
#include "SkyBox/SkyBox.hpp"
#include "Light/Light.hpp"
