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

		if (!read_map()) generate_V1();
	}

	void DataMap::generate_flat() {
		for (int x = 0; x < width; x++)
			for (int z = 0; z < length; z++)
				for (int y = 0; y < height; y++) {
					TileData t = map[calcIndex(x,y,z)];
					t.tid = (y == 0) ? Grass : Air;
				}
	}

	bool DataMap::read_map() {
		FILE* f = fopen("map.dat", "rb");
		if (!f) return false; else fclose(f);

		log("Reading map");
		V::VBinaryFileReader* fr = new V::VBinaryFileReader("map.dat");

		int map_version = fr->read_i32();
		if (fr->read_u8() == 'V' && fr->read_u8() == 'M'
			&& fr->read_u8() == '7' && fr->read_u8() == '6') {
				log("Map version : %d", map_version);
				for (int x = 0; x < width * length * height; x++) {
					if (x % (width * length * height / 7) == 0)
						log("Map %d%% loaded", 100 * x / (width * length * height));
					map[x].tid = fr->read_u8();
					map[x].data_flag = fr->read_u8();
				}
				return true;
			} else {
				log("Invalid map.dat");
				return false;
			}
	}

	void DataMap::save_map() {
		V::VBinaryFileWriter* fw = new V::VBinaryFileWriter("map.dat");
		// 版本号
		fw->write_i32(100);
		// 文件头标识
		fw->write_u8('V');
		fw->write_u8('M');
		fw->write_u8('7');
		fw->write_u8('6');

		for (int x = 0; x < width * length * height; x++) {
			fw->write_u8(map[x].tid);
			fw->write_u8(map[x].data_flag);
		}
		delete fw;
		log("Map saved");
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
				pos = pos * 2.1f + glm::vec2(0.1f, 0.13f); n += sin(glm::perlin(pos) * VMath::PIf * 0.5) * 0.65f * hm;
				pos = pos * 2.2f + glm::vec2(0.1f, 0.13f); n += glm::perlin(pos) * 0.35f;
				pos = pos * 2.6f + glm::vec2(0.15f, 0.1f); n += glm::perlin(pos) * 0.18f;
				pos = pos * 1.9f + glm::vec2(0.2f);
				n += sin(glm::perlin(pos) * VMath::PIf * 0.5) * 0.1f;

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

	Map::~Map() {
		for (int x = 0; x < width * length * height; x++) {
			VMDE_Dispose(delete, chunks[x]);
		}
		XE(delete[], chunks);

		log("Saving map");
		map->save_map();
	}
}
