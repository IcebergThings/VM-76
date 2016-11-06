#include <cstdlib>
#include <cstdio>
#include "VMDE/global.hpp"

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

	class Tile {
	public:
		GDrawable* obj[6] = {NULL, NULL, NULL, NULL, NULL, NULL};

	private:
		GLfloat* vtx[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
		GLuint* itx[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
		glm::mat4 *mat = NULL;

	public:
		Tile(int tid);
		void render();
		void dispose();
	};

	extern Tile* t[16];

	const GLfloat tile_size = 1.0f / 16.0f;

	struct Game {
		int forward, left, right, back;
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

	void loop();
	void start_game();
	void terminate();
	void init_tiles();
}

extern "C" {
	void client_terminate();
}

#endif
