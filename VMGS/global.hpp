#include <cstdlib>
#include <cstdio>
#include <global.hpp>
#include "Tiles/tiles.hpp"

#ifndef _INCLUDE_VMGS_GLOBAL_H
#define _INCLUDE_VMGS_GLOBAL_H

#define GLM_FORCE_SSE41
#define PI 3.1415926f

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

	struct Game {
		int
			forward = GLFW_KEY_W,
			left = GLFW_KEY_A,
			right = GLFW_KEY_D,
			back = GLFW_KEY_S;
	} game;

	class Player {
	public:
		bool in_gui = false;
		float horizontal_angle = 0.0;
		float vertical_angle = 0.0;
		float speed = 0.0;
		glm::vec3 wpos = glm::vec3(0.0, 1.0, 0.0);
	};
	Player game_player;

	void loop();
	void start_game();
	void terminate();
	void init_tiles();

}

extern "C" {
	void client_terminate();
}

#endif
