//=============================================================================
// ■ tiled_map.cpp
//-----------------------------------------------------------------------------
//   Tile地图
//=============================================================================

#include "tiled_map.hpp"

namespace VM76 {


	void TiledMap::init_cinstances () {
		#define FILL_ID(id, f) cinstance[id - 1] = new f;

		FILL_ID(Grass, SimpleCubeTile(0))
		FILL_ID(Stone, SimpleCubeTile(1))
		FILL_ID(Dirt, SimpleCubeTile(2))
		FILL_ID(Glass, SimpleCubeTile(3))
		FILL_ID(WoodPlank, SimpleCubeTile(4))
		FILL_ID(HalfBrick, MultiFaceCubeTile(5,5,6,6,5,5))
		FILL_ID(Brick, SimpleCubeTile(7))
		FILL_ID(TNT, MultiFaceCubeTile(8,8,9,10,8,8))
		FILL_ID(CobbleStone, SimpleCubeTile(16))

		// Fill the rest with MISSING TEXTURE
		for (int i = CobbleStone; i < 16; i ++) {
			FILL_ID(i + 1, SimpleCubeTile(31))
		}

		#undef FILL_ID
	}

	TiledMap::TiledMap(int x, int y, int z, glm::vec3 wp) {
		width = x; length = z; height = y;

		init_cinstances();
		tiles = new TileData[x * y * z];

		mount_point = wp;

		generate_flat();
		bake_tiles();
	}

	TileData air = {0, 0};

	TileData TiledMap::tileQuery(int x, int y, int z) {
		if (x < 0 || x >= width || y < 0 || y >= height || z < 0 || z >= length) return air;
		return tiles[calcTileIndex(x, y, z)];
	}

	void TiledMap::bake_tiles() {
		int count[16][6];
		for (int id = 0; id < 16; id++)
			for (int face = 0; face < 6; face++)
				count[id][face] = 0;

		for (int x = 0; x < width; x++) {
			for (int z = 0; z < length; z++) {
				for (int y = 0; y < height; y++) {
					int id = tiles[calcTileIndex(x,y,z)].tid;
					if (id > 0) {
						id --;

						glm::mat4 translate = glm::translate(glm::mat4(1.0), glm::vec3(x,y,z) + mount_point);

						if (tileQuery(x, y, z - 1).tid == 0) {
							cinstance[id]->mat[0][count[id][0]] = translate;
							count[id][0] ++;
						}

						if (tileQuery(x, y, z + 1).tid == 0) {
							cinstance[id]->mat[1][count[id][1]] = translate;
							count[id][1] ++;
						}

						if (tileQuery(x, y + 1, z).tid == 0) {
							cinstance[id]->mat[2][count[id][2]] = translate;
							count[id][2] ++;
						}

						if (tileQuery(x, y - 1, z).tid == 0) {
							cinstance[id]->mat[3][count[id][3]] = translate;
							count[id][3] ++;
						}

						if (tileQuery(x - 1, y, z).tid == 0) {
							cinstance[id]->mat[4][count[id][4]] = translate;
							count[id][4] ++;
						}

						if (tileQuery(x + 1, y, z).tid == 0) {
							cinstance[id]->mat[5][count[id][5]] = translate;
							count[id][5] ++;
						}
					}
				}
			}
		}

		for (int id = 0; id < 16; id++) {
			cinstance[id]->update_instance(
				count[id][0],count[id][1],count[id][2],
				count[id][3],count[id][4],count[id][5]
			);
		}
	}

	void TiledMap::generate_flat() {
		for (int i = 0; i < width; i ++)
			for (int j = 0; j < length; j++) {
				// Generate rocks
				tiles[calcTileIndex(i, 0, j)].tid = 2; // 2 = rock

				// Generate grass
				tiles[calcTileIndex(i, 1, j)].tid = 1; // 1 = grass

				// Fill the rest with air
				for (int k = 2; k < height; k++) tiles[calcTileIndex(i, k, j)].tid = 0;
			}
	}

	int TiledMap::calcTileIndex(int x, int y, int z) {
		return (width * length) * y + (length) * z + x;
	}

	int TiledMap::calcTileIndex(glm::vec3 pos) {
		return (width * length) * pos.y + (length) * pos.z + pos.x;
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
