//=============================================================================
// ■ tiled_map.cpp
//-----------------------------------------------------------------------------
//   Tile地图
//=============================================================================

#include "tiled_map.hpp"

namespace VM76 {
	TiledMap::TiledMap(int x, int y, int z) {
		width = x; length = z; height = y;

		for (int i = 0; i < 16; i++)
			cinstance[i] = new Cube(i);
		tiles = new TileData[x * y * z];

		generate_flat();
		bake_tiles();
	}

	void TiledMap::bake_tiles() {
		int count[16];
		for (int id = 0; id < 16; id++) count[id] = 0;

		for (int x = 0; x < width; x++) {
			for (int z = 0; z < length; z++) {
				for (int y = 0; y < height; y++) {
					int id = tiles[calcTileIndex(x,y,z)].tid;
					if (id > 0) {
						id --;

						cinstance[id]->mat[count[id]] = glm::translate(glm::mat4(1.0), glm::vec3(x,y,z));
						count[id] ++;
					}
				}
			}
		}

		for (int id = 0; id < 16; id++) {
			cinstance[id]->update_instance(count[id], cinstance[id]->mat);
		}
	}

	void TiledMap::generate_flat() {
		for (int i = 0; i < width; i ++)
			for (int j = 0; j < length; j++) {
				// Generate rocks
				tiles[calcTileIndex(i, 0, j)].tid = 2; // 2 = rock

				// Generate grass
				tiles[calcTileIndex(i, 1, j)].tid = 1; // 1 = grass
			}
	}

	int TiledMap::calcTileIndex(int x, int y, int z) {
		return (width * length) * y + (length) * x + z;
	}

	void TiledMap::render() {
		for (int i = 0; i < 16; i++)
			cinstance[i]->render();
	}

	void TiledMap::dispose() {
		for (int i = 0; i < 16; i++)
			VMDE_Dispose(cinstance[i]);
	}
}
