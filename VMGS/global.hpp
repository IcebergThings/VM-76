//=============================================================================
// ■ VMGS/global.hpp
//=============================================================================

#include <cstdlib>
#include <cstdio>
#include <global.hpp>
#include "Tiles/tiles.hpp"
#include "Physics/phys.hpp"

#ifndef _INCLUDE_VMGS_GLOBAL_H
#define _INCLUDE_VMGS_GLOBAL_H

#undef DEBUG_ENVIRONMENT
#define DEBUG_ENVIRONMENT "VMGS"
#define GLM_FORCE_SSE41

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

	const GLfloat tile_size = 1.0f / 16.0f;

	struct Game {
		int
			key_forward, key_left, key_right, key_back,
			key_up, key_down;
	};
	extern Game game;

	class Player {
	public:
		bool in_gui = false;
		float horizontal_angle = 0.0;
		float vertical_angle = 0.0;
		float speed = 0.0;
		glm::vec3 wpos = glm::vec3(0.0, 1.0, 0.0);
	};
	extern Player game_player;

	class Block {
	public:
		GLuint tid;
		Block(int id);
	};

	class Structure {
	private:
		Tile* t[16];
		int block_count[16];
		glm::mat4* mats[16];
		void init_tiles();

	public:
		Block* str[64][64][64];
		glm::vec3 wpos = glm::vec3(0.0);

		void dispose();
		void update();
		void render();
		Structure();
	};

	void loop();
	void start_game();
	void terminate();
	void init_tiles();
}

extern "C" {
	void client_terminate();
}

#endif
