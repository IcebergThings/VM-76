//=============================================================================
// ■ structure.cpp
//-----------------------------------------------------------------------------
//   脱离方块系统的结构！
//=============================================================================

#include "global.hpp"

namespace VM76 {

	Block::Block(int id) {
		tid = id;
	}

	void Structure::init_tiles() {
		t[GRASS] = new Tile(0);
		t[STONE] = new Tile(1);
		t[DIRT] = new Tile(2);
		t[WOOD] = new Tile(4);
	}

	void Structure::render() {
		for (int x = 0; x < 16; x++) if (block_count[x] > 0)
			t[x]->render(true, true, true, true, true, true);
	}

	void Structure::update() {
		for (int x = 0; x < 16; x++) block_count[x] = 0;
		for (int x = 0; x < 64; x++) {
			for (int y = 0; y < 64; y++) {
				for (int z = 0; z < 64; z++) if (str[x][y][z] != NULL) {
					GLuint tid = str[x][y][z]->tid;
					(mats[tid])[block_count[tid]] = glm::translate(
						glm::mat4(1.0),
						glm::vec3(float(x), float(y), float(z)) + wpos
					);
					block_count[tid]++;
				}
			}
		}

		for (int x = 0; x < 16; x++) if (t[x]) {
			t[x]->update_instance(block_count[x], &(mats[x])[0]);
		}
	}

	void Structure::dispose() {
		for (int x = 0; x < 16; x++) xefree(mats[x]);
		for (int x = 0; x < 16; x++) if (t[x]) {
			t[x]->dispose();
			xefree(t[x]);
		}
	}

	Structure::Structure() {
		init_tiles();
		memset(str, 0, sizeof(str));

		for (int x = 0; x < 16; x++)
			mats[x] = new glm::mat4[128];

		block_count[0] = 1;
		str[0][0][0] = new Block(0);
		str[1][1][0] = new Block(0);

		update();
	}
}
