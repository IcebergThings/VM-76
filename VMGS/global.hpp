#include <VMDE/VMDE.hpp>
#include "Tiles/tiles.hpp"

#ifndef _INCLUDE_VMGS_GLOBAL_H
#define _INCLUDE_VMGS_GLOBAL_H

namespace VM76 {

	enum TileList {
		GRASS,
		STONE,
		DIRT,
		GLASS,
		WOOD,
		STONE_BRICK,
		WALL,
		TNT,
		COBBLE_STONE
	};

	extern Tile* t[16];

	const GLfloat tile_size = 1.0f / 16.0f;

	class Player {
		bool in_gui;
		glm::vec3 pos;
		glm::vec3 look_at;
	};

	void loop();
	void start_game();
	void terminate();
	void init_tiles();

}

extern "C" {
	void client_terminate();

	void i_have_a_key(GLFWwindow* window UNUSED,
		int key UNUSED, int scancode UNUSED, int action UNUSED, int mode UNUSED);
}

#endif
