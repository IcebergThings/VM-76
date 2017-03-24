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
		log("Reading map");
		VBinaryFileReader* fr = new VBinaryFileReader("map.dat");
		if (!fr->f) return false;

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
		VBinaryFileWriter* fw = new VBinaryFileWriter("map.dat");
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

		const glm::mat2 rotate2D = glm::mat2(1.3623, 1.7531, -1.7131, 1.4623);

		for (int i = 0; i < width; i ++) {
			if (i % (width / 12) == 0)
				log("Generated %d%% (%d / %d)",
					(int) (100.0 / width * i),
					i * length * height, width * length * height
				);

			for (int j = 0; j < length; j++) {
				glm::vec2 pos = glm::vec2(i, j) * 0.00001f;

				float ww = glm::perlin(pos * 0.1f) + glm::perlin(-pos * 0.1f);
				ww = ww * ww * 0.5f + 0.5f;
				ww *= 2.0f;

				float w = 1.0;
				float n = .0f;
				for (int i = 0; i < 5; i++) {
					glm::vec2 xp = pos + ww * w * glm::perlin(pos);
					glm::vec2 wv = 1.0f - glm::abs(glm::sin(xp));
					glm::vec2 swv = glm::abs(glm::cos(xp));
					wv = glm::mix(wv, swv, wv);
					n += glm::pow(1.0 - glm::pow(wv.x * wv.y, 0.75), 2.0 * w);

					w *= 0.85f;
					pos = rotate2D * pos * 3.2f;
					pos -= glm::vec2(0.4f, 0.8f) * (float) i;
				}

				pos = glm::vec2(i, j) * -0.00001f; w = 1.0;
				for (int i = 0; i < 3; i++) {
					n += w * glm::simplex(pos);
					w *= -0.65f;
					pos = rotate2D * pos * 3.2f;
				}
				n *= 0.5f;
				n = glm::pow(n, 1.0 + ww);

				n = glm::clamp(n * 0.7f + 0.2f, 1.0f / (float) TERRIAN_MAX_HEIGHT, 1.0f);
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
		GDrawable::close_draw_node();
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
