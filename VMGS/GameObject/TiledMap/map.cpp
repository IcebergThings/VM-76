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

		for (int i = 0; i < width; i++) {
			if (i % (width / 12) == 0)
				log("Generated %d%% (%d / %d)",
					(int) (100.0 / width * i),
					i * length * height, width * length * height
				);

			for (int j = 0; j < length; j++) {
				glm::vec2 coord = glm::vec2(i, j) * 0.006f;

				const glm::mat2 rotate = glm::mat2(1.4, 1.1, -1.2, 1.4);

				glm::vec2 dir = glm::vec2(1.0, 0.1);
				float n = glm::sin(glm::perlin(coord * dir)) * 0.5f; coord *= 1.2f;dir = dir * rotate; coord += dir * 0.3f;
				dir += glm::vec2(n * 0.8, 0.0);
				n += glm::perlin(coord * dir) * 0.45f; dir = dir * rotate;
				dir += glm::vec2(n * 0.6, 0.0);
				n += glm::perlin(coord * dir) * 0.35f; dir = dir * rotate;
				dir += glm::vec2(n * 0.4, 0.0);
				n += glm::perlin(coord * dir) * 0.25f; dir = dir * rotate;
				dir += glm::vec2(n * 0.2, 0.0);
				coord *= 3.01f; dir = dir * rotate; coord += dir * 0.6f;
				n += glm::perlin(coord) * 0.125f;

				n = n + 0.5f;

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
		log("Preparing render command buffer");

		update_render_buffer();
	}

	void Map::update_render_buffer() {
		XE(delete, cmd_buffer);

		int max_count = width * length * height;
		size_t size = 0x10 + (max_count + 1) * sizeof(GDrawable*);
		ASM76::Instruct* cmd_buf = (ASM76::Instruct*) malloc(size);
		memset(cmd_buf, 0x0, size);
		cmd_buf[0] = {ASM76::INTX, CLEnum_GDrawable_batchOnce, 0x10};
		cmd_buf[1] = {ASM76::HALT, 0, 0};
		int real_count = 0;
		GDrawable** list = (GDrawable**) (((uint8_t*) cmd_buf) + 0x10);
		for (int i = 0; i < max_count; i++) {
			GDrawable* obj = chunks[i]->getBatch();
			if (!obj) {
				list[real_count] = obj;
				real_count++;
			}
		}
		size = 0x10 + (real_count + 1) * sizeof(GDrawable*);
		cmd_buf = (ASM76::Instruct*) realloc(cmd_buf, size);
		cmd_buffer = new CmdList({{.size = size, .instruct = cmd_buf}});
	}

	void Map::place_block(glm::vec3 dir, int tid) {
		int cx = (int) dir.x / CHUNK_SIZE;
		int cy = (int) dir.y / CHUNK_SIZE;
		int cz = (int) dir.z / CHUNK_SIZE;

		map->map[map->calcIndex(dir)].tid = tid;
		chunks[calcChunkIndex(cx,cy,cz)]->bake_tiles();

		update_render_buffer();
	}

	void Map::render() {
		cmd_buffer->call();
	}

	Map::~Map() {
		for (int x = 0; x < width * length * height; x++) {
			VMDE_Dispose(delete, chunks[x]);
		}
		XE(delete[], chunks);

		log("Saving map");
		map->save_map();

		XE(delete, cmd_buffer);
	}
}
