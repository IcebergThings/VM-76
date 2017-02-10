//=============================================================================
// ■ map.cpp
//-----------------------------------------------------------------------------
//   地图类
//=============================================================================

#include "tiled_map.hpp"
#include "glm/gtc/noise.hpp"

namespace VM76 {

	DataMap::DataMap(int w, int h, int l) {
		constStone = {1, 0};

		map = (TileData*) malloc(sizeof(TileData) * w * h * l);
		width = w; length = l; height = h;

		generate_V1();
	}

	void DataMap::generate_flat() {
		for (int x = 0; x < width; x++)
			for (int z = 0; z < length; z++)
				for (int y = 0; y < height; y++) {
					TileData t = map[calcIndex(x,y,z)];
					t.tid = (y == 0) ? Grass : Air;
				}
	}

	void DataMap::generate_V1() {
		log("Start generating maps, %d x %d x %d", width, length, height);
		for (int i = 0; i < width; i ++) {
			if (i % (width / 12) == 0)
				log("Generated %d%% (%d / %d)",
					(int) (100.0 / width * i),
					i * length * height, width * length * height
				);

			for (int j = 0; j < length; j++) {
				glm::vec2 pos = glm::vec2(i, j) * 0.001f;
				float hm = glm::perlin(pos);
				pos = pos * 1.3f; hm += glm::perlin(pos) * 0.8;
				hm = hm * hm;
				pos = pos * 1.8f; float n = glm::perlin(pos) * (hm * 0.5 + 0.5);
				pos = pos * 1.5f + glm::vec2(0.1f, 0.13f); n += glm::perlin(pos) * 0.85f * (hm * 0.5 + 0.5);
				pos = pos * 2.1f + glm::vec2(0.1f, 0.13f); n += sin(glm::perlin(pos) * Util::PIf * 0.5) * 0.65f * hm;
				pos = pos * 2.2f + glm::vec2(0.1f, 0.13f); n += glm::perlin(pos) * 0.35f;
				pos = pos * 2.6f + glm::vec2(0.15f, 0.1f); n += glm::perlin(pos) * 0.18f;
				pos = pos * 1.9f + glm::vec2(0.2f);
				n += sin(glm::perlin(pos) * Util::PIf * 0.5) * 0.1f;

				n = glm::clamp(1.0f / (float) TERRIAN_MAX_HEIGHT, n * 0.5f + 0.5f, 1.0f);
				int h = n * TERRIAN_MAX_HEIGHT;
				int ho = h;
				h = glm::clamp(0, h, height);

				for (int y = 0; y < h; y++) {
					map[calcIndex(i,y,j)].tid = (y == ho - 1) ? Grass : Stone;
				}
				for (int y = h; y < height; y++) {
					map[calcIndex(i,y,j)].tid = Air;
				}
			}
		}
		log("Generated 100%%, Complete");
	}

	Map::Map(int w, int h, int l, int csize) {
		log("Init map with size %d, %d, %d in chunk size %d", w, l, h, csize);
		CHUNK_SIZE = csize;
		width = w; length = l; height = h;
		bw = CHUNK_SIZE * w; bl = CHUNK_SIZE * l; bh = CHUNK_SIZE * h;

		chunks = new TiledMap*[w * l * h];

		map = new DataMap(bw, bh, bl);

		for (int x = 0; x < length; x++) {
			log("Baking Chunks: %d%% (%d / %d)",
				(int) (100.0 / length * x),
				x * width * height, width * length * height
			);
			for (int z = 0; z < width; z++)
				for (int y = 0; y < height; y++) {
					int ind = calcChunkIndex(x,y,z);
					chunks[ind] = new TiledMap(
						CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE,
						glm::vec3(CHUNK_SIZE * x, CHUNK_SIZE * y, CHUNK_SIZE * z),
						map
					);
					chunks[ind]->bake_tiles();
				}
		}

		log("Baking Chunks: 100%%, map initialized");
	}

	void Map::place_block(glm::vec3 dir, int tid) {
		int cx = (int) dir.x / CHUNK_SIZE;
		int cy = (int) dir.y / CHUNK_SIZE;
		int cz = (int) dir.z / CHUNK_SIZE;

		map->map[map->calcIndex(dir)].tid = tid;
		chunks[calcChunkIndex(cx,cy,cz)]->bake_tiles();
	}

	void Map::render() {
		for (int x = 0; x < width * length * height; x++)
			chunks[x]->render();
	}

	void Map::dispose() {
		for (int x = 0; x < width * length * height; x++) {
			VMDE_Dispose(delete, chunks[x]);
		}
		XE(delete[], chunks);
	}
}
