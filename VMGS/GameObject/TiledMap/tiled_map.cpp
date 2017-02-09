//=============================================================================
// ■ tiled_map.cpp
//-----------------------------------------------------------------------------
//   Tile地图
//=============================================================================

#include "tiled_map.hpp"
#include "glm/gtc/noise.hpp"

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

	TiledMap::TiledMap(int x, int y, int z, glm::vec3 wp) {
		width = x; length = z; height = y;

		init_cinstances(cinstance);
		tiles = new TileData[x * y * z];

		mount_point = wp;
	}

	TileData air = {0, 0};

	TileData TiledMap::tileQuery(int x, int y, int z) {
		if (x < 0 || x >= width || y < 0 || y >= height || z < 0 || z >= length) return air;
		return tiles[calcTileIndex(x, y, z)];
	}

	void TiledMap::bake_tiles() {
		int count[16][6];

		glm::mat4* temp[16][6];
		for (int x = 0; x < 16; x++)
			for (int y = 0; y < 6; y++)
				temp[x][y] = new glm::mat4[8192];

		memset(count, 0, sizeof(count));

		for (int x = 0; x < width; x++) {
			for (int z = 0; z < length; z++) {
				for (int y = 0; y < height; y++) {
					int id = tiles[calcTileIndex(x,y,z)].tid;
					if (id > 0) {
						id --;

						glm::mat4 translate = glm::translate(glm::mat4(1.0), glm::vec3(x,y,z) + mount_point);

						if (tileQuery(x, y, z - 1).tid == 0) {
							temp[id][0][count[id][0]] = translate;
							count[id][0] ++;
						}

						if (tileQuery(x, y, z + 1).tid == 0) {
							temp[id][1][count[id][1]] = translate;
							count[id][1] ++;
						}

						if (tileQuery(x, y + 1, z).tid == 0) {
							temp[id][2][count[id][2]] = translate;
							count[id][2] ++;
						}

						if (tileQuery(x, y - 1, z).tid == 0) {
							temp[id][3][count[id][3]] = translate;
							count[id][3] ++;
						}

						if (tileQuery(x - 1, y, z).tid == 0) {
							temp[id][4][count[id][4]] = translate;
							count[id][4] ++;
						}

						if (tileQuery(x + 1, y, z).tid == 0) {
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

	void TiledMap::generate_void() {
		for (int i = 0; i < width; i ++)
			for (int j = 0; j < length; j++)
				for (int k = 0; k < height; k++) tiles[calcTileIndex(i, k, j)].tid = 0;
	}

	void TiledMap::generate_land() {
		for (int i = 0; i < width; i ++)
			for (int j = 0; j < length; j++) {
				glm::vec2 pos = (glm::vec2(i, j) + glm::vec2(mount_point.x, mount_point.z)) * 0.002f;
				float n = glm::perlin(pos);
				pos = pos * 1.5f + glm::vec2(0.1f, 0.13f); n += glm::perlin(pos) * 0.8f;
				pos = pos * 2.1f + glm::vec2(0.1f, 0.13f); n += glm::perlin(pos) * 0.6f;
				pos = pos * 2.2f + glm::vec2(0.1f, 0.13f); n += glm::perlin(pos) * 0.45f;
				pos = pos * 2.6f + glm::vec2(0.15f, 0.1f); n += glm::perlin(pos) * 0.15f;
				pos = pos * 4.5f + glm::vec2(0.09f); n += glm::perlin(pos) * 0.06f;

				n = glm::clamp(0.1f, n * 0.5f + 0.5f, 1.0f);
				int h = n * TERRIAN_MAX_HEIGHT - mount_point.y;
				int ho = h;
				h = glm::clamp(0, h, height);

				for (int y = 0; y < h; y++) {
					tiles[calcTileIndex(i, y, j)].tid = (y == ho - 1) ? Grass : Stone;
				}
				for (int y = h; y < height; y++) tiles[calcTileIndex(i, y, j)].tid = Air;
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
