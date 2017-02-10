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

	Tiles* TiledMap::get_instances (int id) {
		switch (id) {
			case Grass:
				return new MultiFaceCubeTile(49,49,0,2,49,49);
				break;
			case Stone:
				return new SimpleCubeTile(1);
				break;
			case Dirt:
				return new SimpleCubeTile(2);
				break;
			case Glass:
				return new SimpleCubeTile(3);
				break;
			case WoodPlank:
				return new SimpleCubeTile(4);
				break;
			case HalfBrick:
				return new MultiFaceCubeTile(5,5,6,6,5,5);
				break;
			case Brick:
				return new SimpleCubeTile(7);
				break;
			case TNT:
				return new MultiFaceCubeTile(8,8,9,10,8,8);
				break;
			case CobbleStone:
				return new SimpleCubeTile(16);
				break;
			default:
				return NULL;
				break;
		}
	}

	TiledMap::TiledMap(int x, int y, int z, glm::vec3 wp, DataMap* m) {
		width = x; length = z; height = y;

		//init_cinstances(cinstance);
		memset(cinstance, 0, sizeof(cinstance));
		map = m;

		mount_point = wp;
	}

	void TiledMap::bake_tiles() {
		int count[16][6];

		glm::mat4* temp[16][6];
		for (int x = 0; x < 16; x++)
			for (int y = 0; y < 6; y++)
				temp[x][y] = (glm::mat4*) malloc(sizeof(glm::mat4) * 8192);

		memset(count, 0, sizeof(count));

		for (int x = mount_point.x; x < mount_point.x + width; x++) {
			for (int z = mount_point.z; z < length + mount_point.z; z++) {
				for (int y = mount_point.y; y < height + mount_point.y; y++) {
					int id = map->tidQuery(x, y, z);
					if (id > 0) {
						id --;

						glm::mat4 tr = glm::translate(glm::mat4(1.0), glm::vec3(x,y,z));
						if (map->tidQuery(x, y, z - 1) == 0) {
							temp[id][0][count[id][0]] = tr;
							count[id][0] ++;
						}

						if (map->tidQuery(x, y, z + 1) == 0) {
							temp[id][1][count[id][1]] = tr;
							count[id][1] ++;
						}

						if (map->tidQuery(x, y + 1, z) == 0) {
							temp[id][2][count[id][2]] = tr;
							count[id][2] ++;
						}

						if (map->tidQuery(x, y - 1, z) == 0) {
							temp[id][3][count[id][3]] = tr;
							count[id][3] ++;
						}

						if (map->tidQuery(x - 1, y, z) == 0) {
							temp[id][4][count[id][4]] = tr;
							count[id][4] ++;
						}

						if (map->tidQuery(x + 1, y, z) == 0) {
							temp[id][5][count[id][5]] = tr;
							count[id][5] ++;
						}
					}
				}
			}
		}

		for (int id = 0; id < 16; id ++) {

			bool has_block_valid = false;
			for (int x = 0; x < 6; x++) if (count[id][x]) {
				has_block_valid = true;
				break;
			}

			if (has_block_valid) {
				if (!cinstance[id]) cinstance[id] = get_instances(id + 1);

				for (int x = 0; x < 6; x++) {
					if (count[id][x] > 0) {
						xefree(cinstance[id]->mat[x]);

						cinstance[id]->mat[x] = new glm::mat4[count[id][x]];
						memcpy(cinstance[id]->mat[x], temp[id][x], sizeof(glm::mat4) * count[id][x]);
					}
				}
			} else if (cinstance[id]) {
				VMDE_Dispose(cinstance[id]);
			}
		}

		for (int x = 0; x < 16; x++)
			for (int y = 0; y < 6; y++)
				xefree(temp[x][y]);

		for (int id = 0; id < 16; id++) {
			if (cinstance[id]) cinstance[id]->update_instance(
				count[id][0],count[id][1],count[id][2],
				count[id][3],count[id][4],count[id][5]
			);
		}
	}

	void TiledMap::render() {
		for (int i = 0; i < 16; i++)
			if (cinstance[i]) cinstance[i]->render();
	}

	void TiledMap::dispose() {
		for (int i = 0; i < 16; i++)
			if (cinstance[i]) VMDE_Dispose(cinstance[i]);
	}
}
