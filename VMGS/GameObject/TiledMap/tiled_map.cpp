//=============================================================================
// ■ tiled_map.cpp
//-----------------------------------------------------------------------------
//   Tile地图
//=============================================================================

#include "tiled_map.hpp"

namespace VM76 {

	void TiledMap::init_cinstances (Tiles* cinstance[]) {
		#define FILL_ID(id, f) cinstance[id - 1] = new f;

		FILL_ID(Grass, MultiFaceCubeTile(49,49,0,2,49,49))
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

	TiledMap::TiledMap(int x, int y, int z, glm::vec3 wp, DataMap* m) {
		width = x; length = z; height = y;

		init_cinstances(cinstance);
		map = m;

		mount_point = wp;
	}

	TileData air = {0, 0};

	void TiledMap::bake_tiles() {
		int count[16][6];

		glm::mat4* temp[16][6];
		for (int x = 0; x < 16; x++)
			for (int y = 0; y < 6; y++)
				temp[x][y] = new glm::mat4[8192];

		memset(count, 0, sizeof(count));

		for (int x = mount_point.x; x < mount_point.x + width; x++) {
			for (int z = mount_point.z; z < length + mount_point.z; z++) {
				for (int y = mount_point.y; y < height + mount_point.y; y++) {
					int id = map->tileQuery(x, y, z).tid;
					if (id > 0) {
						id --;

						glm::mat4 translate = glm::translate(glm::mat4(1.0), glm::vec3(x,y,z));

						if (map->tileQuery(x, y, z - 1).tid == 0) {
							temp[id][0][count[id][0]] = translate;
							count[id][0] ++;
						}

						if (map->tileQuery(x, y, z + 1).tid == 0) {
							temp[id][1][count[id][1]] = translate;
							count[id][1] ++;
						}

						if (map->tileQuery(x, y + 1, z).tid == 0) {
							temp[id][2][count[id][2]] = translate;
							count[id][2] ++;
						}

						if (map->tileQuery(x, y - 1, z).tid == 0) {
							temp[id][3][count[id][3]] = translate;
							count[id][3] ++;
						}

						if (map->tileQuery(x - 1, y, z).tid == 0) {
							temp[id][4][count[id][4]] = translate;
							count[id][4] ++;
						}

						if (map->tileQuery(x + 1, y, z).tid == 0) {
							temp[id][5][count[id][5]] = translate;
							count[id][5] ++;
						}
					}
				}
			}
		}

		for (int id = 0; id < 16; id ++) {
			for (int x = 0; x < 6; x++) {
				xefree(cinstance[id]->mat[x]);
				if (count[id][x] > 0) {
					cinstance[id]->mat[x] = new glm::mat4[count[id][x]];
					memcpy(cinstance[id]->mat[x], temp[id][x], sizeof(glm::mat4) * count[id][x]);
				}
			}
		}

		for (int x = 0; x < 16; x++)
			for (int y = 0; y < 6; y++)
				xefree(temp[x][y]);

		for (int id = 0; id < 16; id++) {
			cinstance[id]->update_instance(
				count[id][0],count[id][1],count[id][2],
				count[id][3],count[id][4],count[id][5]
			);
		}
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
